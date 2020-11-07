#include <iostream>

#include "generated_parser.hpp"
#include "strtable.hpp"
#include "symtable.hpp"

int main(void) {
    // yydebug = 1;

    open_scope();

    if (yyparse() == 0) {
        fprintf(stderr, "PARSE SUCCESSFUL\n");
    } else {
        fprintf(stderr, "PARSE FAILED\n");
        return 1;
    }

    // strtb_print(stderr);

    return 0;
}
