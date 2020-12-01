#include "backend.hpp"
#include <cstdint>
#include <iostream> // TODO DEBUG

#include "ast.hpp"
#include "types.hpp"
#include "util.hpp"

namespace back {
const size_t null_size = 0;
const size_t word_size = 4;
const size_t base_activ_record_size = 1 * word_size;

const char* wasm_type_ptr = "i32";
const char* wasm_type_inte = "i32";
const char* wasm_type_real = "f32";

const char* label_fp = "$fp";
const char* label_sp = "$sp";
}; // namespace back

//
// Frontend-defined interfaces
//

namespace types {
using namespace back;

size_t PrimType::get_size() const
{
    switch (this->kind) {
        case PrimType::VOID:
            return 0;
            break;
        case PrimType::CHAR:
            return 1;
            break;
        case PrimType::INTEGER:
        case PrimType::REAL:
            return word_size;
            break;
        default:
            abort();
    }
}

size_t Pointer::get_size() const { return word_size; }

size_t Vector::get_size() const
{
    return this->num_elem * this->get_base()->get_size();
}

size_t Function::get_size() const { return null_size; }

} // namespace types

//
// Code generation
//

namespace back {

const char* prim_type_text(const types::PrimType* type)
{
    using types::PrimType;
    assert(type);
    switch (type->kind) {
        case PrimType::CHAR:
            return wasm_type_inte;
        case PrimType::INTEGER:
            return wasm_type_inte;
        case PrimType::REAL:
            return wasm_type_real;
        case PrimType::VOID:
        default:
            std::cerr << "failed getting wasm type text" << std::endl;
            std::cerr << *type << std::endl;
            abort();
    }
}

/// Gets corresponding representation of a type in WebAssembly
const char* type_text(const types::Type* type)
{
    assert(type);
    if (auto prim = dynamic_cast<const types::PrimType*>(type)) {
        return prim_type_text(prim);
    } else if (dynamic_cast<const types::Pointer*>(type)) {
        return wasm_type_ptr;
    }
    return wasm_type_ptr;
    // std::cerr <<"failed getting type text" << std::endl;
    // std::cerr << *type << std::endl;
    // abort();
}

struct {
    Counter func;
    Counter global;
    Counter local;
    Counter block;
} labels_numbers;

class Emitter {
    std::ostream& out;

  public:
    Emitter(std::ostream& out) : out(out) {}

    //
    // Emiting wasm instructions
    //

    void emit_const_ptr(uint64_t value)
    {
        out << "(" << wasm_type_ptr << ".const " << value << ")" << std::endl;
    }

    void emit_const_int(const std::string& tptxt, uint64_t value)
    {
        out << "(" << tptxt << ".const " << value << ")" << std::endl;
    }

    void emit_get_global(const std::string& label)
    {
        out << "(get_global " << label << ")" << std::endl;
    }
    void emit_set_global(const std::string& label)
    {
        out << "(set_global " << label << ")" << std::endl;
    }

    void emit_get_local(const std::string& tptxt, const std::string& label)
    {
        out << "(get_local " << label << ")" << std::endl;
    }

    void emit_load(const std::string& tptxt, bool is_byte = false)
    {
        out << "(" << tptxt << ".load";
        if (is_byte) {
            out << "8_s";
        }
        out << ")" << std::endl;
    }

    void emit_drop(size_t num = 1)
    {
        for (size_t i = 0; i < num; ++i) {
            out << "(drop)" << std::endl;
        }
    }

    void emit_get_fp() { emit_get_global(label_fp); }
    void emit_get_sp() { emit_get_global(label_sp); }

    void emit_set_fp() { emit_set_global(label_fp); }
    void emit_set_sp() { emit_set_global(label_sp); }

    void emit_add(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "add"
            << ")" << std::endl;
    }

    //
    // AST handling
    //

    void emit_program(ast::Program* root)
    {
        for (auto child : root->get_children()) {
            if (auto func_def = dynamic_cast<ast::FuncDef*>(child)) {
                labels_numbers.local.reset();
                labels_numbers.block.reset();

                auto name = func_def->ref.get().name;
                out << "(func ";
                out << "$" << name << std::endl;
                auto block = func_def->get_child();
                emit_stmt(block);
                out << ")" << std::endl;
            }
        }
    }

    void emit_stmt(ast::Statement* stmt)
    {
        if (auto block = dynamic_cast<ast::Block*>(stmt)) {
            for (auto child_stmt : block->get_children()) {
                emit_stmt(child_stmt);
            }
        } else if (auto exprstmt = dynamic_cast<ast::ExpressionStmt*>(stmt)) {
            auto expr = exprstmt->get_child();
            emit_expr(expr);
            auto type = expr->get_type();
            auto size = type->get_size();
            if (size > 0) {
                emit_drop();
                // emit_drop(div_ceil(size, word_size));
            }
        }
    }

    void emit_expr(ast::Expr* expr)
    {
        if (auto var = dynamic_cast<ast::Variable*>(expr)) {
            emit_var(var);
        } else if (auto binop = dynamic_cast<ast::BinOp*>(expr)) {
            emit_binop(binop);
        }
    }

    static const char* binop_instr(ast::BinOp* binop)
    {
        assert(binop);
        if (dynamic_cast<ast::Plus*>(binop)) {
            return "add";
        }
        std::cerr << "unknown operator " << (*binop) << std::endl;
        abort();
    }

    void emit_binop(ast::BinOp* expr)
    {
        assert(expr);
        emit_expr(expr->get_left());
        emit_expr(expr->get_right());
        auto type = expr->get_type();
        auto type_txt = type_text(type);
        auto instr = binop_instr(expr);
        out << "(" << type_txt << "." << instr << ")" << std::endl;
    }

    void emit_var(ast::Variable* var)
    {
        assert(var);
        auto& var_row = var->ref.get();
        auto type = var_row.type;
        auto type_txt = type_text(type);
        auto offset = assert_derref(var_row.offset);
        bool is_byte = type->is_compatible_with(types::prim_char);

        emit_const_int(wasm_type_ptr, offset);
        // If it is a local variable, sum frame pointer
        if (!var_row.is_global) {
            emit_get_fp();
            emit_add(wasm_type_ptr);
        }
        emit_load(type_txt, is_byte);
    }
};

void generate_code(std::ostream& out, ast::Program* root)
{
    out << "(module" << std::endl;
    out << "(memory 1)" << std::endl;
    out << "(global " << label_fp << " i32 (i32.const 0))" << std::endl;
    out << "(global " << label_sp << " i32 (i32.const 0))" << std::endl;    // TODO initial value
    Emitter emt(out);
    emt.emit_program(root);
    out << ")" << std::endl;
}

} // namespace back
