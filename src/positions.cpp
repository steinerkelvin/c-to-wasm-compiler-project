#include "positions.hpp"

std::ostream& operator<<(std::ostream& stream, const pos::Pos& pos) {
    return pos.write_repr(stream);
}
