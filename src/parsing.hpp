#if !defined(PARSING_H)
#define PARSING_H

/**
 * @file parsing.hpp
 */

#include "generated_parser.hpp"
#include "symtable.hpp"

// Tipo definido pelo Bison que é retornado pelo scanner
typedef enum yytokentype yytoken_kind_t;

extern int yylineno;

namespace pars {

/// Gets a reference to a declared variable.
symtb::NameRef get_var(const std::string& name);

} // namespace pars

//
// Macros for handling tokens on the scanner.
//

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
        if (symtb::is_typename(yytext)) {                                       \
            yylval.TYPENAME = 0; /* TODO */                                    \
            HANDLE_TOKEN(TYPENAME, PROC);                                      \
        } else {                                                               \
            yylval.ID = new std::string(yytext);                               \
            HANDLE_TOKEN(ID, PROC);                                            \
        }                                                                      \
    }

#endif

#endif // PARSING_H
