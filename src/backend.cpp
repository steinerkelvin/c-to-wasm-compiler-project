#include "backend.hpp"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stack>

#include "ast.hpp"
#include "types.hpp"
#include "util.hpp"

namespace back {
const size_t null_size = 0;
const size_t word_size = 4;
const size_t table_align = 8;
const size_t base_activ_record_size = 1 * word_size;
// const size_t stack_size = 1 << 20; // 1 MiB

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
    std::cerr << "failed getting type text for: " << *type << std::endl;
    abort();
}

struct LabelCounter : util::Counter {
    const std::string prefix;
    LabelCounter(const std::string prefix) : prefix(prefix) {}
    std::string next_label()
    {
        std::string txt("$");
        txt += prefix;
        txt += std::to_string(this->next());
        return txt;
    }
};

struct LoopsCounter {
    util::Counter loop_counter;
    std::stack<size_t> loop_stack;
    void reset()
    {
        loop_counter.reset();
        while (!loop_stack.empty()) {
            loop_stack.pop();
        }
    }
    void push()
    {
        auto num = loop_counter.next();
        loop_stack.push(num);
    }
    void pop()
    {
        loop_stack.pop();
        loop_counter.prev();
    }
    std::string get_label_continue()
    {
        assert(!loop_stack.empty());
        auto num = loop_stack.top();
        std::string txt("$loop");
        txt += std::to_string(num);
        return txt;
    }
    std::string get_label_break()
    {
        assert(!loop_stack.empty());
        auto num = loop_stack.top();
        std::string txt("$loopbrk");
        txt += std::to_string(num);
        return txt;
    }
    std::string get_label_aux()
    {
        assert(!loop_stack.empty());
        auto num = loop_stack.top();
        std::string txt("$loopaux");
        txt += std::to_string(num);
        return txt;
    }
};

class Emitter {
    std::ostream& out;
    const size_t strtb_pos;
    const size_t data_pos;

    struct {
        LabelCounter func{"f"};
        LabelCounter global{"g"};
        LabelCounter local{"v"};
        LabelCounter block{"b"};
    } labels;

    LoopsCounter loops;

    void reset_func_labels()
    {
        labels.local.reset();
        labels.block.reset();
        loops.reset();
    }

  public:
    Emitter(std::ostream& out, size_t strtb_pos, size_t data_pos)
        : out(out), strtb_pos(strtb_pos), data_pos(data_pos)
    {}

    //
    // Emiting Wasm instructions
    //

    void emit_comment(const std::string& txt)
    {
        out << ";; " << txt << std::endl;
    }

    void emit_const_ptr(uint64_t value)
    {
        out << "(" << wasm_type_ptr << ".const " << value << ")" << std::endl;
    }
    void emit_const_int(const std::string& tptxt, uint64_t value)
    {
        out << "(" << tptxt << ".const " << value << ")" << std::endl;
    }
    void emit_const_real(const std::string& tptxt, double value)
    {
        out << "(" << tptxt << ".const " << value << ")" << std::endl;
    }

    void emit_get_local(size_t idx)
    {
        out << "(get_local " << idx << ")" << std::endl;
    }
    void emit_get_local(const std::string& label)
    {
        out << "(get_local " << label << ")" << std::endl;
    }
    void emit_set_local(const std::string& label)
    {
        out << "(set_local " << label << ")" << std::endl;
    }
    void emit_tee_local(const std::string& label)
    {
        out << "(tee_local " << label << ")" << std::endl;
    }

    void emit_get_global(const std::string& label)
    {
        out << "(get_global " << label << ")" << std::endl;
    }
    void emit_set_global(const std::string& label)
    {
        out << "(set_global " << label << ")" << std::endl;
    }

    void emit_drop(size_t num = 1)
    {
        for (size_t i = 0; i < num; ++i) {
            out << "(drop)" << std::endl;
        }
    }

    void emit_load(const std::string& tptxt, bool is_byte = false)
    {
        out << "(" << tptxt << ".load";
        if (is_byte) {
            out << "8_s";
        }
        out << ")" << std::endl;
    }
    void emit_store(const std::string& tptxt, bool is_byte = false)
    {
        out << "(" << tptxt << ".store";
        if (is_byte) {
            out << "8_s";
        }
        out << ")" << std::endl;
    }

    void emit_call(const std::string& label)
    {
        out << "(call $" << label << ")" << std::endl;
    }

    void emit_get_fp() { emit_get_global(label_fp); }
    void emit_set_fp() { emit_set_global(label_fp); }
    void emit_get_sp() { emit_get_global(label_sp); }
    void emit_set_sp() { emit_set_global(label_sp); }

    void emit_get_temp(const std::string& tptxt) {
        std::string label("$temp_");
        label += tptxt;
        emit_get_global(label);
    }
    void emit_set_temp(const std::string& tptxt) {
        std::string label("$temp_");
        label += tptxt;
        emit_set_global(label);
    }
    void emit_tee_temp(const std::string& tptxt) {
        emit_set_temp(tptxt);
        emit_get_temp(tptxt);
    }

    void emit_add(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "add"
            << ")" << std::endl;
    }
    void emit_sub(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "sub"
            << ")" << std::endl;
    }

    void emit_eq(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "eq"
            << ")" << std::endl;
    }
    void emit_ne(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "ne"
            << ")" << std::endl;
    }
    void emit_eqz(const std::string& tptxt)
    {
        out << "(" << tptxt << "."
            << "eqz"
            << ")" << std::endl;
    }

    void emit_br(const std::string& label)
    {
        out << "(br " << label << ")" << std::endl;
    }
    void emit_br_if(const std::string& label)
    {
        out << "(br_if " << label << ")" << std::endl;
    }

    void emit_return() { out << "(return)" << std::endl; }

    //
    // AST handling
    //

    void emit_program(ast::Program* root)
    {
        for (auto child : root->get_children()) {
            if (auto func_def = dynamic_cast<ast::FuncDef*>(child)) {
                emit_func_def(func_def);
            } else {
                std::cerr << "NOT IMPLEMENTED: " << (*child) << std::endl;
                abort();
            }
        }
    }

    void emit_func_def(ast::FuncDef* func_def)
    {
        labels.local.reset();
        labels.block.reset();

        auto& func_row = func_def->ref.get();
        auto func_type =
            assert_ret(dynamic_cast<types::Function*>(func_row.type));

        auto name = func_row.name;
        out << "(func "
            << "$" << name << std::endl;

        // Parameter types
        for (auto [o_pr_name, pr_type] : func_type->parameters) {
            out << "(param " << type_text(pr_type) << ")" << std::endl;
        }

        // Return type
        auto ret_type = func_type->get_base();
        if (ret_type->get_size() > 0) {
            out << "(result " << type_text(ret_type) << ")" << std::endl;
        }

        auto block = func_def->get_child();
        auto o_scope_id = block->scope_id;
        auto& scope = symtb::get_scope(assert_derref(o_scope_id));
        auto frame_size = assert_derref(scope.frame_size);

        // Updates stack pointer, adding frame size
        emit_get_sp();
        emit_const_int(wasm_type_ptr, frame_size);
        emit_add(wasm_type_ptr);
        emit_set_sp();

        {
            // emit parameter variables initialization code
            size_t i = 0;
            for (auto [o_name, type] : func_type->parameters) {
                auto name = assert_derref(o_name);
                emit_comment(name);
                auto type_txt = type_text(type);
                auto is_byte = type->is_compatible_with(types::prim_char);
                auto o_ref = scope.lookup_var(name);
                auto ref = assert_derref(o_ref);
                emit_var_loc_by_ref(ref);
                emit_get_local(i++);
                emit_store(type_txt, is_byte);
            }
        }

        // Emits function body
        emit_stmt(block);

        // Ungrows stack
        emit_get_sp();
        emit_const_int(wasm_type_ptr, frame_size);
        emit_sub(wasm_type_ptr);
        emit_set_sp();

        out << ")" << std::endl << std::endl;
    }

    void emit_return_value(ast::ReturnValue* ret)
    {
        auto expr = ret->get_child();
        emit_expr(expr);
        emit_return();
    }

    void emit_stmt(ast::Statement* stmt)
    {
        assert(stmt);
        if (dynamic_cast<ast::EmptyStmt*>(stmt)) {
            // empty statement
        } else if (auto block = dynamic_cast<ast::Block*>(stmt)) {
            emit_comment("{");
            for (auto child_stmt : block->get_children()) {
                emit_stmt(child_stmt);
            }
            emit_comment("}");
        } else if (dynamic_cast<ast::Return*>(stmt)) {
            emit_return();
        } else if (auto ret_stmt = dynamic_cast<ast::ReturnValue*>(stmt)) {
            emit_return_value(ret_stmt);
        } else if (auto if_stmt = dynamic_cast<ast::IfStmt*>(stmt)) {
            emit_if(if_stmt);
        } else if (auto while_stmt = dynamic_cast<ast::WhileStmt*>(stmt)) {
            emit_while_loop(while_stmt);
        } else if (auto for_stmt = dynamic_cast<ast::ForStmt*>(stmt)) {
            emit_for_loop(for_stmt);
        } else if (auto break_stmt = dynamic_cast<ast::Break*>(stmt)) {
            emit_break(break_stmt);
        } else if (auto continue_stmt = dynamic_cast<ast::Continue*>(stmt)) {
            emit_continue(continue_stmt);
        } else if (auto expr_stmt = dynamic_cast<ast::ExprStmt*>(stmt)) {
            emit_expr_stmt(expr_stmt);
        } else {
            std::cerr << "NOT IMPLEMENTED: " << (*stmt) << std::endl;
            assert(0);
        }
    }

    void emit_if(ast::IfStmt* if_stmt)
    {
        auto if_label = labels.block.next_label();
        auto cond = if_stmt->get_left();
        auto body = if_stmt->get_right();
        out << "(block " << if_label << std::endl;
        emit_expr(cond);
        emit_eqz(wasm_type_inte);
        emit_br_if(if_label);
        emit_stmt(body);
        out << ")" << std::endl;
    }

    void emit_while_loop(ast::WhileStmt* while_stmt)
    {
        loops.push();
        auto break_label = loops.get_label_break();
        auto continue_label = loops.get_label_continue();
        auto cond = while_stmt->get_left();
        auto body = while_stmt->get_right();
        out << "(block " << break_label << std::endl;
        out << "(loop " << continue_label << std::endl;
        emit_expr(cond);
        emit_eqz(wasm_type_inte);
        emit_br_if(break_label);
        emit_stmt(body);
        emit_br(continue_label);
        out << ")" << std::endl;
        out << ")" << std::endl;
        loops.pop();
    }

    void emit_for_loop(ast::ForStmt* for_stmt)
    {
        loops.push();
        auto break_label = loops.get_label_break();
        auto continue_label = loops.get_label_continue();
        auto aux_label = loops.get_label_aux();
        auto init = for_stmt->get_init();
        auto cond_opt = for_stmt->get_cond();
        auto incr = for_stmt->get_incr();
        auto body = for_stmt->get_body();
        emit_stmt(init);
        out << "(block " << break_label << std::endl;
        out << "(loop " << aux_label << std::endl;
        out << "(block " << continue_label << std::endl;
        if (cond_opt) {
            emit_expr(*cond_opt);
            emit_eqz(wasm_type_inte);
            emit_br_if(break_label);
        }
        emit_stmt(body);
        out << ")" << std::endl; // inner block
        emit_stmt(incr);
        emit_br(aux_label);
        out << ")" << std::endl; // loop
        out << ")" << std::endl; // outer block
        loops.pop();
    }

    void emit_break(ast::Break* break_stmt)
    {
        auto block_label = loops.get_label_break();
        emit_br(block_label);
    }

    void emit_continue(ast::Continue* continue_stmt)
    {
        auto block_label = loops.get_label_continue();
        emit_br(block_label);
    }

    void emit_expr_stmt(ast::ExprStmt* expr_stmt)
    {
        auto expr = expr_stmt->get_child();
        emit_expr(expr);
        auto type = expr->get_type();
        auto size = type->get_size();

        if (size > 0) {
            emit_drop();
            // emit_drop(util::div_ceil(size, word_size));
        }
    }

    void emit_expr(ast::Expr* expr)
    {
        if (auto var = dynamic_cast<ast::Variable*>(expr)) {
            emit_var(var);
        } else if (auto int_val = dynamic_cast<ast::IntegerValue*>(expr)) {
            emit_int_val(int_val);
        } else if (auto real_val = dynamic_cast<ast::RealValue*>(expr)) {
            emit_real_val(real_val);
        } else if (auto str_val = dynamic_cast<ast::StringValue*>(expr)) {
            emit_str_val(str_val);
        } else if (auto assign_stmt = dynamic_cast<ast::Assign*>(expr)) {
            emit_assign(assign_stmt);
        } else if (auto binop = dynamic_cast<ast::BinOp*>(expr)) {
            emit_simple_binop(binop);
        } else if (auto func_call = dynamic_cast<ast::Call*>(expr)) {
            emit_func_call(func_call);
        } else if (auto derref = dynamic_cast<ast::Derreference*>(expr)) {
            emit_derref(derref);
        } else if (auto v2p = dynamic_cast<ast::V2P*>(expr)) {
            auto child = v2p->get_child();
            auto child_lex = assert_ret(dynamic_cast<ast::LExpr*>(child));
            emit_lexpr_loc(child_lex);
        }
        // else if (auto derref = dynamic_cast<ast::IndexAccess*>(expr)) {
        // TODO
        // }
        else {
            std::cerr << "NOT IMPLEMENTED: " << (*expr) << std::endl;
            assert(0);
        }
    }

    void emit_var_loc(ast::Variable* var)
    {
        assert(var);
        emit_var_loc_by_ref(var->ref);
    }

    void emit_var_loc_by_ref(const symtb::VarRef& var_ref)
    {
        auto& var_row = var_ref.get();
        auto offset = assert_derref(var_row.offset);

        // If it is a local variable, sum frame pointer
        if (!var_row.is_global) {
            emit_get_fp();
            emit_const_int(wasm_type_ptr, offset);
            emit_add(wasm_type_ptr);
        } else {
            emit_const_int(wasm_type_ptr, data_pos + offset);
        }
    }

    void emit_var(ast::Variable* var)
    {
        assert(var);
        emit_var_loc(var);

        auto& var_row = var->ref.get();
        auto type = var_row.type;
        auto type_txt = type_text(type);
        bool is_byte = type->is_compatible_with(types::prim_char);
        emit_load(type_txt, is_byte);
    }

    void emit_int_val(const ast::IntegerValue* val_node)
    {
        auto val = val_node->get_value();
        emit_const_int(wasm_type_inte, val);
    }
    void emit_real_val(const ast::RealValue* val_node)
    {
        auto val = val_node->get_value();
        emit_const_real(wasm_type_real, val);
    }
    void emit_str_val(const ast::StringValue* val_node)
    {
        auto offse_opt = val_node->get_ref().get().offset;
        auto offset = assert_derref(offse_opt);
        emit_const_int(wasm_type_ptr, strtb_pos + offset);
    }

    void emit_assign(ast::Assign* assign_stmt)
    {
        auto target = assign_stmt->get_left();

        auto type = target->get_type();
        auto type_txt = type_text(type);
        char is_byte = type->is_compatible_with(types::prim_char);
        // TODO refactor into emit_store etc

        auto ltarget = assert_ret(dynamic_cast<ast::LExpr*>(target));
        emit_lexpr_loc(ltarget);

        auto source = assign_stmt->get_right();
        emit_expr(source);
        emit_tee_temp(type_txt);

        emit_store(type_txt, is_byte);

        emit_get_temp(type_txt);
    }

    void emit_func_call(ast::Call* func_call)
    {
        auto func_val = func_call->get_left();
        assert(func_val);
        auto args = func_call->get_right()->get_children();
        for (auto& arg : args) {
            emit_expr(arg);
        }
        if (auto var_node = dynamic_cast<ast::Variable*>(func_val)) {
            auto& var_row = var_node->ref.get();
            auto& var_name = var_row.name;

            // Stores current frame pointer inside stack frame of next function
            emit_get_sp();
            emit_get_fp();
            emit_store(wasm_type_ptr);

            // Updates frame pointer
            emit_get_sp();
            emit_set_fp();

            emit_call(var_name);

            // Restores frame pointer
            emit_get_fp();
            emit_load(wasm_type_ptr);
            emit_set_fp();
        } else {
            std::cerr << "called value is not a name/symbol " << (*func_val)
                      << std::endl;
            abort();
        }
    }

    static const char* binop_instr(ast::BinOp* binop)
    {
        assert(binop);
        if (dynamic_cast<ast::Plus*>(binop)) {
            return "add";
        } else if (dynamic_cast<ast::Minus*>(binop)) {
            return "sub";
        } else if (dynamic_cast<ast::Times*>(binop)) {
            return "mul";
        } else if (dynamic_cast<ast::Over*>(binop)) {
            return "div_s"; // TODO handle real
        } else if (dynamic_cast<ast::Mod*>(binop)) {
            return "rem_s"; // TODO handle real
        } else if (dynamic_cast<ast::Equal*>(binop)) {
            return "eq";
        } else if (dynamic_cast<ast::NotEqual*>(binop)) {
            return "ne";
        } else if (dynamic_cast<ast::Less*>(binop)) {
            return "lt_s"; // TODO handle real
        } else if (dynamic_cast<ast::Greater*>(binop)) {
            return "gt_s";
        } else if (dynamic_cast<ast::LessEqual*>(binop)) {
            return "le_s";
        } else if (dynamic_cast<ast::GreaterEqual*>(binop)) {
            return "ge_s";
        }
        std::cerr << "unknown bin operator " << (*binop) << std::endl;
        abort();
    }

    void emit_simple_binop(ast::BinOp* expr)
    {
        assert(expr);
        emit_expr(expr->get_left());
        emit_expr(expr->get_right());
        auto type = expr->get_type();
        auto type_txt = type_text(type);
        auto instr = binop_instr(expr);
        out << "(" << type_txt << "." << instr << ")" << std::endl;
    }

    void emit_derref(ast::Derreference* derref)
    {
        auto ptr_expr = derref->get_child();
        auto ptype =
            assert_ret(dynamic_cast<types::Pointer*>(ptr_expr->get_type()));
        auto base_type = ptype->get_base();
        auto type_txt = type_text(base_type);
        emit_expr(ptr_expr);
        emit_load(type_txt);
    }

    /**
     * Emits code to compute location of a lexpr on memory
     */
    void emit_lexpr_loc(ast::LExpr* lexpr)
    {
        // action_if<ast::Variable, ast::LExpr>(
        //     lexpr, [this](ast::Variable* e) { emit_var_loc(e); });
        if (auto var = dynamic_cast<ast::Variable*>(lexpr)) {
            emit_var_loc(var);
        } else if (auto derref = dynamic_cast<ast::Derreference*>(lexpr)) {
            auto ptr_expr = derref->get_child();
            emit_expr(ptr_expr);
        }
        // else if (auto access = dynamic_cast<ast::IndexAccess*>(lexpr)) {
        //     auto ptr_expr = access->get_left();
        //     auto idx_expr = access->get_right();
        //     // TODO calc offset
        // }
        else {
            std::cerr << "NOT IMPLEMENTED: " << (*lexpr) << std::endl;
            assert(0);
        }
    }
};

const char* header = R"BLOCK(
(module
    (import "std" "readln" (func $readln (param i32) (param i32) (result i32)))
    (import "std" "_ln" (func $_ln))
    (import "std" "_print" (func $_print (param i32) (param i32)))
    (import "std" "_println" (func $_println (param i32) (param i32)))
    (import "std" "print_int" (func $print_int (param i32)))
    (import "std" "print_real" (func $print_real (param f32)))
    (import "std" "println_int" (func $println_int (param i32)))
    (import "std" "println_real" (func $println_real (param f32)))
    (import "std" "print_int_pad" (func $print_int_pad (param i32) (param i32)))
    (import "std" "print_real_pad" (func $print_real_pad (param f32) (param i32)))

    (export "memory" (memory $mem))
    (export "str_len" (func $str_len))
    (export "str_copy" (func $str_copy))
    (export "str_end" (func $str_end))
    (export "str_cat" (func $str_cat))
    (export "main" (func $main))

    (memory $mem 1)

    (func $str_len (param $po i32) (result i32)
        (local $idx i32)
        (block $out
            (loop $loop
                local.get $po
                local.get $idx
                i32.add
                i32.load8_u
                i32.eqz
                br_if $out
                local.get $idx
                i32.const 1
                i32.add
                local.set $idx
                br $loop
            )
        )
        local.get $idx
    )
    (func $str_copy (param $dest i32) (param $src i32)
        (local $c i32)
        (block $out
            (loop $loop
                local.get $src
                i32.load8_u
                local.tee $c
                i32.eqz
                br_if $out
                local.get $dest
                local.get $c
                i32.store8
                local.get $src
                i32.const 1
                i32.add
                local.set $src
                local.get $dest
                i32.const 1
                i32.add
                local.set $dest
                br $loop
            )
        )
    )
    (func $str_end (param $po i32) (result i32)
        (block $out
            (loop $loop
                local.get $po
                i32.load8_u
                i32.eqz
                br_if $out
                local.get $po
                i32.const 1
                i32.add
                local.set $po
                br $loop
            )
        )
        local.get $po
    )
    (func $print (param $po i32)
        local.get $po
        local.get $po
        call $str_len
        call $_print
    )
    (func $println (param $po i32)
        local.get $po
        call $print
        call $_ln
    )
    (func $str_cat (param $dest i32) (param $src i32)
        local.get $dest
        call $str_end
        local.get $src
        call $str_copy
    )

)BLOCK";

const char* footer = ")\n";

void generate_code(
    std::ostream& out, ast::Program* root, size_t strtb_size, size_t data_size)
{
    const size_t base_pos = 0;
    const size_t strtb_pos = base_pos;
    const size_t data_pos =
        util::ceil<size_t>(strtb_pos + strtb_size, table_align);
    const size_t stack_pos =
        util::ceil<size_t>(data_pos + data_size, table_align);

    std::cerr << "strtb => "
              << "pos: " << strtb_pos << " "
              << "size: " << strtb_size << std::endl;

    std::cerr << "data => "
              << "pos: " << data_pos << " "
              << "size: " << data_size << std::endl;

    out << header;
    // TODO initial stack pointer value
    out << "(global " << label_fp << " (mut i32) (i32.const " << stack_pos
        << "))" << std::endl;
    out << "(global " << label_sp << " (mut i32) (i32.const " << stack_pos
        << "))" << std::endl;
    out << std::endl;
    out << "(global $temp_i32 (mut i32) (i32.const 0))" << std::endl;
    out << "(global $temp_f32 (mut f32) (f32.const 0))" << std::endl;
    out << std::endl;

    strtb::visit([&](const StrRow& str_row) {
        size_t offset = assert_derref(str_row.offset);
        out << "(data ";
        out << "(i32.const " << offset << ")";
        out << " \"" << str_row.content << "\"";
        out << ")" << std::endl;
    });

    Emitter emt(out, strtb_pos, data_pos);
    emt.emit_program(root);
    out << footer;
}

} // namespace back
