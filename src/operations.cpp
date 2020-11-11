#include "operations.hpp"
#include <cstdio>

namespace ops {
using namespace types;

static void type_error(const char* op, const PrimKind t1, const PrimKind t2)
{
    printf(
        "SEMANTIC ERROR (0): incompatible types for operator '%s', LHS is '%s' "
        "and RHS is '%s'.\n",
        op,
        get_prim_text(t1),
        get_prim_text(t2));
    exit(EXIT_FAILURE);
}

static void type_error_unary(const char* op, const PrimKind t1)
{
    printf(
        "SEMANTIC ERROR (0): incompatible type for operator '%s', operand is '%s'.\n",
        op,
        get_prim_text(t1));
    exit(EXIT_FAILURE);
}

static const PrimKind arith[4][4] = {
    /* void	*/ {VOID, VOID, VOID, VOID},
    /* char	*/ {VOID, INTEGER, INTEGER, VOID},
    /* int 	*/ {VOID, INTEGER, INTEGER, REAL},
    /* real	*/ {VOID, VOID, REAL, REAL}};

PrimKind unify_arith(PrimKind l, PrimKind r, const char* op)
{
    PrimKind t = arith[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimKind comp[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, INTEGER},
    /* int  */ {VOID, INTEGER, INTEGER, INTEGER},
    /* real */ {VOID, INTEGER, INTEGER, INTEGER}};

PrimKind unify_comp(PrimKind l, PrimKind r, const char* op)
{
    PrimKind t = comp[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimKind bitwise[4][4] = {
    /* void */ {VOID, VOID, VOID, VOID},
    /* char */ {VOID, INTEGER, INTEGER, VOID},
    /* int  */ {VOID, INTEGER, INTEGER, VOID},
    /* real */ {VOID, VOID, VOID, VOID}
};

PrimKind unify_bitwise(PrimKind l, PrimKind r, const char* op)
{
    PrimKind t = bitwise[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

static const PrimKind unary[4] = {VOID, INTEGER, INTEGER, INTEGER};

PrimKind unary_verify(PrimKind u, const char* op)
{
    PrimKind t = unary[u];
    if (t == VOID)
        type_error_unary(op, u);
    return t;
}

static const PrimKind btnot[4] = {VOID, INTEGER, INTEGER, VOID};

PrimKind btnot_verify(PrimKind u, const char* op)
{
    PrimKind t = btnot[u];
    if (t == VOID)
        type_error_unary(op, u);
    return t;
}

PrimKind assign_verify(PrimKind l, PrimKind r, const char* op){
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
