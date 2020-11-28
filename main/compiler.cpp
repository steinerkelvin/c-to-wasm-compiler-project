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

    symtb::compute_offsets(back::base_activ_record_size);

    // std::cout << (*last_expr) << std::endl;
    // std::cout << (*root) << std::endl;
    // strtb::repr(std::cerr);

    // for (auto decl : root->get_children()) {
    //     if (auto func_def = dynamic_cast<ast::FunctionDefinition*>(decl)) {
    //         std::cerr << *func_def << std::endl;
    //     }
    // }

    return 0;
}
