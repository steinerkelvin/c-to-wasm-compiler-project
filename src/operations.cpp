#include "operations.hpp"
#include <cstdio>

namespace ops {
using namespace types;

static void type_error(const char* op, const PrimType t1, const PrimType t2)
{
    printf(
        "SEMANTIC ERROR (0): incompatible types for operator '%s', LHS is '%s' "
        "and RHS is '%s'.\n",
        op,
        get_prim_text(t1),
        get_prim_text(t2));
    exit(EXIT_FAILURE);
}

static void type_error_unary(const char* op, const PrimType t1)
{
    printf(
        "SEMANTIC ERROR (0): incompatible type for operator '%s', operand is '%s'.\n",
        op,
        get_prim_text(t1));
    exit(EXIT_FAILURE);
}

static const PrimType arith[4][4] = {
    /* void	*/ {VOID, VOID, VOID, VOID},
    /* char	*/ {VOID, INTEGER, INTEGER, VOID},
    /* int 	*/ {VOID, INTEGER, INTEGER, REAL},
    /* real	*/ {VOID, VOID, REAL, REAL}};

PrimType unify_arith(PrimType l, PrimType r, const char* op)
{
    PrimType t = arith[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimType comp[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, INTEGER},
    /* int  */ {VOID, INTEGER, INTEGER, INTEGER},
    /* real */ {VOID, INTEGER, INTEGER, INTEGER}};

PrimType unify_comp(PrimType l, PrimType r, const char* op)
{
    PrimType t = comp[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimType bitwise[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, VOID},
    /* int  */ {VOID, INTEGER, INTEGER, VOID},
    /* real */ {VOID, VOID, VOID, VOID}
};

PrimType unify_bitwise(PrimType l, PrimType r, const char* op)
{
    PrimType t = bitwise[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimType unary[4] = {VOID, INTEGER, INTEGER, INTEGER};

PrimType unary_verify(PrimType u, const char* op)
{
    PrimType t = unary[u];
    if (t == VOID)
        type_error_unary(op, u);
    return t;
}

static const PrimType btnot[4] = {VOID, INTEGER, INTEGER, VOID};

PrimType btnot_verify(PrimType u, const char* op)
{
    PrimType t = btnot[u];
    if (t == VOID)
        type_error_unary(op, u);
    return t;
}

PrimType assign_verify(PrimType l, PrimType r, const char* op){
	if(l == VOID){
		printf("SEMANTIC ERROR (0): incompatible type for operator '%s', LHS is 'void'.\n",
        op);
    	exit(EXIT_FAILURE);
    }
    if(r == VOID){
    	printf("SEMANTIC ERROR (0): incompatible type for operator '%s', RHS is 'void'.\n",
        op);
    	exit(EXIT_FAILURE);
    }	
    else return l;
}

} // namespace ops
