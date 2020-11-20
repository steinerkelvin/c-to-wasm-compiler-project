#if !defined(DECLARATIONS_H)
#define DECLARATIONS_H

/**
 * @file declarations.hpp
 */

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "ast.hpp"
#include "positions.hpp"
#include "types.hpp"

namespace decl {
using types::ContainerType;

//
// Structures to represent type specifiers and qualifiers
//

struct DeclarationSpec : pos::HasPosition {
    // Forces the class to be polymorphic
    virtual ~DeclarationSpec() = default;
};

struct TypeQualifier : DeclarationSpec {
    const enum TypeQualifierKind {
        CONST,
        RESTRICT,
        VOLATILE,
    } kind;
    TypeQualifier(TypeQualifierKind kind) : kind(kind){};
};

struct TypeSpecifier : DeclarationSpec {};

struct SimpleTypeSpec : TypeSpecifier {
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

struct StructOrUnionSpec : TypeSpecifier {
    StructOrUnionSpec(const bool is_un) : is_union_flag(is_un){};
    bool is_union() const { return this->is_union_flag; }

  protected:
    const bool is_union_flag;
};

struct EnumSpec : TypeSpecifier {};

struct TypedefName : TypeSpecifier {
    TypedefName(size_t ref) : ref(ref){};

  protected:
    const size_t ref;
};

using TypeQualOrTypeSpecPointer = std::variant<TypeQualifier*, TypeSpecifier*>;

struct TypeQualOrTypeSpecList : std::vector<TypeQualOrTypeSpecPointer>,
                                pos::HasPosition {
    void add(TypeQualOrTypeSpecPointer item)
    {
        this->push_back(item);
        std::visit([this](auto& item) { this->merge_pos_from(item); }, item);
    }
};

//
// Other declaration specifiers
//

struct StorageClassSpec : DeclarationSpec {
    const enum Kind {
        TYPEDEF,
        EXTERN,
        STATIC,
        AUTO,
        REGISTER,
    } kind;
    StorageClassSpec(const Kind kind) : kind(kind) {}
};

struct TypeDeclSpec : DeclarationSpec {
    TypeDeclSpec(const TypeQualOrTypeSpecPointer value) { this->value = value; }
    TypeQualOrTypeSpecPointer get() const { return this->value; }

  protected:
    TypeQualOrTypeSpecPointer value;
};

struct DeclarationSpecs : std::vector<DeclarationSpec*>, pos::HasPosition {
    void add(DeclarationSpec* spec)
    {
        this->push_back(spec);
        this->merge_pos_from(spec);
    }
};

//
// Type builders
//

/// A type builder can build a specific container given just the base type.
class ContainerTypeBuilder : pos::HasPosition {
    using FuncParameters = const types::Function::Parameters&;
    using Builder = std::function<types::ContainerType*(types::Type*)>;

    /// The actual function that builds the type.
    const Builder func;

    ContainerTypeBuilder(Builder func) : func(func){};

  public:
    /// Builds the container type.
    types::ContainerType* build(types::Type* base_type)
    {
        return this->func(base_type);
    }

    static ContainerTypeBuilder pointer(size_t n);
    static ContainerTypeBuilder vector(size_t size);
    static ContainerTypeBuilder function(FuncParameters parameters);
};

//
// Declarators
//

/**
 * Declarator with no name.
 * It felt good to be out of the rain.
 */
struct AbstractDeclarator {
    std::vector<ContainerTypeBuilder> builders;

    virtual ~AbstractDeclarator() = default;
    void add(ContainerTypeBuilder builder) { builders.push_back(builder); }
};

/// Declarator with a name, and maybe and initialization expression.
struct Declarator : AbstractDeclarator {
    const std::string name;
    std::optional<ast::Expr*> init_expr;

    Declarator(const std::string& name) : name(name) {}
    void set_init(ast::Expr* init_expr) { this->init_expr = init_expr; };
};

/// List of declarators
struct InitDeclarators : std::vector<Declarator*> {
    void add(Declarator* init) { this->push_back(init); }
};

/// Function parameter declaration.
struct ParameterDeclaration
    : std::pair<DeclarationSpecs*, std::optional<AbstractDeclarator*>> {

    using std::pair<DeclarationSpecs*, std::optional<AbstractDeclarator*>>::
        pair;

    static ParameterDeclaration*
    from(DeclarationSpecs* specs, AbstractDeclarator* decl)
    {
        if (decl != NULL) {
            return new ParameterDeclaration(specs, decl);
        } else {
            return new ParameterDeclaration(specs, {});
        }
    }
};

/// List of parameter declarations.
struct ParameterDeclarations : std::vector<ParameterDeclaration*> {
    void add(ParameterDeclaration* param_decl) { this->push_back(param_decl); }
};

//
// Functions for making type builders.
//

/**
 * Given the number of indirections `n`, returns a "builder" that is able of
 * constructing a pointer type with that number of indirections from a given
 * base type.
 */
ContainerTypeBuilder pointer_type_builder(size_t n);

/**
 * Given the expression node `size_expr` that will be resolved an integer size,
 * returns a "builder" that is able of constructing a vector type with that
 * size from a given base type.
 */
ContainerTypeBuilder vector_type_builder(ast::Expr* size_expr, pos::Pos posi);

/**
 * Given a list of parameter declarations, returns a "builder" that is able of
 * constructing a function type with the specified parameter types.
 */
ContainerTypeBuilder
function_type_builder(decl::ParameterDeclarations* param_decls);

//
// Declaration routines.
//

/**
 * Consumes a list of declarations specifiers and a lista of declarators,
 * inserting the name in the last opened scope acordingly.
 */
void declare(const DeclarationSpecs& specs, const InitDeclarators& decls);

/**
 * Declares a funcion and opens a new scope, inserting the function parameters
 * in this new scope.
 */
std::pair<sbtb::NameRef, ScopeId>*
declare_function(const DeclarationSpecs* specs, Declarator* declarator);

}; // namespace decl

#endif /* DECLARATIONS_H */
