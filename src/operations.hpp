#if !defined(OPERATIONS_H)
#define OPERATIONS_H

#include "types.hpp"

namespace ops {
    using types::PrimType;
    void PrimType_error(const char* op, PrimType t1, PrimType t2);
    PrimType unify_arith(PrimType l, PrimType r, const char* op);
    PrimType unify_comp(PrimType l, PrimType r, const char* op);
    PrimType unify_bitwise(PrimType l, PrimType r, const char* op);
    PrimType unary_verify(PrimType u, const char* op);
    PrimType btnot_verify(PrimType u, const char* op);
};

#endif /* OPERATIONS_H */
