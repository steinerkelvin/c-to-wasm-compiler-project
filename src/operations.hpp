#if !defined(OPERATIONS_H)
#define OPERATIONS_H

#include "types.hpp"

namespace ops {
    using types::PrimKind;
    void PrimType_error(const char* op, PrimKind t1, PrimKind t2);
    PrimKind unify_arith(PrimKind l, PrimKind r, const char* op);
    PrimKind unify_comp(PrimKind l, PrimKind r, const char* op);
    PrimKind unify_bitwise(PrimKind l, PrimKind r, const char* op);
    PrimKind unary_verify(PrimKind u, const char* op);
    PrimKind btnot_verify(PrimKind u, const char* op);
    PrimKind assign_verify(PrimKind l, PrimKind r, const char* op);
};

#endif /* OPERATIONS_H */
