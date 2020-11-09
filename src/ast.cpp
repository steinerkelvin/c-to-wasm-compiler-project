#include "ast.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const Node::Node& node) {
    node.write_repr(stream);
    return stream;
};
