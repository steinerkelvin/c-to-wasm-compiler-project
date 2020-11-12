#if !defined(OPERATIONS_H)
#define OPERATIONS_H

#include "types.hpp"

namespace ops {
    using types::Type;
    Type* unify_arith(const Type* l, const Type* r, const char* op);
    Type* unify_comp(const Type* l, const Type* r, const char* op);
    Type* unify_bitwise(const Type* l, const Type* r, const char* op);
    Type* unary_verify(const Type* u, const char* op);
    Type* btnot_verify(const Type* u, const char* op);
    Type* assign_verify(const Type* l, const Type* r, const char* op);
};

#endif /* OPERATIONS_H */