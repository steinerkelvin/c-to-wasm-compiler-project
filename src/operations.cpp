#include <cstdio>
#include "types.hpp"

static const Type arith[4][4] = {	
/*void*/    { NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE },
/*char*/    { NO_TYPE, INTEGER, INTEGER, NO_TYPE },
/*int*/     { NO_TYPE, INTEGER, INTEGER, REAL },
/*real*/    { NO_TYPE, NO_TYPE, REAL, REAL }
};


void type_error(const char* op, Type t1, Type t2) {
    printf("SEMANTIC ERROR (0): incompatible types for operator '%s', LHS is '%s' and RHS is '%s'.\n",
    	op, get_text(t1), get_text(t2));
    exit(EXIT_FAILURE);
}

Type unify_arith(Type l, Type r, char* op){
	Type t = arith[l][r];
	if(t == NO_TYPE)
		type_error(op, l, r);
}

static const Type comp[4][4] = {
/*void*/    { NO_TYPE, NO_TYPE, NO_TYPE, NO_TYPE },
/*char*/    { NO_TYPE, INTEGER, INTEGER, INTEGER },
/*int*/     { NO_TYPE, INTEGER, INTEGER, INTEGER },
/*real*/    { NO_TYPE, INTEGER, INTEGER, INTEGER }
};

Type unify_comp(Type l, Type r, char* op) {
    Type t = comp[l][r];
    if(t == NO_TYPE)
		type_error(op, l, r);
}