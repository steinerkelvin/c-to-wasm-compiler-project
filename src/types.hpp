#if !defined(TYPES_H)
#define TYPES_H

/**
 * @file types.hpp
 */

#include <cassert>
#include <cstdlib>
#include <functional>
#include <ostream>
#include <variant>
#include <vector>

#include "util.hpp"

// Forward declaration
// TODO refatorar?
namespace ast {
struct Expr;
};
std::ostream& operator<<(std::ostream& stream, const ast::Expr& node);

namespace types {

struct Pointer;
struct Type {
    virtual ~Type() = default;
    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        return stream << "NOT_IMPLEMENTED";
    };
    // Converts to pointer type when implicitly conversion is possible
    virtual std::optional<Pointer*> to_pointer_implicit() { return {}; };
    // Checks with this type is compatible with another one
    virtual bool is_compatible_with(const Type* other) = 0;
};

// Enumeração de tipos primitivos
enum PrimKind {
    VOID, // Mover esse void para fora daqui? vai dar uma trabalheira :(
    CHAR,
    INTEGER,
    REAL,
};

// Retorna string que representa um tipo primitivo
const char* get_prim_text(PrimKind kind);

/// Represents primitive types
struct PrimType : Type {
    PrimKind kind;
    PrimType(const PrimType& type) = default;
    PrimType(PrimKind kind) : kind(kind){};
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

/// Represents a type that "contains" another type
// e.g. vetor, ponteiro, etc
class ContainerType : public Type {
    Type* base_type;

  public:
    ContainerType(Type* base_type) : base_type(base_type){};
    /// Returns base type of this compound type
    Type* get_base() const { return this->base_type; }
};

// Representa tipo de um vetor
struct Vector : ContainerType {
    // Tamanho do vetor
    size_t size;

    Vector(Type* base, size_t size) : ContainerType(base), size(size){};

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);

    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        stream << "[" << size << "]";
        this->get_base()->write_repr(stream);
        return stream;
    }
};

// Representa tipo de um ponteiro
struct Pointer : ContainerType {
    // Número de indireções / "profundidade" do ponteiro
    size_t n;

    Pointer(Type* base, size_t n = 1) : ContainerType(base), n(n)
    {
        assert(base);
        assert(n > 0);
    };

    /**
     * Builds a new type removing one indirection level from the pointer type,
     * returning the base type if the pointer has just one indirection level.
     */
    Type* derreference() const;
    /**
     * Builds a new type adding one indirection level to the pointer type.
     */
    static Pointer* add_indiretion(Type* type, size_t n = 1);

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);

    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        // stream << "(";
        for (size_t i = 0; i < this->n; i++) {
            stream << "*";
        }
        // stream << ")";
        this->get_base()->write_repr(stream);
        return stream;
    }
};

// O tipo de uma função deriva de ContainerType contendo um "tipo base"
// sendo seu valor de retorno
struct Function : ContainerType {
    using Parameters =
        std::vector<std::pair<std::optional<std::string>, Type*>>;

    Parameters parameters;
    Function(Type* rettype, const Parameters& parameters)
        : ContainerType(rettype), parameters(parameters){};

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);

    virtual std::ostream& write_repr(std::ostream& stream) const
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
};

}; // namespace types

std::ostream& operator<<(std::ostream& stream, const types::Type& type);

#endif /* TYPES_H */
