#if !defined(POSITIONS_H)
#define POSITIONS_H

/**
 * @file positions.hpp
 * Handling of positions of entities in the input file, for error messages etc
 */

#include <optional>
#include <ostream>

/* Location type.  */
#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE {
    int first_line;
    int first_column;
    int last_line;
    int last_column;
};
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1
#endif

namespace pos {

/**
 * Represents a range in the input text, by storing first and last line and
 * column numbers.
 */
struct Pos : YYLTYPE {
    Pos(const YYLTYPE& yypos)
    {
        this->first_line = yypos.first_line;
        this->first_column = yypos.first_column;
        this->last_line = yypos.last_line;
        this->last_column = yypos.last_column;
    }
    std::ostream& write_repr(std::ostream& stream) const
    {
        stream << this->first_line;
        if (this->last_line != this->first_line) { // TEST
            stream << "-" << this->last_line;
        }
        stream << ":" << this->first_column;
        stream << "-" << this->last_column; // TEST
        return stream;
    }

    /// Merges two ranges resulting in the range that spans these two
    static Pos merge(const Pos& p1, const Pos& p2)
    {
        YYLTYPE new_pos;
        new_pos.first_line = std::min(p1.first_line, p2.first_line);
        new_pos.first_column = std::min(p1.first_column, p2.first_column);
        new_pos.last_line = std::max(p1.last_line, p2.last_line);
        new_pos.last_column = std::max(p1.last_column, p2.last_column);
        return Pos(new_pos);
    }
};

class HasPosition {
    std::optional<pos::Pos> pos;

  public:
    const std::optional<Pos> get_pos() const { return this->pos; }

    void set_pos(pos::Pos pos) { this->pos = pos; }

    /// Merge position from another node into this node
    void merge_pos_from(const HasPosition* node)
    {
        auto this_pos = this->get_pos();
        auto other_pos = node->get_pos();
        if (this_pos) {
            if (other_pos) {
                this->pos = pos::Pos::merge(*this_pos, *other_pos);
            }
        } else {
            this->pos = other_pos;
        }
    }
};

}; // namespace pos

std::ostream& operator<<(std::ostream& stream, const pos::Pos& pos);

#endif
