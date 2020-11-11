#if !defined(PARSING_H)
#define PARSING_H

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
} Token;

// Macros para o scanner para tratar um token
#if defined(DUMP_TOKENS)

// Estas apenas imprimem o tipo do token seguido do lexema.
// Usada para testes/debug.
#define HANDLE_TOKEN(TOK, PROC)                                                \
    {                                                                          \
        printf("%s : %s\n", #TOK, yytext);                                     \
    }
#define HANDLE_TOKEN_ID(PROC) HANDLE_TOKEN(ID, PROC);

#else

#define HANDLE_TOKEN(TOK, PROC)                                                \
    {                                                                          \
        PROC;                                                                  \
        return TOK;                                                            \
    }
#define HANDLE_TOKEN_ID(PROC)                                                  \
    {                                                                          \
        if (sbtb::is_typename(yytext)) {                                       \
            yylval.TYPENAME = 0;                                               \
            HANDLE_TOKEN(TYPENAME, PROC);                                      \
        } else {                                                               \
            yylval.ID = new std::string(yytext);                               \
            HANDLE_TOKEN(ID, PROC);                                            \
        }                                                                      \
    }

#endif

#endif // PARSING_H
