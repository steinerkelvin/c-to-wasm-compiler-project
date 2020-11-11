#include "types.hpp"

namespace types {

const char* get_prim_text(PrimKind kind)
{
    switch (kind) {
        case VOID:
            return "void";
            break;
        case CHAR:
            return "char";
            break;
        case INTEGER:
            return "integer";
            break;
        case REAL:
            return "real";
            break;
    }
    abort();
}

} // namespace types

// TODO refatorar usando função acima
std::ostream& operator<<(std::ostream& stream, const types::PrimType& type)
{
    using types::PrimKind;
    switch (type.kind) {
        case PrimKind::VOID:
            stream << "void";
            break;
        case PrimKind::CHAR:
            stream << "char";
            break;
        case PrimKind::INTEGER:
            stream << "integer";
            break;
        case PrimKind::REAL:
            stream << "real";
            break;
        default:
            abort();
    }
    return stream;
}
