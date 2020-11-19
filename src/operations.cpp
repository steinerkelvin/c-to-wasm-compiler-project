#include "operations.hpp"
#include <cstdio>
#include <iostream>

namespace ops {
using namespace types;
using types::PrimKind;
using types::PrimType;
using types::Type;

static void type_error(const char* op, const Type* t1, const Type* t2)
{
    std::cerr << "SEMANTIC ERROR (0): ";
    std::cerr << "incompatible types for operator \'" << op << "\', ";
    std::cerr << "LHS is \'" << (*t1) << "\' and RHS is \'" << (*t2) << "\'."
              << std::endl;
    exit(EXIT_FAILURE);
}

static void type_error_unary(const char* op, const Type* t1)
{
    std::cerr << "SEMANTIC ERROR (0): ";
    std::cerr << "incompatible type for operator \'" << op << "\', ";
    std::cerr << "operand is \'" << (*t1) << "\'." << std::endl;
    exit(EXIT_FAILURE);
}

//
// Olf stuff
// TODO complete refactor
//

static const PrimKind arith[4][4] = {
    /* void	*/ {VOID, VOID, VOID, VOID},
    /* char	*/ {VOID, INTEGER, INTEGER, VOID},
    /* int 	*/ {VOID, INTEGER, INTEGER, REAL},
    /* real	*/ {VOID, VOID, REAL, REAL}};

Type* unify_arith(const Type* l, const Type* r, const char* op)
{
    const PrimType* pl = dynamic_cast<const PrimType*>(l);
    const PrimType* pr = dynamic_cast<const PrimType*>(r);
    if (!pl || !pr) {
        type_error(op, l, r);
    }
    PrimKind kl = pl->kind;
    PrimKind kr = pr->kind;
    PrimKind result = arith[kl][kr];
    if (result == PrimKind::VOID)
        type_error(op, l, r);
    return new PrimType(result);
}

static const PrimKind comp[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, INTEGER},
    /* int  */ {VOID, INTEGER, INTEGER, INTEGER},
    /* real */ {VOID, INTEGER, INTEGER, INTEGER}};

Type* unify_comp(const Type* l, const Type* r, const char* op)
{
    const PrimType* pl = dynamic_cast<const PrimType*>(l);
    const PrimType* pr = dynamic_cast<const PrimType*>(r);
    if (!pl || !pr) {
        type_error(op, l, r);
    }
    PrimKind kl = pl->kind;
    PrimKind kr = pr->kind;
    PrimKind result = comp[kl][kr];
    if (result == PrimKind::VOID)
        type_error(op, l, r);
    return new PrimType(result);
}

static const PrimKind bitwise[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, VOID},
    /* int  */ {VOID, INTEGER, INTEGER, VOID},
    /* real */ {VOID, VOID, VOID, VOID}};

Type* unify_bitwise(const Type* l, const Type* r, const char* op)
{
    const PrimType* pl = dynamic_cast<const PrimType*>(l);
    const PrimType* pr = dynamic_cast<const PrimType*>(r);
    if (!pl || !pr) {
        type_error(op, l, r);
    }
    PrimKind kl = pl->kind;
    PrimKind kr = pr->kind;
    PrimKind result = bitwise[kl][kr];
    if (result == PrimKind::VOID)
        type_error(op, l, r);
    return new PrimType(result);
}

static const PrimKind unary[4] = {VOID, INTEGER, INTEGER, INTEGER};

Type* unary_verify(const Type* u, const char* op)
{
    const PrimType* prim = dynamic_cast<const PrimType*>(u);
    if (!prim) {
        type_error_unary(op, u);
    }
    PrimKind k = prim->kind;
    PrimKind result = unary[k];
    if (result == PrimKind::VOID) {
        type_error_unary(op, u);
    }
    return new PrimType(result);
}

static const PrimKind btnot[4] = {VOID, INTEGER, INTEGER, VOID};

Type* btnot_verify(const Type* u, const char* op)
{
    const PrimType* prim = dynamic_cast<const PrimType*>(u);
    if (!prim) {
        type_error_unary(op, u);
    }
    PrimKind k = prim->kind;
    PrimKind result = btnot[k];
    if (result == PrimKind::VOID)
        type_error_unary(op, u);
    return new PrimType(result);
}

namespace prim_matrix {

    using ast::bdC2I;
    using ast::bdC2R;
    using ast::bdI2C;
    using ast::bdI2R;
    using ast::bdR2C;
    using ast::bdR2I;

    using ast::CoersionBuilder;
    using CoersionBuilderPair =
        std::pair<ast::CoersionBuilder, ast::CoersionBuilder>;

    using ResultMatrix = PrimKind[4][4];
    using CoersionPairMatrix = const std::optional<CoersionBuilderPair>[4][4];
    using ResultAndCoersionMatrixes =
        const std::pair<const ResultMatrix*, const CoersionPairMatrix*>;
    using p = CoersionBuilderPair;

    const auto& error = std::nullopt;
    const CoersionBuilder rsame = [](Expr* x) { return x; };

    // clang-format off

    const std::optional<CoersionBuilder> assignment[4][4] = {
        /*           void   char    int   real */
        /* void */ {error, error, error, error},
        /* char */ {error, rsame, bdI2C, bdR2C},
        /* int  */ {error, bdC2I, rsame, bdR2I},
        /* real */ {error, bdC2R, bdI2R, rsame}
    };

    const PrimKind arith_result[4][4] = {
        /*          void    char             int              real           */
        /* void	*/ {VOID,   VOID           , VOID           , VOID            },
        /* char	*/ {VOID,   INTEGER        , INTEGER        , VOID            },
        /* int 	*/ {VOID,   INTEGER        , INTEGER        , REAL            },
        /* real	*/ {VOID,   VOID           , REAL           , REAL            }
    };
    const CoersionPairMatrix arith_coersion = {
        /*          void    char             int              real           */
        /* void	*/ {error,  error          , error          , error           },
        /* char	*/ {error,  p{rsame, rsame}, p{bdC2I, rsame}, p{bdC2R, rsame} },
        /* int 	*/ {error,  p{rsame, bdC2I}, p{rsame, rsame}, p{bdI2R, rsame} },
        /* real	*/ {error,  p{rsame, bdC2R}, p{rsame, bdI2R}, p{rsame, rsame} }
    };
    ResultAndCoersionMatrixes arith = {&arith_result, &arith_coersion};

    // clang-format on
} // namespace prim_matrix

using prim_matrix::CoersionBuilderPair;
using prim_matrix::ResultAndCoersionMatrixes;

static void assignment_type_error(const Type* target, const Type* source)
{
    std::cerr << "SEMANTIC ERROR (0): ";
    std::cerr << "type \'" << (*source) << "\' cannot be assigned to type \'"
              << (*target) << "\'." << std::endl;
    exit(EXIT_FAILURE);
}

static Expr* check_prim_assignment(PrimType* target_type, Expr* source_node)
{
    PrimType* source_type = dynamic_cast<PrimType*>(source_node->get_type());
    assert(source_type);
    auto k1 = target_type->kind;
    auto k2 = source_type->kind;
    auto func = prim_matrix::assignment[k1][k2];
    if (!func) {
        assignment_type_error(target_type, source_type);
    }
    Expr* new_node = (*func)(source_node);
    return new_node;
}

Expr* check_assignment(Type* target_type, Expr* source_node)
{
    assert(target_type);
    assert(source_node);
    using types::Function;
    using types::Pointer;
    using types::PrimType;
    using types::Type;
    using types::Vector;
    types::Type* source_type = source_node->get_type();

    // If target type is primitive type
    if (auto tg_prim = dynamic_cast<PrimType*>(target_type)) {
        if (auto src_prim = dynamic_cast<PrimType*>(source_type)) {
            auto new_node = check_prim_assignment(tg_prim, source_node);
            return new_node;
        }
        // If target type is pointer
    } else if (auto tg_pt = dynamic_cast<Pointer*>(target_type)) {
        // If source type can be converted to pointer implicitly
        if (auto src_pt_opt = source_type->to_pointer_implicit()) {
            auto src_pt = *src_pt_opt;
            if (!tg_pt->is_compatible_with(src_pt)) {
                assignment_type_error(target_type, source_type);
            }
            // If source type is pointer already
            if (dynamic_cast<Pointer*>(source_type)) {
                return source_node;
                // If source type is function, add coersion node
            } else if (dynamic_cast<Function*>(source_type)) {
                auto new_node = new ast::F2P(source_node);
                new_node->set_type(src_pt);
                return new_node;
                // If source type is vector, add coersion node
            } else if (dynamic_cast<Vector*>(source_type)) {
                auto new_node = new ast::F2P(source_node);
                new_node->set_type(src_pt);
                return new_node;
            }
        }
    }
    assignment_type_error(target_type, source_type);
    exit(EXIT_FAILURE);
}

Expr* unify_assignment(Expr* target, Expr* value)
{
    assert(target);
    auto target_type = target->get_type();
    auto new_value = check_assignment(target_type, value);
    auto result = new ast::Assign(target, new_value);
    result->set_type(target_type);
    return result;
}

std::pair<Type*, CoersionBuilderPair> unify_bin_prim(
    ResultAndCoersionMatrixes matrixes,
    PrimType* type1,
    PrimType* type2,
    const char* op)
{
    assert(type1);
    assert(type2);
    auto [result_matrix, coersion_matrix] = matrixes;
    PrimKind k1 = type1->kind;
    PrimKind k2 = type2->kind;
    auto result_kind = (*result_matrix)[k1][k2];
    if (result_kind == PrimKind::VOID) {
        type_error(op, type1, type2);
    }
    auto result_type = new PrimType(result_kind);
    auto builder_pair = (*coersion_matrix)[k1][k2];
    assert(builder_pair);
    return {result_type, *builder_pair};
}

Expr* unify_additive(
    Expr* node1, Expr* node2, ast::BinConstructor constr, const char* op)
{
    assert(node1);
    assert(node2);
    auto type1 = node1->get_type();
    auto type2 = node2->get_type();

    if (auto left_type_ptopt = type1->to_pointer_implicit()) {
        // TODO pointer arithmetic?
        abort();
    // Handles primitive type operands
    } else if (auto type1_prim = dynamic_cast<PrimType*>(type1)) {
        if (auto type2_prim = dynamic_cast<PrimType*>(type2)) {
            auto [result_type, coersion] =
                unify_bin_prim(prim_matrix::arith, type1_prim, type2_prim, op);
            auto [c1, c2] = coersion;
            auto new1 = c1(node1);
            auto new2 = c2(node2);
            auto new_node = constr(new1, new2);
            return new_node;
        }
    }
    type_error(op, type1, type2);
}

Expr* address_of(Expr* value)
{
    Type* type = value->get_type();
    // TODO check rvalue

    types::Pointer* new_type = types::Pointer::add_indiretion(type);
    Expr* new_node = new ast::AddressOf(value);
    new_node->set_type(new_type);
    return new_node;
}

Expr* derreference(Expr* value)
{
    Type* type = value->get_type();
    assert(type);

    // TODO refactor into types
    types::Pointer* type_pointer = dynamic_cast<types::Pointer*>(type);
    if (!type_pointer) {
        std::cerr << "SEMANTIC ERROR (0): ";
        std::cerr << "derreferenced value must be of pointer type, ";
        std::cerr << "got \'" << *type << "\' instead." << std::endl;
        exit(1);
    }

    Type* new_type = type_pointer->derreference();
    Expr* new_node = new ast::Derreference(value);
    new_node->set_type(new_type);
    return new_node;
}

Expr* index_access(Expr* value, Expr* index)
{
    using ast::IntegerValue;
    using types::Vector;

    const types::Type* index_type = index->get_type();
    const types::PrimType* index_type_prim =
        dynamic_cast<const types::PrimType*>(index_type);
    // TODO aceitar qualquer expressão com tipo inteiro
    if (!index_type_prim || !(index_type_prim->kind == PrimKind::INTEGER)) {
        std::cerr << "SEMANTIC ERROR (0): ";
        std::cerr << "index must heave integer type, ";
        std::cerr << "got " << *index << " instead." << std::endl;
        exit(1);
    }

    const Type* value_type = value->get_type();
    assert(value_type);
    const Vector* value_type_vector = dynamic_cast<const Vector*>(value_type);
    // TODO pointer
    if (!value_type_vector) {
        std::cerr << "SEMANTIC ERROR (0): ";
        std::cerr << "accessed value must be of array type, ";
        std::cerr << "got \'" << *value_type << "\' instead." << std::endl;
        exit(1);
    }

    Type* base_type = value_type_vector->get_base();
    ast::IndexAccess* new_node = new ast::IndexAccess(index, value);
    new_node->set_type(base_type);
    return new_node;
}

Expr* function_call(Expr* value, ast::Exprs* args_node)
{
    assert(value);
    if (!args_node) {
        args_node = new ast::Exprs();
    }
    auto& args = args_node->children;

    Type* value_type = value->get_type();

    types::Pointer* value_type_pointer =
        dynamic_cast<types::Pointer*>(value_type);
    if (value_type_pointer && value_type_pointer->n == 1) {
        // TODO derreference instead
        value_type = value_type_pointer->get_base();
    }

    types::Function* value_type_func =
        dynamic_cast<types::Function*>(value_type);
    if (!value_type_func) {
        std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
        std::cerr << "called value must be of function type, ";
        std::cerr << "got \'" << (*value_type) << "\' instead." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto type_parameters = value_type_func->parameters;
    if (type_parameters.size() != args.size()) {
        std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
        std::cerr << "wrong number of arguments, "
                  << "expected " << type_parameters.size() << ", "
                  << "got " << args.size() << " instead." << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < type_parameters.size(); ++i) {
        auto tpparam = type_parameters[i];
        auto [n1, t1] = tpparam;
        args[i] = check_assignment(t1, args[i]);
    }

    Type* base_type = value_type_func->get_base();
    ast::Call* new_node = new ast::Call(value, args_node);
    new_node->set_type(base_type);
    return new_node;
}

} // namespace ops
