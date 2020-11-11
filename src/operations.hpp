#if !defined(OPERATIONS_H)
#define OPERATIONS_H

void type_error(const char* op, Type t1, Type t2);
Type unify_arith(Type l, Type r, char* op);
Type unify_comp(Type l, Type r, char* op)


#endif /* OPERATIONS_H */
