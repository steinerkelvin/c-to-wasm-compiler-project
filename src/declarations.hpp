#if !defined(DECLARATIONS_H)
#define DECLARATIONS_H

#include <string>
#include <vector>

#include "types.hpp"

namespace decl {

struct DeclarationSpec {};

struct StorageClassSpec : DeclarationSpec {
    enum StorageClassKind {
        TYPEDEF,
        EXTERN,
        STATIC,
        AUTO,
        REGISTER,
    };
    StorageClassSpec(const StorageClassKind kind) : kind(kind) {}

  protected:
    const StorageClassKind kind;
};

using types::TypeQualifierOrSpec;
struct TypeDeclSpec : DeclarationSpec {
    TypeDeclSpec(const TypeQualifierOrSpec value) { this->value = value; }

  protected:
    types::TypeQualifierOrSpec value;
};

class DeclarationSpecs : std::vector<DeclarationSpec*> {
  public:
    void add(DeclarationSpec* spec) { this->push_back(spec); }
};

struct InitDeclarators : std::vector<std::string*> {
    void add(std::string* init) { this->push_back(init); }
};

void declare(const DeclarationSpecs& specs, const InitDeclarators& decls);

}; // namespace decl

#endif /* DECLARATIONS_H */
 