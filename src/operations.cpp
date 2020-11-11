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
    /* chad */ {VOID, INTEGER, INTEGER, INTEGER},
    /* intd */ {VOID, INTEGER, INTEGER, INTEGER},
    /* read */ {VOID, INTEGER, INTEGER, INTEGER}};

PrimType unify_comp(PrimType l, PrimType r, const char* op)
{
    PrimType t = comp[l][r];
    if (t == VOID)
        type_error(op, l, r);
    return t;
}

} // namespace ops
