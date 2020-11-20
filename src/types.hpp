#if !defined(TYPES_H)
#define TYPES_H

/**
 * @file types.hpp
 */

#include <cassert>
#include <cstdlib>
#include <functional>
#include <ostream>
#include <vector>

#include "util.hpp"

namespace types {

// Forward declaration
struct Pointer;

/// Abstract class representing any type
struct Type {
    /// Converts to pointer type when implicitly conversion is possible
    virtual std::optional<Pointer*> to_pointer_implicit() { return {}; };
    /// Checks if this type is compatible with another one
    virtual bool is_compatible_with(const Type* other) = 0;
    /// Write s-expression representation of this subtree to `stream`
    virtual std::ostream& write_repr(std::ostream& stream) const = 0;
};

/// Represents a primitive type
struct PrimType : Type {
    enum PrimKind {
        VOID = 0,
        CHAR,
        INTEGER,
        REAL,
    };
    PrimKind kind;
    PrimType(const PrimType& type) = default;
    PrimType(PrimKind kind) : kind(kind){};
    PrimKind get_kind() const { return kind; }
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

/// A type that "contains" another type, e.g. pointer, vector, etc.
class ContainerType : public Type {
    Type* base_type;

  public:
    ContainerType(Type* base_type) : base_type(base_type){};
    /// Returns base type of this compound type
    Type* get_base() const { return this->base_type; }
};

/// Represents a pointer type.
struct Pointer : ContainerType {
    /// Number of indirections / "depth" of the pointer
    size_t n;

  public:
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
     * Builds a new type adding one indirection level.
     */
    static Pointer* add_indiretion(Type* type, size_t n = 1);

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

/// Represents a vector type.
struct Vector : ContainerType {
    /// Vector size
    size_t size;

    Vector(Type* base, size_t size) : ContainerType(base), size(size){};

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

/**
 * Represents a function type.
 * The Function class derives from ContainerType with its base type beeing the
 * return type of the function.
 */
struct Function : ContainerType {
    using Parameters =
        std::vector<std::pair<std::optional<std::string>, Type*>>;
    Parameters parameters;

    Function(Type* rettype, const Parameters& parameters)
        : ContainerType(rettype), parameters(parameters){};

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

}; // namespace types

std::ostream& operator<<(std::ostream& stream, const types::Type& type);

#endif /* TYPES_H */
