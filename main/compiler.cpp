#include <iostream>

#include "ast.hpp"
#include "backend.hpp"
#include "generated_parser.hpp"
#include "global.hpp"
#include "strtable.hpp"
#include "symtable.hpp"

int main(void)
{
    // yydebug = 1;

    symtb::init();

    if (yyparse() == 0) {
        fprintf(stderr, "PARSE SUCCESSFUL\n");
    } else {
        fprintf(stderr, "PARSE FAILED\n");
        return 1;
    }

    const size_t strtb_size = strtb::compute_offsets();
    symtb::compute_offsets(back::base_activ_record_size);
    const size_t data_size = symtb::get_global_scope_size();

    // std::cout << (*last_expr) << std::endl;
    // std::cout << (*root) << std::endl;
    // strtb::repr(std::cerr);

    back::generate_code(std::cout, root, strtb_size, data_size);

    return 0;
}
