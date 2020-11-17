#if !defined(OPERATIONS_H)
#define OPERATIONS_H

/**
 * @file operations.hpp
 *
 * Type checking, type converstion and building AST nodes on expressions
 */

#include "ast.hpp"
#include "types.hpp"

namespace ops {
using ast::Expr;
using ast::Exprs;
using types::Type;

Type* unify_arith(const Type* l, const Type* r, const char* op);
Type* unify_comp(const Type* l, const Type* r, const char* op);
Type* unify_bitwise(const Type* l, const Type* r, const char* op);
Type* unary_verify(const Type* u, const char* op);
Type* btnot_verify(const Type* u, const char* op);
Type* assign_verify(Type* l, Type* r, const char* op);

Expr* check_assignment(Type* target_type, Expr *value);

Expr* address_of(Expr* value);
// Expr *derreference(Expr *value); // TODO
Expr* index_access(Expr* value, Expr* index);
Expr* function_call(Expr* value, ast::Exprs* parameters);

}; // namespace ops

#endif /* OPERATIONS_H */
