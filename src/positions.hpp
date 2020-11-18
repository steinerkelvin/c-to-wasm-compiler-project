#if !defined(POSITIONS_H)
#define POSITIONS_H

#include <ostream>

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

struct Pos : YYLTYPE {
    Pos(const YYLTYPE &yypos) {
        this->first_line = yypos.first_line;
        this->first_column = yypos.first_column;
        this->last_line = yypos.last_line;
        this->last_column = yypos.last_column;
    }
    std::ostream& write_repr(std::ostream& stream) {
        stream << this->first_line;
        if (this->last_line != this->first_line) {
            stream << "-" << this->last_line;
        }
        stream << ":" << this->first_column << "-" << this->last_column;
        return stream;
    }
};

#endif
