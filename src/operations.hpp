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

// TODO old
Type* unify_bitwise_old(const Type* l, const Type* r, const char* op);

Expr* make_unary(
    Expr* node, ast::UnBuilder builder, const char* op, pos::Pos posi);
Expr* make_btnot(
    Expr* node, ast::UnBuilder builder, const char* op, pos::Pos posi);

Expr* unify_additive(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi);
Expr* unify_multi(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi);
Expr* unify_comp(
    Expr* node1,
    Expr* node2,
    ast::BinBuilder builder,
    const char* op,
    pos::Pos posi);

Expr* check_assign(Type* target_type, Expr* value, pos::Pos posi);
Expr* unify_assign(Expr* target, Expr* value, pos::Pos posi);

Expr* check_bool(Expr* cond_node, pos::Pos posi);

Expr* address_of(Expr* value, pos::Pos posi);
Expr* derreference(Expr* value, pos::Pos posi);
Expr* index_access(Expr* value, Expr* index, pos::Pos posi);
Expr* function_call(Expr* value, ast::Exprs* parameters, pos::Pos posi);

}; // namespace ops

#endif /* OPERATIONS_H */
