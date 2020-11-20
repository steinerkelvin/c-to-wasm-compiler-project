#include "declarations.hpp"

#include <cassert>
#include <iostream>
#include <variant>

#include "symtable.hpp"

namespace decl {

static void error_simple_type_spec(const char* const kind_name)
{
    fprintf(stderr, "SEMANTIC ERROR (%d): ", 0); // TODO
    fprintf(stderr, "%s type specifier not valid here\n", kind_name);
    exit(1);
}

static void handle_simple_type_spec(
    std::optional<types::PrimType>& result_type, const SimpleTypeSpec* spec)
{
    using types::PrimType;
    assert(spec);

    switch (spec->kind) {
        case SimpleTypeSpec::VOID:
            if (result_type && result_type->kind != PrimType::VOID) {
                error_simple_type_spec("void");
            } else {
                result_type = PrimType(PrimType::VOID);
            }
            break;
        case SimpleTypeSpec::CHAR:
            if (result_type && result_type->kind != PrimType::CHAR) {
                error_simple_type_spec("char");
                exit(1);
            } else {
                result_type = PrimType(PrimType::CHAR);
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
                result_type = PrimType(PrimType::INTEGER);
            }
            break;
        case SimpleTypeSpec::FLOAT:
        case SimpleTypeSpec::DOUBLE:
            if (result_type && result_type->kind != PrimType::REAL) {
                error_simple_type_spec("floating");
                exit(1);
            } else {
                result_type = PrimType(PrimType::REAL);
            }
            break;
        default:
            abort();
    }
}

types::PrimType make_type(const TypeQualOrTypeSpecList& pspecs)
{
    using types::PrimType;

    std::optional<PrimType> result_type;

    for (const auto& pspec : pspecs) {
        if (const auto it = std::get_if<TypeSpecifier*>(&pspec)) {
            const TypeSpecifier* ptpspec = *it;
            assert(ptpspec);
            // Trata especificador simples de tipo
            const auto simptpspec =
                dynamic_cast<const SimpleTypeSpec*>(ptpspec);
            if (simptpspec) {
                // TODO usar um membro virtual em TypeSpecifier
                handle_simple_type_spec(result_type, simptpspec);
            }
        }
    }
    assert(result_type);
    return *result_type;
}

ContainerTypeBuilder ContainerTypeBuilder::pointer(size_t n)
{
    auto func = [n](types::Type* type) -> types::ContainerType* {
        return types::Pointer::add_indiretion(type, n);
    };
    return ContainerTypeBuilder(func);
}

ContainerTypeBuilder ContainerTypeBuilder::vector(size_t size)
{
    auto func = [size](types::Type* base) -> ContainerType* {
        return new types::Vector(base, size);
    };
    return ContainerTypeBuilder(func);
}

ContainerTypeBuilder ContainerTypeBuilder::function(FuncParameters parameters)
{
    auto func = [parameters](types::Type* rettype) -> ContainerType* {
        return new types::Function(rettype, parameters);
    };
    return ContainerTypeBuilder(func);
}

ContainerTypeBuilder pointer_type_builder(size_t n)
{
    return ContainerTypeBuilder::pointer(n);
}

ContainerTypeBuilder vector_type_builder(ast::Expr* size_expr, pos::Pos posi)
{
    assert(size_expr);
    using ast::IntegerValue;
    const IntegerValue* size_int_node =
        dynamic_cast<const IntegerValue*>(size_expr);
    if (!size_int_node) {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "vector size must be an integer literal value, ";
        std::cerr << "got \'" << (*size_expr) << "\' instead." << std::endl;
        exit(1);
    }
    size_t size = size_int_node->get_value();
    return ContainerTypeBuilder::vector(size);
}

ContainerTypeBuilder function_type_builder(AbstractParameterDecls* param_decls)
{
    using ast::IntegerValue;
    using types::Type;

    std::vector<std::pair<std::optional<std::string>, Type*>> parameters;

    // Caso não haja parâmetros declarados
    if (!param_decls) {
        return ContainerTypeBuilder::function(parameters);
    }

    // Varre os declaradores de parâmetro
    for (auto& param_decl : *param_decls) {
        const DeclarationSpecs* decl_specs = param_decl->first;
        assert(decl_specs);

        // Lista onde serão armazenados os especificadores de tipo que estão
        // entre os especificadores de declaração do parâmetro
        TypeQualOrTypeSpecList typedecl_specs;

        for (const auto& pspec : *decl_specs) {
            // Trata especificadores de storage
            StorageClassSpec* stor_spec =
                dynamic_cast<StorageClassSpec*>(pspec);
            if (stor_spec != NULL) {
                switch (stor_spec->kind) {
                    case StorageClassSpec::Kind::REGISTER:
                        std::cerr << "NOT IMPLEMENTED ERROR (" << 0 << "): ";
                        std::cerr << "'register' storage-class specifier is "
                                     "not implemented."
                                  << std::endl;
                        exit(1);
                        break;
                    default:
                        std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
                        std::cerr
                            << "The only storage-class specifier that shall "
                               "occur in a parameter declaration is 'register'."
                            << std::endl;
                        exit(1);
                }
            }
            // Trata especificadores de tipo, armazenando na lista acima
            TypeDeclSpec* typedecl_spec = dynamic_cast<TypeDeclSpec*>(pspec);
            if (typedecl_spec != NULL) {
                TypeQualOrTypeSpecPointer it = typedecl_spec->get();
                typedecl_specs.push_back(it);
            }
        }

        // Consome os especificadores de tipo para construir o tipo base
        types::PrimType base_type = make_type(typedecl_specs);

        AbstractDeclarator* abs_declarator = *param_decl->second;
        assert(abs_declarator);

        // Constrói o tipo final com todos os construtores que foram
        // empilhados na redução do declarador
        types::Type* type = new types::PrimType(base_type);

        while (!abs_declarator->builders.empty()) {
            auto builder = *(abs_declarator->builders.end() - 1);
            abs_declarator->builders.pop_back();
            type = builder.build(type);
        }

        // Trata o nome do parâmetro
        std::optional<std::string> name;
        Declarator* declarator = dynamic_cast<Declarator*>(abs_declarator);
        if (declarator) {
            name = declarator->name;
        }

        parameters.push_back({name, type});
    }

    return ContainerTypeBuilder::function(parameters);
}

void declare(const DeclarationSpecs& pspecs, const InitDeclarators& decls)
{
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

    for (auto& decl : decls) {
        types::Type* type = new types::PrimType(base_type);

        // Constrói o tipo final com todos os construtores que foram empilhados
        // na redução do declarador
        while (!decl->builders.empty()) {
            auto builder = *(decl->builders.end() - 1);
            decl->builders.pop_back();
            type = builder.build(type);
        }

        const std::string name = decl->name;
        if (is_typedef) {
            if (sbtb::lookup_type(name, true)) {
                std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
                std::cerr << "\'" << name << "\' already declared in this scope"
                          << std::endl;
                assert(0); // TODO
            }
            sbtb::insert_typename(name.c_str(), type);
        } else {
            if (sbtb::lookup_name(name, true)) {
                std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
                std::cerr << "\'" << name << "\' already declared in this scope"
                          << std::endl;
                assert(0); // TODO
            }
            sbtb::insert_name(name.c_str(), type);
        }
    }
}

// TODO nome mais legível para esse tipo de retorno
std::pair<sbtb::NameRef, ScopeId>*
declare_function(const DeclarationSpecs* specs, Declarator* declarator)
{
    assert(specs);
    assert(declarator);

    TypeQualOrTypeSpecList typedecl_specs;

    for (const auto& pspec : *specs) {
        StorageClassSpec* stor_spec = dynamic_cast<StorageClassSpec*>(pspec);
        if (stor_spec != NULL) {
            switch (stor_spec->kind) {
                case StorageClassSpec::Kind::STATIC:
                    // TODO ?
                    break;
                default:
                    std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
                    std::cerr << "only 'static' storage class is allowed on "
                                 "function definitions."
                              << std::endl;
                    exit(1);
            }
        }
        TypeDeclSpec* typedecl_spec = dynamic_cast<TypeDeclSpec*>(pspec);
        if (typedecl_spec != NULL) {
            TypeQualOrTypeSpecPointer it = typedecl_spec->get();
            typedecl_specs.push_back(it);
        }
    }

    types::Type* type = new types::PrimType(make_type(typedecl_specs));

    while (!declarator->builders.empty()) {
        auto builder = *(declarator->builders.end() - 1);
        declarator->builders.pop_back();
        type = builder.build(type);
    }

    types::Function* type_func = dynamic_cast<types::Function*>(type);
    assert(type_func);

    const std::string name = declarator->name;
    if (sbtb::lookup_name(name, true)) {
        // TODO allow defining already declared function
        std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
        std::cerr << "\'" << name << "\' already declared in this scope."
                  << std::endl;
        exit(1);
    }
    sbtb::NameRef ref = sbtb::insert_name(name, type);

    std::vector<std::pair<std::string, types::Type*>> concrete_parameters;
    for (auto [param_name, param_type] : type_func->parameters) {
        if (!param_name) {
            std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
            std::cerr << "missing function parameter identifier." << std::endl;
            exit(1);
        }
        concrete_parameters.push_back({*param_name, param_type});
    }

    ScopeId scope_id = sbtb::open_scope();

    // Declare the functions parameters in the new scope
    for (auto [param_name, param_type] : concrete_parameters) {
        if (sbtb::lookup_name(param_name, true)) {
            // TODO allow defining already declared function
            std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
            std::cerr << "\'" << param_name << "\' already declared."
                      << std::endl;
            exit(1);
        }
        sbtb::insert_name(param_name, param_type);
    }

    return new std::pair<sbtb::NameRef, ScopeId>{ref, scope_id};
}

}; // namespace decl
