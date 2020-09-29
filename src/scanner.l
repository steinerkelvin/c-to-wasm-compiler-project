%option outfile="scanner.cpp"
%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <stdio.h>
#include "parser.h"

void yyerror (char const* s) {
    printf("SYNTAX ERROR (%d): %s\n", yylineno, s);
    exit(EXIT_FAILURE);
}
%}


digit        0|([1-9]+[0-9]*)
alpha        [a-zA-Z]+
alphanum     [a-zA-Z0-9]+
real         (0|([1-9]+[0-9]*))"."[0-9]+

%%

    /* Ignora espacos em branco */
[ \t\n]                 { }

    /* Ignora comentarios */
"/*"[^*/]*"*/"          { } 
"//".*(\\\n.*)*\n       { }

    /* Ignora diretivas para o preprocessador */
^[ \t\n]*"#".*\n        { }

auto             {return AUTO;}
break            {return BREAK;}
case             {return CASE;}
char             {return CHAR;}
const            {return CONST;}
continue         {return CONTINUE;}
default          {return DEFAULT;}
do               {return DO;}
double           {return DOUBLE;}
else             {return ELSE;}
enum             {return ENUM;}
extern           {return EXTERN;}
float            {return FLOAT;}
for              {return FOR;}
goto             {return GOTO;}
if               {return IF;}
int              {return INT;}
long             {return LONG;}
register         {return REGISTER;}
return           {return RETURN;}
short            {return SHORT;}
signed           {return SIGNED;}
sizeof           {return SIZEOF;}
static           {return STATIC;}
struct           {return STRUCT;}
switch           {return SWITCH;}
typedef          {return TYPEDEF;}
union            {return UNION;}
unsigned         {return UNSIGNED;}
void             {return VOID;}
volatile         {return VOLATILE;}
while            {return WHILE;}

"+"              {return PLUS;}
"-"              {return MINUS;}
"*"              {return STAR;}
"/"              {return OVER;}
"++"             {return PLUSPLUS;}
"--"             {return MINUSMINUS;}
,                {return COMMA;}
"="              {return ASSIGN;}
"=="             {return EQ;}
"<"              {return LT;}
">"              {return BT;}
"<="             {return LET;}
">="             {return BET;}
"("              {return LPAR;}
")"              {return RPAR;}
"["              {return LB;}
"]"              {return RB;}
"{"              {return LCB;}
"}"              {return RCB;}
";"              {return SEMI;}
":"				 {return COLON;}

{digit}          {return INT_VAL;}
{real}           {return REAL_VAL;}
\".*\"           {return STR_VAL;}

[a-zA-Z_][0-9a-zA-Z_]*        {return ID;}

.                           {printf("LEXICAL ERROR: Unknown symbol %s\n", yytext);
                              exit(EXIT_FAILURE); }
