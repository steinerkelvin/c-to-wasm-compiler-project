#include "ast.hpp"
#include <iostream>

namespace ast {
const CoersionBuilder bdI2R = [](Expr* node) { return new I2R(node); };
const CoersionBuilder bdR2I = [](Expr* node) { return new R2I(node); };
const CoersionBuilder bdI2C = [](Expr* node) { return new I2C(node); };
const CoersionBuilder bdC2I = [](Expr* node) { return new C2I(node); };
} // namespace ast

std::ostream& operator<<(std::ostream& stream, const ast::Node& node)
{
    node.write_repr(stream);
    return stream;
}

// necessário em types.hpp
// TODO refatorar?
std::ostream& operator<<(std::ostream& stream, const ast::Expr& node)
{
    node.write_repr(stream);
    return stream;
}
