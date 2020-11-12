#if !defined(TYPES_H)
#define TYPES_H

#include <cassert>
#include <cstdlib>
#include <ostream>
#include <variant>
#include <vector>

namespace types {

struct Type {
    virtual std::ostream& write_repr(std::ostream& stream) const
    {
        return stream << "void";
    };
    virtual bool is_void() { return true; }
    virtual bool is_pointer() { return false; }
    virtual bool is_pointable() { return false; }
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
    virtual bool is_void() { return this->kind == VOID; }
    virtual std::ostream& write_repr(std::ostream& stream) const;
};

// Representa tipo de vetor
struct Vector : Type {
    // Tipo base, de um elemento do vetor
    Type* base;
    // Tamanho do vetor
    size_t size;

    Vector(Type* base, size_t size) : base(base), size(size){};
    virtual std::ostream& write_repr(std::ostream& stream) const {
        return this->base->write_repr(stream) << "[" << size << "]";
    }
};

// Representa tipo de um ponteiro
struct Pointer : Type {
    // Tipo base o elemento apontado pelo ponteiro
    Type* base;
    // Número de indireções / "profundidade" do ponteiro
    size_t n;

    Pointer(Type* base, size_t n) : base(base), n(n) { assert(n > 0); };
};

struct Function : Type {
    Type* return_type;
    std::vector<Type*> parameters;
    Function(Type* rettype, const std::vector<Type*>& parameters)
        : return_type(rettype), parameters(parameters){};
};

// Estruturas para representar especificadores e qualificadores em declarações

struct TypeQualifier {
    enum TypeQualifierKind {
        CONST,
        RESTRICT,
        VOLATILE,
    } kind;
};

struct TypeSpec {
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
