#include "types.hpp"

using namespace types;

std::ostream& operator<<(std::ostream& stream, const types::Type& type)
{
    using types::PrimitiveType;
    switch (type.kind) {
        case PrimitiveType::VOID:
            stream << "void";
            break;
        case PrimitiveType::CHAR:
            stream << "char";
            break;
        case PrimitiveType::INTEGER:
            stream << "integer";
            break;
        case PrimitiveType::REAL:
            stream << "real";
            break;
        default:
            abort();
    }
    return stream;
}
