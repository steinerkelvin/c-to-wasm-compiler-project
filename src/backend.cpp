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

// TODO move to util.hpp
template <typename T>
T div_ceil(T a, T b)
{
    return a / b + (a % b != 0);
}

namespace back {

const char* prim_type_text(const types::PrimType* type)
{
    using types::PrimType;
    assert(type);
    switch (type->kind) {
        case PrimType::CHAR:
            return "i32";
        case PrimType::INTEGER:
            return "i32";
        case PrimType::REAL:
            return "f32";
        case PrimType::VOID:
        default:
            std::cerr << *type << std::endl;
            assert("failed getting type wasm text" && 0);
    }
}

/// Gets corresponding representation of a type in WebAssembly
const char* type_text(const types::Type* type)
{
    assert(type);
    if (dynamic_cast<const types::Pointer*>(type)) {
        return "i32";
    } else if (auto prim = dynamic_cast<const types::PrimType*>(type)) {
        return prim_type_text(prim);
    }
    return "i32";
    // std::cerr << *type << std::endl;
    // assert("failed getting type text" && 0);
}

struct {
    Counter func;
    Counter global;
    Counter local;
    Counter block;
} labels_numbers;

void emit_program(std::ostream& out, ast::Program* root);
void emit_funcdef(std::ostream& out, ast::FuncDef* func_def);
void emit_stmt(std::ostream& out, ast::Statement* root);
void emit_expr(std::ostream& out, ast::Expr* expr);
void emit_binop(std::ostream& out, ast::BinOp* expr);
void emit_var(std::ostream& out, ast::Variable* var);

//
// Emiting wasm instructions
//

void emit_const(std::ostream& out, const std::string& tptxt, uint64_t value)
{
    out << "(i32.const " << value << ")" << std::endl;
}

void emit_load(
    std::ostream& out, const std::string& tptxt, bool is_byte = false)
{
    out << "(" << tptxt << ".load";
    if (is_byte) {
        out << "8_s";
    }
    out << ")" << std::endl;
}

void emit_drop(std::ostream& out, size_t num = 1)
{
    for (size_t i = 0; i < num; ++i) {
        out << "(drop)" << std::endl;
    }
}

//
// AST handling
//

void generate_code(std::ostream& out, ast::Program* root)
{
    out << "(module" << std::endl;
    out << "(memory 1)" << std::endl;
    emit_program(out, root);
    out << ")" << std::endl;
}

void emit_program(std::ostream& out, ast::Program* root)
{
    for (auto child : root->get_children()) {
        if (auto func_def = dynamic_cast<ast::FuncDef*>(child)) {
            labels_numbers.local.reset();
            labels_numbers.block.reset();

            auto name = func_def->ref.get().name;
            out << "(func ";
            out << "$" << name << std::endl;
            auto block = func_def->get_child();
            emit_stmt(out, block);
            out << ")" << std::endl;
        }
    }
}

void emit_stmt(std::ostream& out, ast::Statement* stmt)
{
    if (auto block = dynamic_cast<ast::Block*>(stmt)) {
        for (auto child_stmt : block->get_children()) {
            emit_stmt(out, child_stmt);
        }
    } else if (auto exprstmt = dynamic_cast<ast::ExpressionStmt*>(stmt)) {
        auto expr = exprstmt->get_child();
        emit_expr(out, expr);
        auto type = expr->get_type();
        auto size = type->get_size();
        if (size > 0) {
            emit_drop(out);
            // emit_drop(out, div_ceil(size, word_size));
        }
    }
}

void emit_expr(std::ostream& out, ast::Expr* expr)
{
    if (auto var = dynamic_cast<ast::Variable*>(expr)) {
        emit_var(out, var);
    } else if (auto binop = dynamic_cast<ast::BinOp*>(expr)) {
        emit_binop(out, binop);
    }
}

static const char* binop_instr(ast::BinOp* binop)
{
    assert(binop);
    auto type = binop->get_type();
    auto type_str = type_text(type);
    if (auto plus = dynamic_cast<ast::Plus*>(binop)) {
        return "add";
    }
    // return "";
    abort();
}

void emit_binop(std::ostream& out, ast::BinOp* expr)
{
    assert(expr);
    emit_expr(out, expr->get_left());
    emit_expr(out, expr->get_right());
    auto type = expr->get_type();
    auto type_txt = type_text(type);
    auto instr = binop_instr(expr);
    out << "(" << type_txt << "." << instr << ")" << std::endl;
}

void emit_var(std::ostream& out, ast::Variable* var)
{
    assert(var);
    auto& row = var->ref.get();
    auto type = row.type;
    auto type_str = type_text(type);
    auto offset = assert_derref(row.offset);
    bool is_byte = type->is_compatible_with(types::prim_char);
    emit_const(out, type_str, offset);
    emit_load(out, type_str, is_byte);
}

} // namespace back
