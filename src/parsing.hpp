#if !defined(PARSING_H)
#define PARSING_H

#include <list>

#include "generated_parser.hpp"

// Tipo definido pelo Bison que é retornado pelo scanner
typedef enum yytokentype yytoken_kind_t;

// Estrutura para armazenar informações de um token, incluindo o lexema
typedef struct token {
    yytoken_kind_t type;
    char* lexeme;
    union {
        size_t str_id;
    };
} token_t;

// // Enumeração de tipos possíveis de um valor semântico do parser
// typedef enum svalue_type {
//     EMPTY = 0,
//     TOKEN,
//     SLIST,
// } svalue_type_t;

// // União discriminada que consiste em um valor semântico do parser
// typedef struct YYSTYPE stype_t;
// struct YYSTYPE {
//     svalue_type_t tag;
//     union {
//         token_t token;
//         std::list<stype_t> *slist;
//     } value;
// };

// static inline stype_t slist_new() {
//     stype_t slist = (stype_t){.tag = SLIST};
//     slist.value.slist = new std::list<stype_t>();
//     return slist;
// }

// static inline stype_t *slist_push(stype_t *slist, stype_t item) {
//     (*slist).value.slist->push_back(item);
//     return slist;
// }

// #define HANDLE_DECLARATION(SPECS, INITS)                                       \
//     {                                                                          \
//         for (auto it : *(SPECS).value.slist) {                                 \
//             if (it.tag != TOKEN) {                                             \
//                 continue;                                                      \
//             }                                                                  \
//             if (strcmp(it.value.token.lexeme, "typedef") == 0) {               \
//                 for (auto it : *(INITS).value.slist) {                         \
//                     if (it.tag != TOKEN) {                                     \
//                         continue;                                              \
//                     }                                                          \
//                     insert_typename(it.value.token.lexeme);                    \
//                 }                                                              \
//             }                                                                  \
//         }                                                                      \
//     }

// Macros para o scanner para tratar um token
#if defined(DUMP_TOKENS)

// Estas apenas imprimem o tipo do token seguido do lexema.
// Usada para testes/debug.
#define HANDLE_TOKEN(TOK, PROC)                                                \
    { printf("%s : %s\n", #TOK, yytext); }
#define HANDLE_TOKEN_ID(PROC) HANDLE_TOKEN(ID, PROC);

#else

#define HANDLE_TOKEN(TOK, PROC)                                                \
    {                                                                          \
        /* yylval = (YYSTYPE){                                                 \
            .tag = TOKEN,                                                      \
        };                                                                     \
        yylval.value.token = {                                                 \
            .type = (TOK),                                                     \
            .lexeme = strdup((yytext)),                                        \
        };                                                                     \
        __attribute__((unused)) token_t *const token = &yylval.value.token; */ \
        PROC;                                                                  \
        return TOK;                                                            \
    }
#define HANDLE_TOKEN_ID(PROC)                                                  \
    {                                                                          \
        if (is_typename(yytext)) {                                             \
            yylval.TYPENAME = 0;                                               \
            HANDLE_TOKEN(TYPENAME, PROC);                                      \
        } else {                                                               \
            yylval.ID = new std::string(yytext);                                   \
            HANDLE_TOKEN(ID, PROC);                                            \
        }                                                                      \
    }

#endif

#endif // PARSING_H
