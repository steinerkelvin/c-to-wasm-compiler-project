#include <iostream>

#include "generated_parser.h"
#include "symtable.h"

int main(void) {
    // yydebug = 1;
    if (yyparse() == 0) {
        fprintf(stderr, "PARSE SUCCESSFUL!\n");
    } else {
        fprintf(stderr, "PARSE FAILED!\n");
    }

    std::cout << is_typename("kek") << std::endl;

    return 0;
}
