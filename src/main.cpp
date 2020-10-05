#include <iostream>

#include "generated_parser.h"
#include "symtable.h"

int main(void) {
    // yydebug = 1;
    if (yyparse() == 0) {
        fprintf(stderr, "PARSE SUCCESSFUL!\n");
        return 0;
    } else {
        fprintf(stderr, "PARSE FAILED!\n");
        return 1;
    }
}
