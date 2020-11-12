#include "declarations.hpp"

#include <cassert>
#include <iostream>
#include <variant>

#include "symtable.hpp"

using namespace decl;

void error_simple_type_spec(const char* const kind_name)
{
    fprintf(stderr, "SEMANTIC ERROR (%d): ", 0); // TODO
    fprintf(stderr, "%s type specifier not valid here\n", kind_name);
    exit(1);
}

void handle_simple_type_spec(
    std::optional<types::PrimType>& result_type,
    const types::SimpleTypeSpec* spec)
{
    using types::PrimKind;
    using types::PrimType;
    using types::SimpleTypeSpec;
    assert(spec);

    switch (spec->kind) {
        case SimpleTypeSpec::VOID:
            if (result_type && result_type->kind != PrimKind::VOID) {
                error_simple_type_spec("void");
            } else {
                result_type = PrimType{PrimKind::VOID};
            }
            break;
        case SimpleTypeSpec::CHAR:
            if (result_type && result_type->kind != PrimKind::CHAR) {
                error_simple_type_spec("char");
                exit(1);
            } else {
                result_type = PrimType{PrimKind::CHAR};
            }
            break;
        case SimpleTypeSpec::SHORT:
        case SimpleTypeSpec::INT:
        case SimpleTypeSpec::LONG:
        case SimpleTypeSpec::SIGNED:
        case SimpleTypeSpec::UNSIGNED:
            if (result_type && result_type->kind != PrimKind::INTEGER) {
                error_simple_type_spec("integer");
                exit(1);
            } else {
                result_type = PrimType{PrimKind::INTEGER};
            }
            break;
        case SimpleTypeSpec::FLOAT:
        case SimpleTypeSpec::DOUBLE:
            if (result_type && result_type->kind != PrimKind::REAL) {
                error_simple_type_spec("floating");
                exit(1);
            } else {
                result_type = PrimType{PrimKind::REAL};
            }
            break;
        default:
            abort();
    }
}

types::PrimType make_type(const types::TypeQualOrTypeSpecList& pspecs)
{
    using types::PrimType;
    using types::SimpleTypeSpec;
    using types::TypeSpec;

    std::optional<PrimType> result_type;

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

    types::PrimType base_type = make_type(typedecl_specs);
    // std::cerr << base_type << std::endl;

    for (auto& decl : decls) {
        types::Type *type = new types::PrimType(base_type);

        for (auto vec_size_expr : decl->vec_sizes) {
            auto value_node = dynamic_cast<ast::IntegerValue*>(vec_size_expr);
            if (value_node) {
                size_t vec_size = value_node->get_value();
                type = new types::Vector(type, vec_size);
            } else {
                abort();
            }
        }

        const std::string name = decl->name;
        if (is_typedef) {
            if (sbtb::lookup_type(name)) {
                std::cerr << name << std::endl;
                assert(0); // TODO
            }
            sbtb::insert_typename(name.c_str(), type);
        } else {
            if (sbtb::lookup_name(name)) {
                std::cerr << name << std::endl;
                assert(0); // TODO
            }
            sbtb::insert_name(name.c_str(), type);
        }
    }
}
