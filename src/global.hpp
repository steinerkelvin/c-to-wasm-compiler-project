#if !defined(GLOBAL_H)
#define GLOBAL_H

#include "ast.hpp"
#include "types.hpp"

extern ast::Expr* last_expr;
extern ast::Program* root;
extern types::PrimType res_type;

#endif /* GLOBAL_H */
