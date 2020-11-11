#if !defined(TYPES_H)
#define TYPES_H

#include <cstdlib>
#include <ostream>
#include <variant>
#include <vector>

namespace types {

enum PrimType {
    VOID,
    CHAR,
    INTEGER,
    REAL,
};

struct Type {
    PrimType kind;
};

const char * get_prim_text(PrimType kind);

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
