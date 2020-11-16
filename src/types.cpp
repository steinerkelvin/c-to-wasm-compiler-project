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

std::optional<Pointer*> Pointer::to_pointer_implicit() { return this; }
std::optional<Pointer*> Vector::to_pointer_implicit()
{
    return new Pointer(this, 1);
}
std::optional<Pointer*> Function::to_pointer_implicit()
{
    return new Pointer(this, 1);
}

bool PrimType::is_compatible_with(const Type* other)
{
    if (auto casted = dynamic_cast<const PrimType*>(other)) {
        return this->kind == casted->kind;
    }
    return false;
}
bool Pointer::is_compatible_with(const Type* other)
{
    if (auto casted = dynamic_cast<const Pointer*>(other)) {
        // Check of compatibility of base type and number of indirections
        return this->get_base()->is_compatible_with(casted->get_base()) &&
               this->n == casted->n;
    }
    return false;
}
bool Vector::is_compatible_with(const Type* other)
{
    if (auto casted = dynamic_cast<const Vector*>(other)) {
        // Check of compatibility of base type and vector size
        return this->get_base()->is_compatible_with(casted->get_base()) &&
               this->size == casted->size;
    }
    return false;
}
bool Function::is_compatible_with(const Type* other)
{
    if (auto casted = dynamic_cast<const Function*>(other)) {
        // Check for return type compatibility
        if (!this->get_base()->is_compatible_with(casted->get_base())) {
            return false;
        }
        // Check for parameters compatibility
        auto& params1 = this->parameters;
        auto& params2 = casted->parameters;
        // Check for same number of paramenters
        if (params1.size() != params2.size()) {
            return false;
        }
        std::vector<bool> compatible_params;
        std::transform(
            params1.begin(), params1.end(), params2.begin(),
            std::back_inserter(compatible_params),
            [](const auto& p1, const auto& p2) {
                auto [n1, t1] = p1;
                auto [n2, t2] = p2;
                return t1->is_compatible_with(t2);
            });
        return std::all_of(
            compatible_params.begin(), compatible_params.end(),
            [](bool x) { return x; });
    }
    return false;
}

} // namespace types

std::ostream& operator<<(std::ostream& stream, const types::Type& type)
{
    return type.write_repr(stream);
}

std::ostream& types::PrimType::write_repr(std::ostream& stream) const
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
