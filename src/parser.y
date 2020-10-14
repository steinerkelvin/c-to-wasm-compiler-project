%output "generated_parser.cpp"        // File name of generated parser.
%defines "generated_parser.h"         // Produces a 'parser.h'
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.

%define parse.trace

%define api.value.type {struct YYSTYPE}

%code requires {
#include <string>
}

%{
#include <stdio.h>
#include <string>
#include <iostream>
#include <string.h>
#include "parsing.h"
#include "symtable.h"

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
%token ASSIGN STARASS OVERASS MODASS PLUSASS MINASS SLASS SRASS ANDASS XORASS ORASS
%token PLUS MINUS STAR OVER PERC PLUSPLUS MINUSMINUS
%token EQ NEQ LT GT LET GET
%token AND OR
%token BTOR BTXOR
%token NOT BTNOT
%token LEFT RIGHT
%token LPAR RPAR LCB RCB LB RB
%token DOT ARROW AMPER
%token SEMI COLON COMMA QUEST ELLIPSIS
%token INT_VAL REAL_VAL CHAR_VAL STR_VAL
%token ID TYPENAME

// // EZ: Criei dois níveis de prioridades distintos para poder usar nas regras mais para baixo.
// %precedence LOW // Acabei criando um 'token' para a prioridade baixa, mas poderia usar outro.

// %left COMMA
// %right ASSIGN
// %left EQ LT GT LET GET
// %left PLUS MINUS
// %left STAR OVER
// %precedence PLUSPLUS MINUSMINUS
// %precedence PREFIX
%precedence RPAR
%precedence ELSE
// %precedence POINTER
// %left LPAR  // Associação da chamada de função
// %left LB    // Associação do operador de índice

// %precedence LB // Aqui tem de ser '[' porque queremos que o parser seja guloso.

%%

all : program ;

program :
      program program-part
    | %empty
    ;
program-part :
      function-definition
    | declaration
    ;

declaration :
      declaration-specifiers SEMI
    | declaration-specifiers[specs] init-declarator-list[inits]     { HANDLE_DECLARATION($specs, $inits); } SEMI
    ;

declaration-specifiers :
      declaration-specifiers[list] declaration-specifier[spec]      { $$ = $list;       slist_push(&($$), $spec); }
    | declaration-specifier[spec]                                   { $$ = slist_new(); slist_push(&($$), $spec); }
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
      VOID
    | CHAR
    | SHORT
    | INT
    | LONG
    | FLOAT
    | DOUBLE
    | SIGNED
    | UNSIGNED
    | struct-or-union-spec
    | enum-specifier
    | typedef-name
    ;

typedef-name : TYPENAME ;

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
    | specifier-qualifier-list SEMI
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
    | ENUM ID LCB enumerator-list trailing-comma RCB
    | ENUM    LCB enumerator-list trailing-comma RCB
    ;

trailing-comma : COMMA | %empty ;

enumerator-list :
      enumerator-list COMMA enumerator
    | enumerator
    ;
enumerator :
      ID ASSIGN constant-expression
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
//       "_Alignas" LPAR type-name RPAR
//     | "_Alignas" LPAR constant-expression RPAR
//     ;

init-declarator-list :
      init-declarator-list[list] COMMA init-declarator[dec]     { $$ = $list;     ; slist_push(&($$), $dec); }
    | init-declarator[dec]                                      { $$ = slist_new(); slist_push(&($$), $dec); }
    ;
init-declarator :
      declarator ASSIGN initializer
    | declarator
    ;

declarator :
      pointer direct-declarator        { $$ = $2; }
    | direct-declarator
    ;

pointer :
      pointer STAR type-qualifier-list-opt
    | STAR type-qualifier-list-opt
    ;

direct-declarator :
      ID
    | LPAR declarator RPAR      { $$ = $2; }
    | direct-declarator LB type-qualifier-list-opt                              RB
    | direct-declarator LB type-qualifier-list-opt        assignment-expression RB
    | direct-declarator LB type-qualifier-list STATIC     assignment-expression RB
    | direct-declarator LB STATIC type-qualifier-list-opt assignment-expression RB
    | direct-declarator LB type-qualifier-list-opt STAR                         RB
    | direct-declarator LPAR parameter-type-list RPAR
    | direct-declarator LPAR identifier-list-opt RPAR
    ;

identifier-list-opt :
      identifier-list-opt COMMA ID
    | ID
    | %empty
    ;

type-qualifier-list-opt :
      type-qualifier-list
    | %empty
    ;
type-qualifier-list :
      type-qualifier-list type-qualifier
    | type-qualifier
    ;

parameter-type-list :
      parameter-list
    | parameter-list COMMA ELLIPSIS
    ;

parameter-list :
      parameter-declaration
    | parameter-list COMMA parameter-declaration
    ;
parameter-declaration :
      declaration-specifiers declarator
    | declaration-specifiers abstract-declarator-opt
    ;


type-name : specifier-qualifier-list abstract-declarator-opt ;

abstract-declarator-opt :
      abstract-declarator
    | %empty
    ;
abstract-declarator :
      pointer
    | pointer direct-abstract-declarator
    | direct-abstract-declarator
    ;

direct-abstract-declarator :
      LPAR abstract-declarator RPAR
    |                            LB type-qualifier-list-opt assignment-expression RB
    | direct-abstract-declarator LB type-qualifier-list-opt assignment-expression RB
    |                            LB type-qualifier-list-opt                       RB
    | direct-abstract-declarator LB type-qualifier-list-opt                       RB
    |                            LB STATIC type-qualifier-list-opt assignment-expression RB
    | direct-abstract-declarator LB STATIC type-qualifier-list-opt assignment-expression RB
    |                            LB type-qualifier-list STATIC     assignment-expression RB
    | direct-abstract-declarator LB type-qualifier-list STATIC     assignment-expression RB
    // |                            LB STAR RB                  // Conflito (mas sinceramente, me parece inútil)
    // | direct-abstract-declarator LB STAR RB                  // Conflito (mas sinceramente, me parece inútil)
    |                            LPAR parameter-type-list RPAR
    | direct-abstract-declarator LPAR parameter-type-list RPAR
    |                            LPAR                     RPAR
    | direct-abstract-declarator LPAR                     RPAR
    ;


initializer :
      assignment-expression
    | LCB initializer-list trailing-comma RCB
    ;

initializer-list :
      designation-opt initializer
    | initializer-list COMMA designation-opt initializer
    ;

designation-opt :
      designator-list ASSIGN
    | %empty
    ;

designator-list :
      designator
    | designator-list designator
    ;

designator :
      LB constant-expression RB
    | DOT ID
    ;


function-definition :
      declaration-specifiers declarator declaration-list-opt compound-stmt
    ;

declaration-list-opt :
      declaration-list-opt declaration
    | %empty
    ;


stmt :
      empty-stmt
    | labeled-stmt
    | compound-stmt
    | if-stmt
    | switch-stmt
    | case-stmt
    | default-stmt
    | goto-stmt
    | break-stmt
    | continue-stmt
    | return-stmt
    | while-stmt
    | do-while-stmt
    | for-stmt
    | expr-stmt
    ;

empty-stmt :
      SEMI
    ;

labeled-stmt : ID COLON stmt ;

goto-stmt : GOTO ID SEMI ;

compound-stmt :
      LCB {open_scope();} block-list-opt {close_scope();} RCB
    ;
block-list-opt :
      block-list-opt block-item
    | %empty
    ;
block-item : declaration | stmt ;

if-stmt :
      IF LPAR expression RPAR stmt
    | IF LPAR expression RPAR stmt ELSE stmt
    ;

return-stmt :
      RETURN expression-opt SEMI
    ;

continue-stmt :
      CONTINUE SEMI
    ;
break-stmt :
      BREAK SEMI
    ;
case-stmt :
      CASE constant-expression COLON stmt
    ;
default-stmt :
      DEFAULT COLON stmt
    ;

while-stmt :
      WHILE LPAR expression RPAR stmt
    ;

do-while-stmt :
    DO stmt WHILE LPAR expression RPAR SEMI
    ;

for-stmt :
      FOR LPAR expression-opt SEMI expression-opt SEMI expression-opt RPAR stmt
    | FOR LPAR declaration         expression-opt SEMI expression-opt RPAR stmt
    ;

switch-stmt :
      SWITCH LPAR expression RPAR stmt
    ;

expr-stmt :
      expression SEMI
    ;

expression-opt : expression | %empty ;

expression : comma-expression ;

comma-expression :
      assignment-expression
    | expression COMMA assignment-expression
    ;

assignment-expression :
      conditional-expression
    | unary-expression assignment-operator assignment-expression
    ;

assignment-operator :
      ASSIGN
    | STARASS
    | OVERASS
    | MODASS
    | PLUSASS
    | MINASS
    | SLASS
    | SRASS
    | ANDASS
    | XORASS
    | ORASS
    ;

constant-expression : conditional-expression ;

conditional-expression :
      or-expression
    | or-expression QUEST expression COLON conditional-expression
    ;

or-expression :
      and-expression
    | or-expression OR and-expression
    ;

and-expression :
      bit-or-expression
    | and-expression AND bit-or-expression
    ;

bit-or-expression :
      bit-xor-expression
    | bit-or-expression BTOR bit-xor-expression
    ;

bit-xor-expression :
      bit-and-expression
    | bit-xor-expression BTXOR bit-and-expression
    ;

bit-and-expression :
      equality-expression
    | bit-and-expression AMPER equality-expression
    ;

equality-expression :
      relational-expression
    | equality-expression EQ  relational-expression
    | equality-expression NEQ relational-expression
    ;

relational-expression :
      shift-expression
    | relational-expression LT shift-expression
    | relational-expression GT shift-expression
    | relational-expression LET shift-expression
    | relational-expression GET shift-expression
    ;

shift-expression :
      additive-expression
    | shift-expression LEFT  additive-expression
    | shift-expression RIGHT additive-expression
    ;

additive-expression :
      multiplicative-expression
    | additive-expression PLUS  multiplicative-expression
    | additive-expression MINUS multiplicative-expression
    ;

multiplicative-expression :
      cast-expression
    | multiplicative-expression STAR cast-expression
    | multiplicative-expression OVER cast-expression
    | multiplicative-expression PERC cast-expression
    ;

cast-expression :
      unary-expression
    | LPAR type-name RPAR cast-expression
    ;

unary-expression :
      postfix-expression                        // %prec LOW
    | PLUSPLUS   unary-expression
    | MINUSMINUS unary-expression
    | AMPER cast-expression
    | STAR  cast-expression
    | PLUS  cast-expression
    | MINUS cast-expression
    | BTNOT cast-expression
    | NOT   cast-expression
    | SIZEOF unary-expression
    | SIZEOF LPAR type-name RPAR
    // | _Alignof LPAR type-name RPAR
    ;

postfix-expression :
      primary-expression
    | postfix-expression LB expression RB       // %prec LB
    | postfix-expression LPAR argument-expression-list-opt RPAR
    | postfix-expression DOT   ID
    | postfix-expression ARROW ID
    | postfix-expression PLUSPLUS
    | postfix-expression MINUSMINUS
    // | ( type-name ) { initializer-list }     // TODO
    // | ( type-name ) { initializer-list , }
    ;


argument-expression-list-opt : argument-expression-list | %empty ;
argument-expression-list :
      argument-expression-list COMMA assignment-expression
    | assignment-expression
    ;

primary-expression :
      ID
    | INT_VAL
    | REAL_VAL
    | CHAR_VAL
    | STR_VAL
    | LPAR expression RPAR
    // | generic-expression ??
    ;


%%
