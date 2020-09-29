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
%token ASSIGN EQ LT BT LET BET LPAR RPAR SEMI LCB RCB COLON
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

all : program               { $$ = $1; std::cout << $$ << std::endl; }

program :
    program program-part    { $$ = $1 + $2; }
    | %empty                { $$ = std::string(""); }
    ;
program-part : 
    function-definition     { $$ = std::string("void func() {\n}\n"); }
    | declaration           
    ;

declaration :
    function-declaration    { $$ = std::string("void func();\n"); }
    | var-declaration-stmt  { $$ = std::string("int x = 0;\n"); }
    ;

function-declaration : type-spec ID LPAR param-list RPAR SEMI ;
function-definition  : type-spec ID LPAR param-list RPAR compound-stmt ;

// ? Há maneira melhor de especificar lista de elementos separados por vírgula?
param-list : param-list COMMA param-spec | param-spec | %empty ;
param-spec : type-spec | type-spec ID ;

type-spec : VOID | CHAR | SHORT | INT | FLOAT | DOUBLE | ID ;

var-list : var-list COMMA var-part | var-part ;
var-part : ID | ID ASSIGN expr ;

return-stmt : RETURN return-value SEMI;
return-value : expr | %empty ;

continue-stmt : CONTINUE SEMI ;
break-stmt : BREAK SEMI ;
case-stmt : CASE expr COLON stmt ;//problema: só aceita constantes
default-stmt : DEFAULT COLON stmt ;

stmt :
      empty-stmt
    | compound-stmt
    | var-declaration-stmt 
    | assign-stmt
    | if-stmt
    | return-stmt
	| break-stmt
	| case-stmt//o case e o default são única e exclusivamente do switch?
	| default-stmt
	| continue-stmt
    | expr-stmt
	| while-stmt
	| do-while-stmt
	| for-stmt
	| switch-stmt
    ;

empty-stmt : SEMI ;

compound-stmt : LCB stmt-list RCB ;
stmt-list : stmt-list stmt | %empty ;

// TODO qualificadores, ponteiros, arrays
var-declaration-stmt : type-spec var-list SEMI ;

assign-stmt : expr ASSIGN expr SEMI ;

if-stmt : IF LPAR expr RPAR stmt | IF LPAR expr RPAR stmt ELSE stmt ;

while-stmt : WHILE LPAR expr RPAR stmt ;

do-while-stmt : DO stmt WHILE LPAR expr RPAR ;

for-stmt: FOR LPAR stmt stmt expr RPAR stmt | FOR LPAR stmt stmt RPAR stmt;

switch-stmt : SWITCH LPAR expr RPAR stmt;//há um sério problema aqui, que é não aceitar condicionais, somente constantes;

// TODO switch. incluir: continue e break nos loops e switch

expr-stmt : expr SEMI ;


expr: expr LT expr
    | expr BT expr
    | expr LET expr
    | expr BET expr
    | expr EQ expr
    | expr PLUS expr
    | expr MINUS expr
    | expr STAR expr
    | expr OVER expr
    | MINUS expr %prec UMINUS
    | expr PLUSPLUS
    | expr MINUSMINUS
    | LPAR expr RPAR
    | function-call
    | INT_VAL
    | REAL_VAL
    | STR_VAL
    | ID
    ;
    // TODO acesso de índice, derreferenciação, acesso de membro (incluindo ->)
    // cast

function-call : expr LPAR arg-list RPAR
arg-list : arg-list COMMA expr | expr | %empty 

%%

int main(void) {
    if (yyparse() == 0) printf("PARSE SUCCESSFUL!\n");
    else                printf("PARSE FAILED!\n");
    return 0;
}
