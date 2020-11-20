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
    Pos(const YYLTYPE& yypos);
    std::ostream& write_repr(std::ostream& stream) const;
    /// Merges two ranges resulting in the range that spans these two
    static Pos merge(const Pos& p1, const Pos& p2);
};

class HasPosition {
    std::optional<pos::Pos> pos;

  public:
    const std::optional<Pos> get_pos() const;

    void set_pos(pos::Pos pos);

    /// Merge position from another node into this node
    void merge_pos_from(const HasPosition* node);
};

}; // namespace pos

std::ostream& operator<<(std::ostream& stream, const pos::Pos& pos);

#endif
