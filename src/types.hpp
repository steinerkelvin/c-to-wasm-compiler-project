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
#include "positions.hpp"

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

// Representa tipos primitivos
struct PrimType : Type {
    PrimKind kind;
    PrimType(const PrimType& type) = default;
    PrimType(PrimKind kind) : kind(kind){};
    virtual bool is_compatible_with(const Type* other);
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

// Representa um tipo que "contém" um outro tipo base
// e.g. vetor, ponteiro, etc
struct ContainerType : Type {
    // Tipo de uma função que recebe um tipo base e constrói um ContainerType
    using Builder = std::function<ContainerType*(Type*)>;
    ContainerType(Type* base) : base(base){};
    // Retorna o tipo base desse tipo composto
    Type* get_base() const { return this->base; }

  protected:
    // Tipo base
    Type* base;
};

// Representa tipo de um vetor
struct Vector : ContainerType {
    // Tamanho do vetor
    size_t size;

    Vector(Type* base, size_t size) : ContainerType(base), size(size){};

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type *other);

    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        stream << "[" << size << "]";
        this->base->write_repr(stream);
        return stream;
    }

    // Retorna um "construtor" para o tipo Vetor a partir de um tipo base
    static ContainerType::Builder builder(size_t size)
    {
        return [size](Type* base) -> ContainerType* {
            return new Vector(base, size);
        };
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
    static Pointer* add_indiretion(Type *type);

    virtual std::optional<Pointer*> to_pointer_implicit();
    virtual bool is_compatible_with(const Type *other);

    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        // stream << "(";
        for (size_t i = 0; i < this->n; i++) {
            stream << "*";
        }
        // stream << ")";
        this->base->write_repr(stream);
        return stream;
    }

    // Retorna um "construtor" para o tipo Vetor a partir de um tipo base
    static ContainerType::Builder builder(size_t size)
    {
        return [size](Type* base) -> ContainerType* {
            return new Pointer(base, size);
        };
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
    virtual bool is_compatible_with(const Type *other);

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
        this->base->write_repr(stream);
        return stream;
    }

    // Retorna um "construtor" para o tipo Function a partir de um tipo base
    static ContainerType::Builder builder(const Parameters& parameters)
    {
        return [parameters](Type* rettype) -> ContainerType* {
            return new Function(rettype, parameters);
        };
    }
};

//
// Estruturas para representar especificadores e qualificadores em declarações
//
// TODO mover para delclarations.hpp ?
//

struct TypeQualifier : pos::HasPosition {
    const enum TypeQualifierKind {
        CONST,
        RESTRICT,
        VOLATILE,
    } kind;
    TypeQualifier(TypeQualifierKind kind) : kind(kind) {};
};

struct TypeSpec : pos::HasPosition {
    virtual ~TypeSpec() = default;
};
struct SimpleTypeSpec : TypeSpec {
    const enum Kind {
        VOID,
        CHAR,
        SHORT,
        INT,
        LONG,
        SIGNED,
        UNSIGNED,
        FLOAT,
        DOUBLE,
    } kind;
    SimpleTypeSpec(const Kind kind) : kind(kind) {}
};

struct StructOrUnionSpec : TypeSpec {
    StructOrUnionSpec(const bool is_un) : is_union_flag(is_un){};
    bool is_union() const { return this->is_union_flag; }

  protected:
    const bool is_union_flag;
};
struct EnumSpec : TypeSpec {};
struct TypedefName : TypeSpec {
    TypedefName(size_t ref) : ref(ref){};

  protected:
    const size_t ref;
};

using TypeQualOrTypeSpecPointer = std::variant<TypeQualifier*, TypeSpec*>;

struct TypeQualOrTypeSpecList : std::vector<TypeQualOrTypeSpecPointer> {
    void add(TypeQualOrTypeSpecPointer item) { this->push_back(item); }
};

}; // namespace types

std::ostream& operator<<(std::ostream& stream, const types::Type& type);

#endif /* TYPES_H */
