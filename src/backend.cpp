#include "backend.hpp"

#include "types.hpp"

namespace back {
    const size_t null_size = 0;
    const size_t word_size = 4;
    const size_t base_activ_record_size = 1 * word_size;
};

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
