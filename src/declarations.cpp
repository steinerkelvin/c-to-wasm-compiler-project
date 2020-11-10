#include "declarations.hpp"

#include <cassert>
#include <iostream>

#include "symtable.hpp"

using namespace decl;

void decl::declare(const DeclarationSpecs& pspecs, const InitDeclarators& decls)
{
    bool is_typedef = false;
    for (const auto& pspec : pspecs) {
        StorageClassSpec* stor_spec = dynamic_cast<StorageClassSpec*>(pspec);
        if (stor_spec != NULL) {
            switch (stor_spec->kind) {
            case StorageClassSpec::Kind::TYPEDEF:
                is_typedef = true;
                break;
            default:
                assert(0);
            }
        }
    }

    for (auto const& decl : decls) {
        if (is_typedef) {
            if (lookup_type(*decl).has_value()) {
                assert(0);
            }
            insert_type(decl->c_str());
        } else {
            if (lookup_name(*decl).has_value()) {
                assert(0);
            }
            insert_name(decl->c_str());
        }
    }

}
