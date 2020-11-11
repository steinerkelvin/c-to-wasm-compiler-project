#include "declarations.hpp"

#include <cassert>
#include <iostream>
#include <variant>

#include "symtable.hpp"

using namespace decl;

void error_simple_type_spec(const char *const kind_name)
{
    fprintf(stderr, "SEMANTIC ERROR (%d): ", 0); // TODO
    fprintf(stderr, "%s type specifier not valid here\n", kind_name);
    exit(1);
}

void handle_simple_type_spec(
    std::optional<types::Type>& result_type, const types::SimpleTypeSpec* spec)
{
    using types::PrimType;
    using types::SimpleTypeSpec;
    using types::Type;
    assert(spec);

    switch (spec->kind) {
        case SimpleTypeSpec::VOID:
            if (result_type && result_type->kind != PrimType::VOID) {
                error_simple_type_spec("void");
            } else {
                result_type = Type{PrimType::VOID};
            }
            break;
        case SimpleTypeSpec::CHAR:
            if (result_type && result_type->kind != PrimType::CHAR) {
                error_simple_type_spec("char");
                exit(1);
            } else {
                result_type = Type{PrimType::CHAR};
            }
            break;
        case SimpleTypeSpec::SHORT:
        case SimpleTypeSpec::INT:
        case SimpleTypeSpec::LONG:
        case SimpleTypeSpec::SIGNED:
        case SimpleTypeSpec::UNSIGNED:
            if (result_type && result_type->kind != PrimType::INTEGER) {
                error_simple_type_spec("integer");
                exit(1);
            } else {
                result_type = Type{PrimType::INTEGER};
            }
            break;
        case SimpleTypeSpec::FLOAT:
        case SimpleTypeSpec::DOUBLE:
            if (result_type && result_type->kind != PrimType::REAL) {
                error_simple_type_spec("floating");
                exit(1);
            } else {
                result_type = Type{PrimType::REAL};
            }
            break;
        default:
            abort();
    }
}

types::Type make_type(const types::TypeQualOrTypeSpecList& pspecs)
{
    using types::SimpleTypeSpec;
    using types::Type;
    using types::TypeSpec;

    std::optional<Type> result_type;

    for (const auto& pspec : pspecs) {
        if (const auto it = std::get_if<TypeSpec*>(&pspec)) {
            const TypeSpec* ptpspec = *it;
            assert(ptpspec);
            // Trata especificador simples de tipo
            const auto simptpspec =
                dynamic_cast<const SimpleTypeSpec*>(ptpspec);
            if (simptpspec) {
                // TODO usar um membro virtual em TypeSpec
                handle_simple_type_spec(result_type, simptpspec);
            }
        }
    }
    assert(result_type);
    return *result_type;
}

void decl::declare(const DeclarationSpecs& pspecs, const InitDeclarators& decls)
{
    using types::TypeQualOrTypeSpecList;
    using types::TypeQualOrTypeSpecPointer;
    TypeQualOrTypeSpecList typedecl_specs;

    bool is_typedef = false;
    for (const auto& pspec : pspecs) {
        StorageClassSpec* stor_spec = dynamic_cast<StorageClassSpec*>(pspec);
        if (stor_spec != NULL) {
            switch (stor_spec->kind) {
                case StorageClassSpec::Kind::TYPEDEF:
                    is_typedef = true;
                    break;
                default:
                    abort();
            }
        }
        TypeDeclSpec* typedecl_spec = dynamic_cast<TypeDeclSpec*>(pspec);
        if (typedecl_spec != NULL) {
            TypeQualOrTypeSpecPointer it = typedecl_spec->get();
            typedecl_specs.push_back(it);
        }
    }

    types::Type the_type = make_type(typedecl_specs);
    // std::cerr << the_type << std::endl;

    for (auto const& decl : decls) {
        if (is_typedef) {
            if (lookup_type(*decl).has_value()) {
                std::cerr << *decl << std::endl;
                assert(0);
            }
            insert_type(decl->c_str());
        } else {
            if (lookup_name(*decl).has_value()) {
                std::cerr << *decl << std::endl;
                assert(0);
            }
            insert_name(decl->c_str());
        }
    }
}
