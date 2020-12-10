#include "operations.hpp"
#include <iostream>

#include "positions.hpp"

namespace ops {
using namespace types;
using types::PrimType;
using types::Type;

static void type_error_old(const char* op, const Type* t1, const Type* t2)
{
    assert(t1);
    assert(t2);
    std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
    std::cerr << "incompatible types for operator \'" << op << "\', ";
    std::cerr << "LHS is \'" << (*t1) << "\' and RHS is \'" << (*t2) << "\'."
              << std::endl;
    exit(EXIT_FAILURE);
}

static void
type_error(const char* op, pos::Pos posi, const Type* t1, const Type* t2)
{
    assert(t1);
    assert(t2);
    std::cerr << "SEMANTIC ERROR (" << posi << "): ";
    std::cerr << "incompatible types for operator \'" << op << "\', ";
    std::cerr << "LHS is \'" << (*t1) << "\' and RHS is \'" << (*t2) << "\'."
              << std::endl;
    exit(EXIT_FAILURE);
}

static void type_error_unary(const char* op, pos::Pos posi, const Type* type)
{
    std::cerr << "SEMANTIC ERROR (" << posi << "): ";
    std::cerr << "incompatible type for operator \'" << op << "\', ";
    std::cerr << "operand has type \'" << (*type) << "\'." << std::endl;
    exit(EXIT_FAILURE);
}

static void
type_error_assign(pos::Pos posi, const Type* target, const Type* source)
{
    std::cerr << "SEMANTIC ERROR (" << posi << "): ";
    std::cerr << "type \'" << (*source) << "\' cannot be assigned to type \'"
              << (*target) << "\'." << std::endl;
    exit(EXIT_FAILURE);
}

static void type_error_bool(pos::Pos posi, const Type* type)
{
    std::cerr << "SEMANTIC ERROR (" << posi << "): ";
    std::cerr << "expression cannot be resolved to boolean." << std::endl;
    exit(EXIT_FAILURE);
}

//
// Olf stuff
// TODO complete refactor
//

// clang-format off
static const PrimType::PrimKind bitwise[4][4] = {
    /* void */ {PrimType::VOID, PrimType::VOID,    PrimType::VOID,    PrimType::VOID},
    /* char */ {PrimType::VOID, PrimType::INTEGER, PrimType::INTEGER, PrimType::VOID},
    /* int  */ {PrimType::VOID, PrimType::INTEGER, PrimType::INTEGER, PrimType::VOID},
    /* real */ {PrimType::VOID, PrimType::VOID,    PrimType::VOID,    PrimType::VOID} };
// clang-format on

Type* unify_bitwise_old(const Type* l, const Type* r, const char* op)
{
    const PrimType* pl = dynamic_cast<const PrimType*>(l);
    const PrimType* pr = dynamic_cast<const PrimType*>(r);
    if (!pl || !pr) {
        type_error_old(op, l, r);
    }
    PrimType::PrimKind kl = pl->kind;
    PrimType::PrimKind kr = pr->kind;
    PrimType::PrimKind result = bitwise[kl][kr];
    if (result == PrimType::VOID)
        type_error_old(op, l, r);
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

    using ResultMatrix = PrimType::PrimKind[4][4];
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

    const auto VOID = PrimType::VOID;
    // const auto CHAR = PrimType::CHAR;
    const auto INTEGER = PrimType::INTEGER;
    const auto REAL = PrimType::REAL;

    const PrimType::PrimKind arith_result[4][4] = {
        /*          void    char             int              real           */
        /* void	*/ {VOID,   VOID           , VOID           , VOID            },
        /* char	*/ {VOID,   INTEGER        , INTEGER        , REAL            },
        /* int 	*/ {VOID,   INTEGER        , INTEGER        , REAL            },
        /* real	*/ {VOID,   REAL           , REAL           , REAL            }
    };
    const CoersionPairMatrix arith_coersion = {
        /*          void    char             int              real           */
        /* void	*/ {error,  error          , error          , error           },
        /* char	*/ {error,  p{rsame, rsame}, p{bdC2I, rsame}, p{bdC2R, rsame} },
        /* int 	*/ {error,  p{rsame, bdC2I}, p{rsame, rsame}, p{bdI2R, rsame} },
        /* real	*/ {error,  p{rsame, bdC2R}, p{rsame, bdI2R}, p{rsame, rsame} }
    };
    ResultAndCoersionMatrixes arith = {&arith_result, &arith_coersion};

    const PrimType::PrimKind comp_result[4][4] = {
        /*          void    char             int              real           */
        /* void	*/ {VOID,   VOID           , VOID           , VOID            },
        /* char	*/ {VOID,   INTEGER        , INTEGER        , INTEGER         },
        /* int 	*/ {VOID,   INTEGER        , INTEGER        , INTEGER         },
        /* real	*/ {VOID,   INTEGER        , INTEGER        , INTEGER         }
    };
    const CoersionPairMatrix comp_coersion = {
        /*          void    char             int              real           */
        /* void	*/ {error,  error          , error          , error           },
        /* char	*/ {error,  p{rsame, rsame}, p{bdC2I, rsame}, p{bdC2R, rsame} },
        /* int 	*/ {error,  p{rsame, bdC2I}, p{rsame, rsame}, p{bdI2R, rsame} },
        /* real	*/ {error,  p{rsame, bdC2R}, p{rsame, bdI2R}, p{rsame, rsame} }
    };
    ResultAndCoersionMatrixes comp = {&comp_result, &comp_coersion};

    // clang-format on
} // namespace prim_matrix

using prim_matrix::CoersionBuilderPair;
using prim_matrix::ResultAndCoersionMatrixes;

Expr* make_unary(
    Expr* node, ast::UnBuilder builder, const char* op, pos::Pos posi)
{
    assert(node);
    auto type = node->get_type();
    if (auto type_prim = dynamic_cast<PrimType*>(type)) {
        auto kind = type_prim->get_kind();
        if (!(kind == PrimType::VOID)) {
            return builder(node);
        }
    }
    type_error_unary(op, posi, type);
    abort();
}

Expr* make_btnot(
    Expr* node, ast::UnBuilder builder, const char* op, pos::Pos posi)
{
    assert(node);
    auto type = node->get_type();
    if (auto type_prim = dynamic_cast<PrimType*>(type)) {
        auto kind = type_prim->get_kind();
        if (kind == PrimType::CHAR || kind == PrimType::INTEGER) {
            return builder(node);
        }
    }
    type_error_unary(op, posi, type);
    abort();
}

static Expr*
check_prim_assignment(PrimType* target_type, Expr* source_node, pos::Pos posi)
{
    PrimType* source_type = dynamic_cast<PrimType*>(source_node->get_type());
    assert(source_type);
    auto k1 = target_type->kind;
    auto k2 = source_type->kind;
    auto func = prim_matrix::assignment[k1][k2];
    if (!func) {
        type_error_assign(posi, target_type, source_type);
    }
    Expr* new_node = (*func)(source_node);
    return new_node;
}

Expr* check_assign(Type* target_type, Expr* source_node, pos::Pos posi)
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
            auto new_node = check_prim_assignment(tg_prim, source_node, posi);
            return new_node;
        }
        // If target type is pointer
    } else if (auto tg_pt = dynamic_cast<Pointer*>(target_type)) {
        // If source type can be converted to pointer implicitly
        if (auto src_pt_opt = source_type->to_pointer_implicit()) {
            auto src_pt = *src_pt_opt;
            if (!tg_pt->is_compatible_with(src_pt)) {
                type_error_assign(posi, target_type, source_type);
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
                auto new_node = new ast::V2P(source_node);
                new_node->set_type(src_pt);
                return new_node;
            }
        }
    }
    type_error_assign(posi, target_type, source_type);
    exit(EXIT_FAILURE);
}

Expr* unify_assign(Expr* target, Expr* value, pos::Pos posi)
{
    assert(target);
    auto target_type = target->get_type();
    auto new_value = check_assign(target_type, value, posi);
    auto ltarget = dynamic_cast<ast::LExpr*>(target);
    if (ltarget && ltarget->is_lvalue()) {
        auto result = new ast::Assign(ltarget, new_value);
        result->set_type(target_type);
        return result;
    } else {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "non l-value on left side of assignment." << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::pair<Type*, CoersionBuilderPair> unify_bin_prim(
    ResultAndCoersionMatrixes matrixes,
    PrimType* type1,
    PrimType* type2,
    const char* op,
    pos::Pos posi)
{
    assert(type1);
    assert(type2);
    auto [result_matrix, coersion_matrix] = matrixes;
    PrimType::PrimKind k1 = type1->kind;
    PrimType::PrimKind k2 = type2->kind;
    auto result_kind = (*result_matrix)[k1][k2];
    if (result_kind == PrimType::VOID) {
        type_error(op, posi, type1, type2);
    }
    auto result_type = new PrimType(result_kind);
    auto builder_pair = (*coersion_matrix)[k1][k2];
    assert(builder_pair);
    return {result_type, *builder_pair};
}

Expr* unify_additive(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi)
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
            auto [result_type, coersion] = unify_bin_prim(
                prim_matrix::arith, type1_prim, type2_prim, op, posi);
            auto [c1, c2] = coersion;
            auto new1 = c1(node1);
            auto new2 = c2(node2);
            auto new_node = builder(new1, new2);
            new_node->set_type(result_type);
            return new_node;
        }
    }
    type_error(op, posi, type1, type2);
    abort();
}

Expr* unify_multi(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi)
{
    assert(node1);
    assert(node2);
    auto type1 = node1->get_type();
    auto type2 = node2->get_type();

    if (auto type1_prim = dynamic_cast<PrimType*>(type1)) {
        if (auto type2_prim = dynamic_cast<PrimType*>(type2)) {
            // Handle primitive type operands with matrix for arithmetic
            // on primitive types
            auto [result_type, coersion] = unify_bin_prim(
                prim_matrix::arith, type1_prim, type2_prim, op, posi);
            auto [c1, c2] = coersion;
            auto new1 = c1(node1);
            auto new2 = c2(node2);
            auto new_node = builder(new1, new2);
            new_node->set_type(result_type);
            return new_node;
        }
    }
    type_error(op, posi, type1, type2);
    abort();
}

Expr* unify_comp(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi)
{
    assert(node1);
    assert(node2);
    auto type1 = node1->get_type();
    auto type2 = node2->get_type();

    if (auto type1_ptopt = dynamic_cast<Pointer*>(type1)) {
        if (auto type2_ptopt = dynamic_cast<Pointer*>(type2)) {
            // TODO pointer comparison?
        }
        abort();
    } else if (auto type1_prim = dynamic_cast<PrimType*>(type1)) {
        if (auto type2_prim = dynamic_cast<PrimType*>(type2)) {
            // Handles primitive type operands
            auto [result_type, coersion] = unify_bin_prim(
                prim_matrix::comp, type1_prim, type2_prim, op, posi);
            auto [c1, c2] = coersion;
            auto new1 = c1(node1);
            auto new2 = c2(node2);
            auto new_node = builder(new1, new2);
            new_node->set_type(result_type);
            return new_node;
        }
    }
    type_error(op, posi, type1, type2);
    abort();
}

Expr* check_bool(Expr* node, pos::Pos posi)
{
    assert(node);
    auto type = node->get_type();
    if (auto type_prim = dynamic_cast<PrimType*>(type)) {
        auto kind = type_prim->get_kind();
        Expr* result_node = node;
        switch (kind) {
            case PrimType::CHAR:
                result_node = new ast::C2I(node);
                return result_node;
            case PrimType::INTEGER:
                return result_node;
            default:
                type_error_bool(posi, type);
        }
    }
    type_error_bool(posi, type);
    abort();
}

Expr* unify_logic(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi)
{
    assert(node1);
    assert(node2);
    auto pos1 = node1->get_pos();
    auto pos2 = node2->get_pos();
    assert(pos1);
    assert(pos2);
    // TODO alternative `check_bool` function to pass `op`
    node1 = check_bool(node1, *pos1);
    node2 = check_bool(node2, *pos2);
    Expr* new_node = builder(node1, node2);
    new_node->set_type(node1->get_type());
    return new_node;
}

Expr* address_of(Expr* value, pos::Pos posi)
{
    Type* type = value->get_type();
    auto lvalue = dynamic_cast<ast::LExpr*>(value);
    if (lvalue && lvalue->is_lvalue()) {
        types::Pointer* new_type = types::Pointer::add_indiretion(type);
        Expr* new_node = new ast::AddressOf(value);
        new_node->set_type(new_type);
        return new_node;
    }
    std::cerr << "SEMANTIC ERROR (" << posi << "): ";
    std::cerr << "trying to take address of non l-value expression."
              << std::endl;
    exit(EXIT_FAILURE);
}

Expr* derreference(Expr* value, pos::Pos posi)
{
    Type* type = value->get_type();
    assert(type);

    // TODO refactor into types
    types::Pointer* type_pointer = dynamic_cast<types::Pointer*>(type);
    if (!type_pointer) {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "derreferenced value must be of pointer type, ";
        std::cerr << "got \'" << *type << "\' instead." << std::endl;
        exit(1);
    }

    Type* new_type = type_pointer->derreference();
    Expr* new_node = new ast::Derreference(value);
    new_node->set_type(new_type);
    return new_node;
}

Expr* index_access(Expr* value, Expr* index, pos::Pos posi)
{
    using ast::IntegerValue;
    using types::Vector;

    const types::Type* index_type = index->get_type();

    const types::PrimType* index_type_prim =
        dynamic_cast<const types::PrimType*>(index_type);
    if (!index_type_prim || !(index_type_prim->kind == PrimType::CHAR ||
                              index_type_prim->kind == PrimType::INTEGER)) {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "index must heave integer type, ";
        std::cerr << "got \'" << *index_type << "\' instead." << std::endl;
        exit(1);
    }

    const Type* value_type = value->get_type();
    assert(value_type);
    const Vector* value_type_vector = dynamic_cast<const Vector*>(value_type);
    // TODO pointer
    if (!value_type_vector) {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "accessed value must be of array type, ";
        std::cerr << "got \'" << *value_type << "\' instead." << std::endl;
        exit(1);
    }

    Type* base_type = value_type_vector->get_base();
    ast::IndexAccess* new_node = new ast::IndexAccess(index, value);
    new_node->set_type(base_type);
    return new_node;
}

Expr* function_call(Expr* value, ast::Exprs* args_node, pos::Pos posi)
{
    assert(value);
    if (!args_node) {
        args_node = new ast::Exprs();
    }
    auto& args = args_node->get_children();

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
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "called value must be of function type, ";
        std::cerr << "got \'" << (*value_type) << "\' instead." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto type_parameters = value_type_func->parameters;
    if (type_parameters.size() != args.size()) {
        std::cerr << "SEMANTIC ERROR (" << posi << "): ";
        std::cerr << "wrong number of arguments, "
                  << "expected " << type_parameters.size() << ", "
                  << "got " << args.size() << " instead." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto new_args_node = new ast::Exprs();
    for (size_t i = 0; i < type_parameters.size(); ++i) {
        auto tpparam = type_parameters[i];
        auto [n1, t1] = tpparam;
        auto node = args[i];
        auto posi = node->get_pos();
        assert(posi);
        auto new_arg = check_assign(t1, args[i], *posi);
        new_args_node->add(new_arg);
    }
    delete args_node;

    Type* base_type = value_type_func->get_base();
    ast::Call* new_node = new ast::Call(value, new_args_node);
    new_node->set_type(base_type);
    return new_node;
}

ast::Statement* return_value(Expr* value_node, pos::Pos posi)
{
    assert(value_node);
    // TODO type check
    auto new_node = new ast::ReturnValue(value_node);
    return new_node;
}

} // namespace ops
