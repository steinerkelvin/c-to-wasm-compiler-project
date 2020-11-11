#include "types.hpp"

namespace types {

const char* get_prim_text(PrimType kind)
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

std::ostream& operator<<(std::ostream& stream, const types::Type& type)
{
    using types::PrimType;
    switch (type.kind) {
        case PrimType::VOID:
            stream << "void";
            break;
        case PrimType::CHAR:
            stream << "char";
            break;
        case PrimType::INTEGER:
            stream << "integer";
            break;
        case PrimType::REAL:
            stream << "real";
            break;
        default:
            abort();
    }
    return stream;
}
