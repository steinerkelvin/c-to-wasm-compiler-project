%option outfile="scanner.cpp"
%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <stdio.h>
#include "parsing.h"
#include "symtable.h"

void yyerror (char const* s) {
    fprintf(stderr, "SYNTAX ERROR (%d): %s\n", yylineno, s);
    exit(EXIT_FAILURE);
}
%}

digit        0|([1-9]+[0-9]*)
alpha        [a-zA-Z]+
alphanum     [a-zA-Z0-9]+
real         (0|([1-9]+[0-9]*))"."[0-9]+

%%

    /* Ignora espacos em branco */
[ \t\n\r]                 { }

    /* Ignora comentarios */
"/*"[^*/]*"*/"          { } 
"//".*(\\(\r\n|\n).*)*\n       { }

    /* Ignora diretivas para o preprocessador */
^[ \t\n\r]*"#".*\n        { }

auto                    { HANDLE_TOKEN(AUTO, {}); }
break                   { HANDLE_TOKEN(BREAK, {}); }
case                    { HANDLE_TOKEN(CASE, {}); }
char                    { HANDLE_TOKEN(CHAR, {}); }
const                   { HANDLE_TOKEN(CONST, {}); }
continue                { HANDLE_TOKEN(CONTINUE, {}); }
default                 { HANDLE_TOKEN(DEFAULT, {}); }
do                      { HANDLE_TOKEN(DO, {}); }
double                  { HANDLE_TOKEN(DOUBLE, {}); }
else                    { HANDLE_TOKEN(ELSE, {}); }
enum                    { HANDLE_TOKEN(ENUM, {}); }
extern                  { HANDLE_TOKEN(EXTERN, {}); }
float                   { HANDLE_TOKEN(FLOAT, {}); }
for                     { HANDLE_TOKEN(FOR, {}); }
goto                    { HANDLE_TOKEN(GOTO, {}); }
if                      { HANDLE_TOKEN(IF, {}); }
inline                  { HANDLE_TOKEN(INLINE, {}); }
int                     { HANDLE_TOKEN(INT, {}); }
long                    { HANDLE_TOKEN(LONG, {}); }
register                { HANDLE_TOKEN(REGISTER, {}); }
return                  { HANDLE_TOKEN(RETURN, {}); }
short                   { HANDLE_TOKEN(SHORT, {}); }
signed                  { HANDLE_TOKEN(SIGNED, {}); }
sizeof                  { HANDLE_TOKEN(SIZEOF, {}); }
static                  { HANDLE_TOKEN(STATIC, {}); }
struct                  { HANDLE_TOKEN(STRUCT, {}); }
switch                  { HANDLE_TOKEN(SWITCH, {}); }
typedef                 { HANDLE_TOKEN(TYPEDEF, {}); }
union                   { HANDLE_TOKEN(UNION, {}); }
unsigned                { HANDLE_TOKEN(UNSIGNED, {}); }
void                    { HANDLE_TOKEN(VOID, {}); }
volatile                { HANDLE_TOKEN(VOLATILE, {}); }
while                   { HANDLE_TOKEN(WHILE, {}); }

"+"                     { HANDLE_TOKEN(PLUS, {}); }
"-"                     { HANDLE_TOKEN(MINUS, {}); }
"*"                     { HANDLE_TOKEN(STAR, {}); }
"/"                     { HANDLE_TOKEN(OVER, {}); }
"%"                     { HANDLE_TOKEN(PERC, {}); }
"++"                    { HANDLE_TOKEN(PLUSPLUS, {}); }
"--"                    { HANDLE_TOKEN(MINUSMINUS, {}); }
"=="                    { HANDLE_TOKEN(EQ, {}); }
"!="                    { HANDLE_TOKEN(NEQ, {}); }
"<"                     { HANDLE_TOKEN(LT, {}); }
">"                     { HANDLE_TOKEN(GT, {}); }
"<="                    { HANDLE_TOKEN(LET, {}); }
">="                    { HANDLE_TOKEN(GET, {}); }
"*="                    { HANDLE_TOKEN(STARASS, {}); } 
"/="                    { HANDLE_TOKEN(OVERASS, {}); }
"%="                    { HANDLE_TOKEN(MODASS, {}); }
"+="                    { HANDLE_TOKEN(PLUSASS, {}); }
"-="                    { HANDLE_TOKEN(MINASS, {}); }
"<<="                   { HANDLE_TOKEN(SLASS, {}); }
">>="                   { HANDLE_TOKEN(SRASS, {}); }
"&="                    { HANDLE_TOKEN(ANDASS, {}); }
"^="                    { HANDLE_TOKEN(XORASS, {}); }
"|="                    { HANDLE_TOKEN(ORASS, {}); }

"&&"                    { HANDLE_TOKEN(AND, {}); }
"||"                    { HANDLE_TOKEN(OR, {}); }
"&"                     { HANDLE_TOKEN(AMPER, {}); }
"|"                     { HANDLE_TOKEN(BTOR, {}); }
"^"                     { HANDLE_TOKEN(BTXOR, {}); }
"!"                     { HANDLE_TOKEN(NOT, {}); }
"~"                     { HANDLE_TOKEN(BTNOT, {}); }
"<<"                    { HANDLE_TOKEN(LEFT, {}); }
">>"                    { HANDLE_TOKEN(RIGHT, {}); }
"("                     { HANDLE_TOKEN(LPAR, {}); }
")"                     { HANDLE_TOKEN(RPAR, {}); }
"["                     { HANDLE_TOKEN(LB, {}); }
"]"                     { HANDLE_TOKEN(RB, {}); }
"{"                     { HANDLE_TOKEN(LCB, {}); }
"}"                     { HANDLE_TOKEN(RCB, {}); }
"?"                     { HANDLE_TOKEN(QUEST, {}); }
";"                     { HANDLE_TOKEN(SEMI, {}); }
":"                     { HANDLE_TOKEN(COLON, {}); }
","                     { HANDLE_TOKEN(COMMA, {}); }
"."                     { HANDLE_TOKEN(DOT, {}); }
"->"                    { HANDLE_TOKEN(ARROW, {}); }
"="                     { HANDLE_TOKEN(ASSIGN, {}); }
    /* TODO  *= /= %= += -= <<= >>= &= ^= |=  */

{digit}                 { HANDLE_TOKEN(INT_VAL, {}); }
{real}                  { HANDLE_TOKEN(REAL_VAL, {}); }
    /* TODO corrigir e adicionar outras representações */

\'([^\r\n\']|"\\\'")*\'         { HANDLE_TOKEN(CHAR_VAL, {}); }
\"([^\r\n\"]|"\\\"")*\"         { HANDLE_TOKEN(STR_VAL, {}); }

[a-zA-Z_][0-9a-zA-Z_]*  { HANDLE_TOKEN_ID({}); }

.                       {   printf("LEXICAL ERROR: Unknown symbol %s\n", yytext);
                            exit(EXIT_FAILURE); }
