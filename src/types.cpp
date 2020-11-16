#include "types.hpp"

namespace types {

const char *get_prim_text(PrimKind kind)
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

std::ostream &operator<<(std::ostream &stream, const types::Type &type)
{
    return type.write_repr(stream);
}

std::ostream &types::PrimType::write_repr(std::ostream &stream) const
{
    using types::PrimKind;
    switch (this->kind) {
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
