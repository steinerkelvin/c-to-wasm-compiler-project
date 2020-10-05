#if !defined(PARSING_H)
#define PARSING_H

#include <list>

#include "generated_parser.h"

// Tipo definido pelo Bison que é retornado pelo scanner
typedef enum yytokentype yytoken_kind_t;

typedef union token_value {
} token_value_t;

typedef struct token {
    yytoken_kind_t type;
    char *lexeme;
    token_value_t value;
} token_t;

typedef enum svalue_type {
    EMPTY = 0,
    TOKEN,
    SLIST,
} svalue_type_t;

typedef struct YYSTYPE stype_t;
struct YYSTYPE {
    svalue_type_t tag;
    union {
        token_t token;
        std::list<stype_t> *slist;
    } value;
};

#if defined(DUMP_TOKENS)
#define HANDLE_TOKEN(TOK, PROC)                                                \
    { printf("%s : %s\n", #TOK, yytext); }
#define HANDLE_TOKEN_ID(PROC) HANDLE_TOKEN(ID, PROC);
#else
#define HANDLE_TOKEN(TOK, PROC)                                                \
    {                                                                          \
        yylval = (YYSTYPE){                                                    \
            .tag = TOKEN,                                                      \
        };                                                                     \
        yylval.value.token = {                                                 \
            .type = (TOK),                                                     \
            .lexeme = strdup((yytext)),                                        \
            .value = {},                                                       \
        };                                                                     \
        PROC;                                                                  \
        return TOK;                                                            \
    }
#define HANDLE_TOKEN_ID(PROC)                                                  \
    {                                                                          \
        if (is_typename(yytext)) {                                             \
            HANDLE_TOKEN(TYPENAME, PROC);                                      \
        } else {                                                               \
            HANDLE_TOKEN(ID, PROC);                                            \
        }                                                                      \
    }
#endif

#endif // PARSING_H
