%output "generated_parser.cpp"        
%defines "generated_parser.hpp"        
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.
%define parse.trace

// %define api.value.type {struct YYSTYPE}
%define api.value.type union

%code requires {
#include <string>
#include "ast.hpp"
}

%{
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "parsing.hpp"
#include "symtable.hpp"
#include "ast.hpp"
#include "global.hpp"

int yylex(void);
void yyerror(char const *s);
%}

%type <Node::Program*> program
%type <Node::Declaration*> program-part function-definition declaration

%type <Node::Statement*> stmt
%type <Node::Block*> compound-stmt block-list-opt
%type <Node::Statement*> block-item
%type <Node::Statement*> empty-stmt
%type <Node::Statement*> labeled-stmt
%type <Node::Statement*> if-stmt
%type <Node::Statement*> switch-stmt
%type <Node::Statement*> case-stmt
%type <Node::Statement*> default-stmt
%type <Node::Statement*> goto-stmt
%type <Node::Statement*> break-stmt
%type <Node::Statement*> continue-stmt
%type <Node::Statement*> return-stmt
%type <Node::Statement*> while-stmt
%type <Node::Statement*> do-while-stmt
%type <Node::Statement*> for-stmt
%type <Node::ExpressionStmt*> expr-stmt

%type <Node::Expr*> expression
%type <Node::Expr*> comma-expression
%type <Node::Expr*> assignment-expression
%type <Node::Expr*> constant-expression
%type <Node::Expr*> conditional-expression
%type <Node::Expr*> or-expression
%type <Node::Expr*> and-expression
%type <Node::Expr*> bit-or-expression
%type <Node::Expr*> bit-xor-expression
%type <Node::Expr*> bit-and-expression
%type <Node::Expr*> equality-expression
%type <Node::Expr*> relational-expression
%type <Node::Expr*> shift-expression
%type <Node::Expr*> additive-expression
%type <Node::Expr*> multiplicative-expression
%type <Node::Expr*> cast-expression
%type <Node::Expr*> unary-expression
%type <Node::Expr*> postfix-expression
%type <Node::Expr*> primary-expression



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

%token <long long int> INT_VAL <double> REAL_VAL <char> CHAR_VAL <size_t> STR_VAL
%token <size_t> ID <size_t> TYPENAME

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

all : program { root = $1; } ;

program
    : %empty                { $$ = new Node::Program(); }
    | program program-part  { $$ = $1; $$->add($2); }
    ;
program-part
    : function-definition
    | declaration
    ;

declaration 
    : declaration-specifiers SEMI                                       { $$ = new Node::Declaration(); }
    | declaration-specifiers[specs] init-declarator-list[inits] SEMI    { $$ = new Node::Declaration(); }
        /* { HANDLE_DECLARATION($specs, $inits); } */
    ;

declaration-specifiers :
      declaration-specifiers[list] declaration-specifier[spec]      // { $$ = $list;       slist_push(&($$), $spec); }
    | declaration-specifier[spec]                                   // { $$ = slist_new(); slist_push(&($$), $spec); }
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
      init-declarator-list[list] COMMA init-declarator[dec]     // { $$ = $list;     ; slist_push(&($$), $dec); }
    | init-declarator[dec]                                      // { $$ = slist_new(); slist_push(&($$), $dec); }
    ;
init-declarator :
      declarator ASSIGN initializer
    | declarator
    ;

declarator :
      pointer direct-declarator        // { $$ = $2; }
    | direct-declarator
    ;

pointer :
      pointer STAR type-qualifier-list-opt
    | STAR type-qualifier-list-opt
    ;

direct-declarator : 
      ID
    | LPAR declarator RPAR      // { $$ = $2; }
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


function-definition
    : declaration-specifiers declarator declaration-list-opt compound-stmt[body]
        { $$ = new Node::FunctionDefinition($body); }
    ;

declaration-list-opt :
      declaration-list-opt declaration
    | %empty
    ;


stmt 
    : empty-stmt
    | labeled-stmt
    | compound-stmt     { $$ = $1; }
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
    | expr-stmt         { $$ = $1; }
    ;

empty-stmt :
      SEMI              { $$ = NULL; }
    ;

labeled-stmt
    : ID COLON stmt { $$ = $3; }
    ;  

goto-stmt
    : GOTO ID SEMI  { $$ = NULL; }
    ;      

compound-stmt
    : LCB {open_scope();} block-list-opt[block] {close_scope();} RCB   { $$ = $block; }
    ;
block-list-opt
    : %empty                        { $$ = new Node::Block(); }
    | block-list-opt block-item     { $$ = $1; $$->add($2); }
    ;
block-item
    : declaration                   { $$ = NULL; }
    | stmt
    ;

if-stmt 
    : IF LPAR expression RPAR stmt[body]            { $$ = $body; }
    | IF LPAR expression RPAR stmt[body] ELSE stmt  { $$ = $body; }
    ;

return-stmt :
      RETURN expression-opt SEMI    { $$ = NULL; }
    ;

continue-stmt :
      CONTINUE SEMI     { $$ = NULL; }
    ;
break-stmt :
      BREAK SEMI        { $$ = NULL; }
    ;
case-stmt :
      CASE constant-expression COLON stmt[body]     { $$ = $body; }
    ;
default-stmt
    : DEFAULT COLON stmt[body]                      { $$ = $body; }
    ;

while-stmt
    : WHILE LPAR expression RPAR stmt[body]         { $$ = $body; }
    ;

do-while-stmt
    : DO stmt[body] WHILE LPAR expression RPAR SEMI   { $$ = $body; }
    ;

for-stmt
    : FOR LPAR expression-opt SEMI expression-opt SEMI expression-opt RPAR stmt[body]   { $$ = $body; }
    | FOR LPAR declaration         expression-opt SEMI expression-opt RPAR stmt[body]   { $$ = $body; }
    ;

switch-stmt
    : SWITCH LPAR expression RPAR stmt[body]    { $$ = $body; }
    ;

expr-stmt :
      expression SEMI   { $$ = new Node::ExpressionStmt($1); }
    ;

expression-opt : expression | %empty ;

expression : comma-expression       { $$ = $1; last_expr = $$; } ;

comma-expression :
      assignment-expression
    | expression COMMA assignment-expression
    ;

assignment-expression 
    : conditional-expression
    | unary-expression assignment-operator assignment-expression  { $$ = $3; }
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
    | additive-expression PLUS  multiplicative-expression   { $$ = new Node::Plus($1, $3); }
    | additive-expression MINUS multiplicative-expression   { $$ = new Node::Minus($1, $3); }
    ;

multiplicative-expression :
      cast-expression
    | multiplicative-expression STAR cast-expression    { $$ = new Node::Times($1, $3); }
    | multiplicative-expression OVER cast-expression    { $$ = new Node::Over($1, $3); }
    | multiplicative-expression PERC cast-expression
    ;

cast-expression : 
      unary-expression
    | LPAR type-name RPAR cast-expression[value]        { $$ = $value; }
    ;

unary-expression : 
      postfix-expression
    | PLUSPLUS   unary-expression   { $$ = new Node::PrefixPlusPlus($2); }
    | MINUSMINUS unary-expression   { $$ = new Node::PrefixMinusMinus($2); }
    | AMPER cast-expression         { $$ = $2; }
    | STAR  cast-expression         { $$ = $2; }
    | PLUS  cast-expression         { $$ = $2; }
    | MINUS cast-expression         { $$ = new Node::InvertSignal($2); }
    | BTNOT cast-expression         { $$ = new Node::BitNot($2); }
    | NOT   cast-expression         { $$ = new Node::Not($2); }
    | SIZEOF unary-expression       { $$ = $2; } 
    | SIZEOF LPAR type-name RPAR    { assert(0); }
    // | _Alignof LPAR type-name RPAR
    ;

postfix-expression :
      primary-expression
    | postfix-expression LB expression RB       // %prec LB
    | postfix-expression LPAR argument-expression-list-opt RPAR
    | postfix-expression DOT   ID
    | postfix-expression ARROW ID
    | postfix-expression PLUSPLUS       { $$ = new Node::PrefixPlusPlus($1);  }
    | postfix-expression MINUSMINUS     { $$ = new Node::PrefixMinusMinus($1);  }
    // | ( type-name ) { initializer-list }
    // | ( type-name ) { initializer-list , }
    ;


argument-expression-list-opt : argument-expression-list | %empty ;
argument-expression-list :
      argument-expression-list COMMA assignment-expression
    | assignment-expression
    ;

primary-expression :
      ID            { $$ = new Node::Variable($1); }
    | INT_VAL       { $$ = new Node::IntegerValue($1); }
    | REAL_VAL      { $$ = new Node::FloatingValue($1); }
    | CHAR_VAL      { $$ = new Node::CharValue($1); }
    | STR_VAL       { $$ = new Node::StringValue($1); }
    | LPAR expression RPAR      { $$ = $2; }
    // | generic-expression ??
    ;


%%
