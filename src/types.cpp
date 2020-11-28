#include "types.hpp"

namespace types {

Type* Pointer::derreference() const
{
    assert(this->n >= 1);
    if (this->n == 1) {
        return this->get_base();
    }
    // n >= 2
    auto new_type = new Pointer(*this);
    new_type->n--;
    return new_type;
}
Pointer* Pointer::add_indiretion(Type* type, size_t n)
{
    if (auto type_pt = dynamic_cast<Pointer*>(type)) {
        auto new_type = new Pointer(*type_pt);
        new_type->n += n;
        return new_type;
    }
    return new Pointer(type, n);
}

std::optional<Pointer*> Pointer::to_pointer_implicit() { return this; }
std::optional<Pointer*> Vector::to_pointer_implicit()
{
    // TODO
    abort();
    return new Pointer(this);
}
std::optional<Pointer*> Function::to_pointer_implicit()
{
    return new Pointer(this);
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
               this->num_elem == casted->num_elem;
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

std::ostream& types::PrimType::write_repr(std::ostream& stream) const
{
    switch (this->kind) {
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

std::ostream& Pointer::write_repr(std::ostream& stream) const
{
    // stream << "(";
    for (size_t i = 0; i < this->n; i++) {
        stream << "*";
    }
    // stream << ")";
    this->get_base()->write_repr(stream);
    return stream;
}

std::ostream& Vector::write_repr(std::ostream& stream) const
{
    stream << "[" << this->num_elem << "]";
    this->get_base()->write_repr(stream);
    return stream;
}

std::ostream& Function::write_repr(std::ostream& stream) const
{
    stream << "(";

    Once first;
    for (auto [name, type] : parameters) {
        if (!first) {
            stream << ", ";
        }
        type->write_repr(stream);
        if (name) {
            stream << " " << *name;
        }
    }

    stream << ")";
    this->get_base()->write_repr(stream);
    return stream;
}

} // namespace types

std::ostream& operator<<(std::ostream& stream, const types::Type& type)
{
    return type.write_repr(stream);
}
