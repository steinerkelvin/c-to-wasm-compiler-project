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

Type* assign_verify(const Type* l, const Type* r, const char* op)
{
    const PrimType* pl = dynamic_cast<const PrimType*>(l);
    const PrimType* pr = dynamic_cast<const PrimType*>(r);
    if (!pl || !pr) {
        type_error(op, l, r);
    }
    PrimKind kl = pl->kind;
    PrimKind kr = pr->kind;
    if (kl == PrimKind::VOID) {
        std::cerr << "SEMANTIC ERROR (0): ";
        std::cerr << "incompatible type for operator '%s', ";
        std::cerr << "LHS is \'" << (*l) << "\'." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (kr == PrimKind::VOID) {
        std::cerr << "SEMANTIC ERROR (0): ";
        std::cerr << "incompatible type for operator '%s', ";
        std::cerr << "RHS is \'" << (*r) << "\'." << std::endl;
        exit(EXIT_FAILURE);
    }
    return new Type(*r);
}

Expr* address_of(Expr* value)
{
    Type* type = value->get_type();
    types::Pointer* type_pointer = dynamic_cast<types::Pointer*>(type);

    // TODO check rvalue

    types::Pointer* new_type;
    // TODO refactor into types
    if (type_pointer) {
        new_type = new types::Pointer(*type_pointer);
        new_type->n++;
    } else {
        new_type = new types::Pointer(type, 1);
    }

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
        std::cerr << "derreference value must be of pointer type, ";
        std::cerr << "got \'" << *type << "\' instead." << std::endl;
        exit(1);
    }

    Type* new_type;
    if (type_pointer->n >= 2) {
        types::Pointer* new_pointer_type = new types::Pointer(*type_pointer);
        new_pointer_type->n--;
        new_type = new_pointer_type;
    } else {
        new_type = type_pointer->get_base();
    }

    Expr* new_node = new ast::AddressOf(value);
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

Expr* function_call(Expr* value, ast::Exprs* parameters)
{
    assert(value);
    if (!parameters) {
        parameters = new ast::Exprs();
    }

    Type* value_type = value->get_type();

    types::Pointer* value_type_pointer =
        dynamic_cast<types::Pointer*>(value_type);
    if (value_type_pointer && value_type_pointer->n == 1) {
        value_type = value_type_pointer->get_base();
    }

    types::Function* value_type_func =
        dynamic_cast<types::Function*>(value_type);
    if (!value_type_func) {
        std::cerr << "SEMANTIC ERROR (" << 0 << "): ";
        std::cerr << "called value must be of function type, ";
        std::cerr << "got \'" << (*value_type) << "\' instead." << std::endl;
        exit(1);
    }

    Type* base_type = value_type_func->get_base();
    // TODO checar tipos dos parâmetros
    ast::Call* new_node = new ast::Call(value, parameters);
    new_node->set_type(base_type);
    return new_node;
}

} // namespace ops
