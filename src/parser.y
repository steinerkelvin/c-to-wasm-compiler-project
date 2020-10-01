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

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token VOID CHAR SHORT INT LONG FLOAT DOUBLE SIGNED UNSIGNED
%token CONST RESTRICT VOLATILE
%token INLINE
%token SIZEOF
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
%precedence POINTER
%left LPAR  // Associação da chamada de função
%left LB    // Associação do operador de índice

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

// declaration :
//       function-declaration
//     | var-declaration-stmt
//     ;

declaration :
	  declaration-specifiers SEMI
	| declaration-specifiers init-declarator-list SEMI
	;

declaration-specifiers :
	  declaration-specifiers declaration-specifier
	| declaration-specifier
	;
declaration-specifier :
	  storage-class-specifier
	| type-specifier
	| type-qualifier
	| function-specifier
	// | alignment-specifier
	;

storage-class-specifier :
	  TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type-specifier :
      VOID                		{ $$ = std::string("void"); }
    | CHAR                		{ $$ = std::string("char"); }
    | SHORT               		{ $$ = std::string("short"); }
    | INT                 		{ $$ = std::string("int"); }
	| LONG	               		{ $$ = std::string("long"); }
    | FLOAT               		{ $$ = std::string("float"); }
    | DOUBLE              		{ $$ = std::string("double"); }
	| SIGNED            		{ $$ = std::string("signed"); }
	| UNSIGNED 					{ $$ = std::string("unsigned"); }
    | ID                  		{ $$ = std::string("x"); }
	| typedef-name
	| struct-or-union-spec
	| enum-specifier
    ;

typedef-name : ID ;

struct-or-union-spec :
	  struct-or-union ID
	| struct-or-union ID LCB struct-declaration-list RCB
	| struct-or-union    LCB struct-declaration-list RCB
	;

struct-declaration-list :
	  struct-declaration
	| struct-declaration-list struct-declaration
	;

struct-declaration :
	specifier-qualifier-list struct-declarator-list SEMI
	specifier-qualifier-list SEMI
	// | static_assert-declaration
	;

specifier-qualifier-list :
	  specifier-qualifier-list type-specifier
	| specifier-qualifier-list type-qualifier
	| type-specifier
	| type-qualifier
	;

struct-declarator-list :
	  struct-declarator
	| struct-declarator-list COMMA struct-declarator
	;

struct-declarator :
	declarator
	//| declarator COLON constant-expression
	;

struct-or-union : STRUCT | UNION ;

enum-specifier :
	  ENUM ID
	| ENUM ID LCB enumerator-list traling-comma RCB
	| ENUM    LCB enumerator-list traling-comma RCB
	;

traling-comma : COMMA | %empty ;

enumerator-list :
	  enumerator-list COMMA enumerator
	| enumerator
	;
enumerator :
	  ID ASSIGN expr
	| ID
	;

type-qualifier :
	  CONST
	| RESTRICT
	| VOLATILE
	// | "_Atomic"
	;

function-specifier :
	  INLINE
	// | "_Noreturn"
	;

// alignment-specifier :
// 	  "_Alignas" LPAR type-name RPAR
// 	| "_Alignas" LPAR constant-expression RPAR
// 	;

init-declarator-list :
	  init-declarator-list COMMA init-declarator
	| init-declarator
	;
init-declarator :
	  declarator ASSIGN initializer
	| declarator
	;

declarator :
	  pointer direct-declarator
	| direct-declarator
	;

pointer :
	  pointer STAR type-qualifier-list-opt
	| STAR type-qualifier-list-opt
	;

direct-declarator :
	  ID
	| LPAR declarator RPAR
	| direct-declarator LB type-qualifier-list-opt expr RB
	| direct-declarator LB type-qualifier-list-opt      RB
	| direct-declarator LB type-qualifier-list-opt STAR RB
	// TODO static?
	| direct-declarator LPAR parameter-type-list RPAR
	// | direct-declarator LPAR identifier-list-opt RPAR  // TODO
	;

type-qualifier-list :
  	  type-qualifier-list type-qualifier
	| type-qualifier
	;
type-qualifier-list-opt :
	  type-qualifier-list
	| %empty
	;

parameter-type-list :
	  parameter-list
	// | parameter-list COMMA "..."
	;

parameter-list :
	  parameter-declaration
	| parameter-list COMMA parameter-declaration
	;
parameter-declaration :
	  declaration-specifiers declarator
	| declaration-specifiers abstract-declarator-opt
	;

abstract-declarator-opt : %empty;  // TODO


initializer : 
	  expr
	// | initializer-list trailing-comma  // TODO
	| array-init-expr
	;

array-init-expr : LCB expr-list RCB	;

// function-declaration :
//       type-specifier ID LPAR param-list RPAR SEMI            { $$ = $1 + " " + "x" + "(" + $4 + ")" + ";\n"; }
//     ;

function-definition :
      type-specifier ID LPAR param-list RPAR compound-stmt   { $$ = $1 + " " + "x" + "(" + $4 + ")" + $6; }
    ;

// ? Há maneira melhor de especificar lista de elementos separados por vírgula?
param-list :
      param-list COMMA param-spec   { $$ = $1 + ", " + $3; }
    | param-spec
    | %empty
    ;
param-spec :
      type-specifier
    | type-specifier ID        { $$ = $1 + " " + "x"; }
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
    type-specifier var-list SEMI         { $$ = $1 + " " + $2 + ";\n" ; }
    ;
var-list :
      var-list COMMA var-item       { $$ = $1 + ", " + $3; }
    | var-item
    ;
var-item :
      ID                            { $$ = std::string("x"); }
    | ID ASSIGN expr                { $$ = std::string("x") + " = " + $3; }
//     | array-var-item ///////////////////////////  ARRAY  //////////////////////////
//     ;
// array-var-item:
//     array-var
//     | array-var ASSIGN array-expr
//     ;
// array-expr : LCB expr-list RCB
// array-var:
//     ID LB expr RB
//     | ID LB R
//     ;

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


expr: expr LT expr              { $$ = std::string("(") + $1 + "<" + $3 + ")"; }
    | expr BT expr              { $$ = std::string("(") + $1 + ">" + $3 + ")"; }
    | expr LET expr             { $$ = std::string("(") + $1 + "<=" + $3 + ")"; }
    | expr BET expr             { $$ = std::string("(") + $1 + ">=" + $3 + ")"; }
    | expr EQ expr              { $$ = std::string("(") + $1 + "==" + $3 + ")"; }
    | expr PLUS expr            { $$ = std::string("(") + $1 + "+" + $3 + ")"; }
    | expr MINUS expr           { $$ = std::string("(") + $1 + "-" + $3 + ")"; }
    | expr STAR expr            { $$ = std::string("(") + $1 + "*" + $3 + ")"; }
    | expr OVER expr            { $$ = std::string("(") + $1 + "/" + $3 + ")"; }
    | MINUS expr %prec UMINUS   { $$ = std::string("(-") + $2 + ")"; }
    | expr PLUSPLUS             { $$ = std::string("(") + $1 + "++" + ")"; }
    | expr MINUSMINUS           { $$ = std::string("(") + $1 + "--" + ")"; }
    | LPAR expr RPAR            { $$ = std::string("(") + $2 + ")"; }
    | function-call             { $$ = std::string("(") + $1 + ")"; }
    | expr LB expr RB           { $$ = std::string("(") + $1 + "(" + $3 + ")"; }
    | STAR expr  %prec POINTER  { $$ = std::string("(*") + $1 + ")"; }
    | INT_VAL                   { $$ = std::string("0"); }
    | REAL_VAL                  { $$ = std::string("0.0"); }
    | STR_VAL                   { $$ = std::string("\"\""); }
    | ID                        { $$ = std::string("x"); }
    ;
    // TODO acesso de índice, derreferenciação, acesso de membro (incluindo ->)
    // cast

function-call :
    expr LPAR expr-list RPAR     { $$ = $1 + "(" + $3 + ")"; }
    ;

expr-list :
      expr-list COMMA expr      { $$ = $1 + ", " + $3; }
    | expr                      { $$ = $1; }
    | %empty                    { $$ = std::string(""); }
    ;

%%

int main(void) {
    if (yyparse() == 0) fprintf(stderr, "PARSE SUCCESSFUL!\n");
    else                fprintf(stderr, "PARSE FAILED!\n");
    return 0;
}
