%output "parser.cpp"        // File name of generated parser.
%defines "parser.h"         // Produces a 'parser.h'
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.

%define api.value.type {std::string}

%code requires {
#include <string>
}

%{
#include <stdio.h>
#include <string>
#include <iostream>

int yylex(void);
void yyerror(char const *s);
%}

%token AUTO CONST VOLATILE STATIC EXTERN
%token LONG REGISTER SIGNED UNSIGNED
%token VOID CHAR SHORT INT FLOAT DOUBLE
%token TYPEDEF SIZEOF
%token ENUM STRUCT UNION
%token BREAK CASE CONTINUE DEFAULT DO ELSE FOR GOTO IF RETURN SWITCH WHILE
%token ASSIGN EQ LT BT LET BET LPAR RPAR SEMI LCB RCB COLON LB RB
%token PLUS MINUS STAR OVER PLUSPLUS MINUSMINUS
%token INT_VAL REAL_VAL ID STR_VAL

%left COMMA
%left EQ LT BT LET BET
%left PLUS MINUS
%left STAR OVER
%precedence PLUSPLUS MINUSMINUS
%precedence UMINUS
%precedence RPAR
%precedence ELSE
%left LPAR // Associação da chamada de função

// TODO Outras precedências e associatividade

%%

all : program                   { $$ = $1; std::cout << $$ << std::endl; }

program :
      program program-part      { $$ = $1 + $2; }
    | %empty                    { $$ = std::string(""); }
    ;
program-part :
      function-definition
    | declaration
    ;

declaration :
      function-declaration
    | var-declaration-stmt
    ;

function-declaration :
      type-spec ID LPAR param-list RPAR SEMI            { $$ = $1 + " " + "x" + "(" + $4 + ")" + ";\n"; }
    ;
function-definition :
      type-spec ID LPAR param-list RPAR compound-stmt   { $$ = $1 + " " + "x" + "(" + $4 + ")" + $6; }
    ;

// ? Há maneira melhor de especificar lista de elementos separados por vírgula?
param-list :
      param-list COMMA param-spec   { $$ = $1 + ", " + $3; }
    | param-spec
    | %empty
    ;
param-spec :
      type-spec
    | type-spec ID      { $$ = $1 + " " + "x"; }
    ;

type-spec :
      VOID              { $$ = std::string("void"); }
    | CHAR              { $$ = std::string("char"); }
    | SHORT             { $$ = std::string("short"); }
    | INT               { $$ = std::string("int"); }
    | FLOAT             { $$ = std::string("float"); }
    | DOUBLE            { $$ = std::string("double"); }
    | ID                { $$ = std::string("x"); }
    ;


stmt :
      empty-stmt
    | compound-stmt
    | var-declaration-stmt
    | assign-stmt
    | if-stmt
    | return-stmt
    | break-stmt
    | case-stmt
    | default-stmt
    | continue-stmt
    | while-stmt
    | do-while-stmt
    | for-stmt
    | switch-stmt
    | expr-stmt
    ;

empty-stmt :
      SEMI                          { $$ = std::string(";\n"); }
    ;

compound-stmt :
      LCB stmt-list RCB             { $$ = std::string("{\n") + $2 + "}\n"; }
    ;
stmt-list :
      stmt-list stmt                { $$ = $1 + $2; }
    | %empty                        { $$ = std::string(""); }
    ;

// TODO qualificadores, ponteiros, arrays
var-declaration-stmt :
    type-spec var-list SEMI         { $$ = $1 + " " + $2 + ";\n" ; }
    ;
var-list :
      var-list COMMA var-part       { $$ = $1 + ", " + $3; }
    | var-part
    ;
var-part :
      ID                            { $$ = std::string("x"); }
    | ID ASSIGN expr                { $$ = std::string("x") + " = " + $3; }
    | array-stmt ///////////////////////////  ARRAY  //////////////////////////
    ;
array-stmt: 
    array 
    | array ASSIGN LCB arg-list RCB
    ;
array: 
    ID LB expr RB 
    | ID LB RB 
    ;    

assign-stmt :
      expr ASSIGN expr SEMI         { $$ = $1 + " = " + $3 + ";\n" ; }
    ;

if-stmt :
      IF LPAR expr RPAR stmt                { std::string("if (") + $3 + ")" + $4; }
    | IF LPAR expr RPAR stmt ELSE stmt      { std::string("if (") + $3 + ")" + $4 + "else" + $6; }
    ;

return-stmt :
    RETURN return-value SEMI        { $$ = std::string("return ") + $2 + ";\n" ;  }
    ;
return-value :
      expr                          { $$ = $1; }
    | %empty                        { $$ = std::string(""); }
    ;

continue-stmt :
      CONTINUE SEMI                 { $$ = std::string("continue") + ";\n" ; }
    ;
break-stmt :
      BREAK SEMI                    { $$ = std::string("break") + ";\n" ; }
    ;
case-stmt :
      CASE expr COLON stmt          { $$ = std::string("case ") + $2 + ":\n" + $4 ; }
    ;
default-stmt :
      DEFAULT COLON stmt            { $$ = std::string("default ") + ":\n" + $3 ; }
    ;

while-stmt :
      WHILE LPAR expr RPAR stmt     { $$ = std::string("while (") + $3 + ") " + $5; }
    ;

do-while-stmt :
    DO stmt WHILE LPAR expr RPAR SEMI   { $$ = std::string("do ") + $2 + "while (" + $5 + ")" + ";\n" ; }
    ;

// TODO corrigir esses stmt
for-stmt:
      FOR LPAR stmt stmt expr RPAR stmt     { $$ = std::string("for (;;) ") + $7; }
    | FOR LPAR stmt stmt RPAR stmt          { $$ = std::string("for (;;) ") + $6; }
    ;

switch-stmt :
      SWITCH LPAR expr RPAR stmt            { $$ = std::string("switch (") + $3 + ") " + $5; }
    ;

expr-stmt :
      expr SEMI                             { $$ = $1 + ";\n" ; }
    ;


expr: expr LT expr          { $$ = std::string("(") + $1 + "<" + $3 + ")"; }
    | expr BT expr          { $$ = std::string("(") + $1 + ">" + $3 + ")"; }
    | expr LET expr         { $$ = std::string("(") + $1 + "<=" + $3 + ")"; }
    | expr BET expr         { $$ = std::string("(") + $1 + ">=" + $3 + ")"; }
    | expr EQ expr          { $$ = std::string("(") + $1 + "==" + $3 + ")"; }
    | expr PLUS expr        { $$ = std::string("(") + $1 + "+" + $3 + ")"; }
    | expr MINUS expr       { $$ = std::string("(") + $1 + "-" + $3 + ")"; }
    | expr STAR expr        { $$ = std::string("(") + $1 + "*" + $3 + ")"; }
    | expr OVER expr        { $$ = std::string("(") + $1 + "/" + $3 + ")"; }
    | MINUS expr %prec UMINUS   { $$ = std::string("(-") + $2 + ")"; }
    | expr PLUSPLUS         { $$ = std::string("(") + $1 + "++" + ")"; }
    | expr MINUSMINUS       { $$ = std::string("(") + $1 + "--" + ")"; }
    | LPAR expr RPAR        { $$ = std::string("(") + $2 + ")"; }
    | function-call         { $$ = std::string("(") + $1 + ")"; }
    | INT_VAL               { $$ = std::string("0"); }
    | REAL_VAL              { $$ = std::string("0.0"); }
    | STR_VAL               { $$ = std::string("\"\""); }
    | ID                    { $$ = std::string("x"); }
    ;
    // TODO acesso de índice, derreferenciação, acesso de membro (incluindo ->)
    // cast

function-call :
    expr LPAR arg-list RPAR     { $$ = $1 + "(" + $3 + ")"; }
    ;
arg-list :
      arg-list COMMA expr       { $$ = $1 + ", " + $3; }
    | expr                      { $$ = $1; }
    | %empty                    { $$ = std::string(""); }

%%

int main(void) {
    if (yyparse() == 0) fprintf(stderr, "PARSE SUCCESSFUL!\n");
    else                fprintf(stderr, "PARSE FAILED!\n");
    return 0;
}
