#if !defined(DECLARATIONS_H)
#define DECLARATIONS_H

#include <string>
#include <vector>

#include "ast.hpp"
#include "types.hpp"

namespace decl {

struct DeclarationSpec {
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
    };
    StorageClassSpec(const Kind kind) : kind(kind) {}
    const Kind kind;
};

using types::TypeQualOrTypeSpecPointer;
struct TypeDeclSpec : DeclarationSpec {
    TypeDeclSpec(const TypeQualOrTypeSpecPointer value) { this->value = value; }
    TypeQualOrTypeSpecPointer get() const { return this->value; }

  protected:
    types::TypeQualOrTypeSpecPointer value;
};

struct DeclarationSpecs : std::vector<DeclarationSpec*> {
    void add(DeclarationSpec* spec) { this->push_back(spec); }
};

struct AbstractDeclarator {
    size_t pointer = 0;
    std::vector<ast::Expr*> vec_sizes;
    void set_pointer(size_t n) { this->pointer = n; }
    void add_vec(ast::Expr* size_exp) { this->vec_sizes.push_back(size_exp); }
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

void declare(const DeclarationSpecs& specs, const InitDeclarators& decls);

}; // namespace decl

#endif /* DECLARATIONS_H */