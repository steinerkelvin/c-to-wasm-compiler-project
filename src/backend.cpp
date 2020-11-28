#include "backend.hpp"

#include "ast.hpp"
#include "types.hpp"

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

namespace back {

// // TODO move to util.hpp
// template <typename T, typename B>
// void action_if(B* value, std::function<void(T*)> action)
// {
//     auto cast_value = dynamic_cast<T*>(value);
//     if (cast_value != nullptr) {
//         action(cast_value);
//     }
// }

void emit_program(std::ostream& out, ast::Program* root);
void emit_funcdef(std::ostream& out, ast::FuncDef* func_def);

void generate_code(std::ostream& out, ast::Program* root)
{
    out << "(module" << std::endl;
    emit_program(out, root);
    out << ")" << std::endl;
}

void emit_program(std::ostream& out, ast::Program* root)
{
    for (auto child : root->get_children()) {
        if (auto func_def = dynamic_cast<ast::FuncDef*>(child)) {
            auto name = func_def->ref.get().name;
            out << "(func ";
            out << "$" << name << std::endl;
            out << ")" << std::endl;
        }
    }
}

} // namespace back
