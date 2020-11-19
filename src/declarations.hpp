#if !defined(DECLARATIONS_H)
#define DECLARATIONS_H

/**
 * @file declarations.hpp
 */

#include <optional>
#include <string>
#include <vector>

#include "ast.hpp"
#include "types.hpp"
#include "positions.hpp"

namespace decl {
using types::ContainerType;

struct DeclarationSpec : pos::HasPosition {
    /** Força a classe a ser polimórfica */
    virtual ~DeclarationSpec() = default;
};

struct StorageClassSpec : DeclarationSpec {
    enum Kind {
        TYPEDEF,
        EXTERN,
        STATIC,
        AUTO,
        REGISTER,
    } kind;
    // const Kind ;
    StorageClassSpec(const Kind kind) : kind(kind) {}
};

using types::TypeQualOrTypeSpecPointer;
struct TypeDeclSpec : DeclarationSpec {
    TypeDeclSpec(const TypeQualOrTypeSpecPointer value) { this->value = value; }
    TypeQualOrTypeSpecPointer get() const { return this->value; }

  protected:
    types::TypeQualOrTypeSpecPointer value;
};

struct DeclarationSpecs : std::vector<DeclarationSpec*>, pos::HasPosition {
    void add(DeclarationSpec* spec) {
        this->push_back(spec);
        this->merge_pos_from(spec);
    }
};

struct AbstractDeclarator {
    std::vector<ContainerType::Builder> builders;

    virtual ~AbstractDeclarator() = default;
    void add(ContainerType::Builder builder) { builders.push_back(builder); }
};

struct Declarator : AbstractDeclarator {
    const std::string name;
    std::optional<ast::Expr*> init_expr;

    Declarator(const std::string& name) : name(name) {}
    void set_init(ast::Expr* init_expr) { this->init_expr = init_expr; };
};

struct InitDeclarators : std::vector<Declarator*> {
    void add(Declarator* init) { this->push_back(init); }
};

struct AbstractParameterDecl
    : std::pair<DeclarationSpecs*, std::optional<AbstractDeclarator*>> {

    using std::pair<DeclarationSpecs*, std::optional<AbstractDeclarator*>>::
        pair;

    static AbstractParameterDecl*
    from(DeclarationSpecs* specs, AbstractDeclarator* decl)
    {
        if (decl != NULL) {
            return new AbstractParameterDecl(specs, decl);
        } else {
            return new AbstractParameterDecl(specs, {});
        }
    }
};

struct AbstractParameterDecls : std::vector<AbstractParameterDecl*> {
    void add(AbstractParameterDecl* param_decl) { this->push_back(param_decl); }
};

// Retorna um construtor de tipo de vetor resolvendo a expressão `size_expr`
// para literal de inteiro, imprimindo erro e abortando em caso de falha
types::ContainerType::Builder vector_type_builder(ast::Expr* size_expr);

// Retorna um construtor de tipo de função a partir de uma lista de
// declaradores de parâmetros `param_decls`
types::ContainerType::Builder
function_type_builder(decl::AbstractParameterDecls* param_decls);

// Consome especificadores de declaração e declaradores, registrando
// adequadamente os nomes no último escopo aberto
void declare(const DeclarationSpecs& specs, const InitDeclarators& decls);

std::pair<sbtb::NameRef, ScopeId>*
declare_function(const DeclarationSpecs* specs, Declarator* declarator);

}; // namespace decl

#endif /* DECLARATIONS_H */
