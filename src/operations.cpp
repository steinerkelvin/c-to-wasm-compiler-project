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

} // namespace ops
