#if !defined(TYPES_H)
#define TYPES_H

#include <cstdlib>
#include <variant>

namespace types {

struct TypeQualifier {
    enum TypeQualifierKind {
        CONST,
        RESTRICT,
        VOLATILE,
    } kind;
};

struct TypeSpec {};
struct SimpleTypeSpec : TypeSpec {
    enum TypeSpecKind {
        VOID,
        CHAR,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        SIGNED,
        UNSIGNED,
    };
    SimpleTypeSpec(const TypeSpecKind kind) : kind(kind) {}

  protected:
    const TypeSpecKind kind;
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

using TypeQualifierOrSpec = std::variant<TypeQualifier, TypeSpec>;

}; // namespace types

enum class PrimitiveType {
    VOID,
    INT,
    CHAR,
    BOOL,
    FLOAT,
    DOUBLE,
};

typedef struct {
    PrimitiveType type;
} Type;

#endif /* TYPES_H */
