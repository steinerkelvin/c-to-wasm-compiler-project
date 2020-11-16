#include "ast.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &stream, const ast::Node &node)
{
    node.write_repr(stream);
    return stream;
}

// necessário em types.hpp
// TODO refatorar?
std::ostream &operator<<(std::ostream &stream, const ast::Expr &node)
{
    node.write_repr(stream);
    return stream;
}
