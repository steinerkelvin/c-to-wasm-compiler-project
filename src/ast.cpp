#include "ast.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, Node::BaseNode& node) {
    node.write_repr(stream);
    return stream;
};
