%output "generated_parser.cpp"        
%defines "generated_parser.hpp"        
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.
%define parse.trace

%define api.value.type union

%code requires {
#include <string>
#include "types.hpp"
#include "declarations.hpp"
#include "ast.hpp"
}

%{
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "types.hpp"
#include "declarations.hpp"
#include "parsing.hpp"
#include "symtable.hpp"
#include "ast.hpp"
#include "global.hpp"

int yylex(void);
void yyerror(char const *s);
%}

%type <ast::Program*> program
%type <ast::Declaration*> program-part function-definition declaration

%type <types::TypeQualifier*> type-qualifier
%type <types::TypeSpec*> type-specifier
%type <types::TypeQualOrTypeSpecList*> specifier-qualifier-list
%type <size_t> typedef-name
%type <bool> struct-or-union
%type <types::StructOrUnionSpec*> struct-or-union-spec
%type <types::EnumSpec*> enum-spec

%type <decl::DeclarationSpecs*> declaration-specifiers
%type <decl::DeclarationSpec*> declaration-specifier
%type <decl::StorageClassSpec*> storage-class-specifier
%type <decl::InitDeclarators*> init-declarator-list
%type <std::string*> init-declarator
%type <std::string*> declarator
%type <std::string*> direct-declarator

%type <ast::Statement*> stmt
%type <ast::Block*> compound-stmt block-list-opt
%type <ast::Statement*> block-item
%type <ast::Statement*> empty-stmt
%type <ast::Statement*> labeled-stmt
%type <ast::Statement*> if-stmt
%type <ast::Statement*> switch-stmt
%type <ast::Statement*> case-stmt
%type <ast::Statement*> default-stmt
%type <ast::Statement*> goto-stmt
%type <ast::Statement*> break-stmt
%type <ast::Statement*> continue-stmt
%type <ast::Statement*> return-stmt
%type <ast::Statement*> while-stmt
%type <ast::Statement*> do-while-stmt
%type <ast::Statement*> for-stmt
%type <ast::ExpressionStmt*> expr-stmt

%type <ast::Expr*> expression
%type <ast::Expr*> comma-expression
%type <ast::Expr*> assignment-expression
%type <ast::Expr*> constant-expression
%type <ast::Expr*> conditional-expression
%type <ast::Expr*> or-expression
%type <ast::Expr*> and-expression
%type <ast::Expr*> bit-or-expression
%type <ast::Expr*> bit-xor-expression
%type <ast::Expr*> bit-and-expression
%type <ast::Expr*> equality-expression
%type <ast::Expr*> relational-expression
%type <ast::Expr*> shift-expression
%type <ast::Expr*> additive-expression
%type <ast::Expr*> multiplicative-expression
%type <ast::Expr*> cast-expression
%type <ast::Expr*> unary-expression
%type <ast::Expr*> postfix-expression
%type <ast::Expr*> primary-expression

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
%token <std::string*> ID <size_t> TYPENAME

%precedence RPAR
%precedence ELSE

%%

all : program { root = $1; } ;

program
    : %empty                { $$ = new ast::Program(); }
    | program program-part  { $$ = $1; $$->add($2); }
    ;
program-part
    : function-definition
    | declaration
    ;

declaration 
    : declaration-specifiers SEMI
        {
            $$ = new ast::Declaration();
        }
    | declaration-specifiers[specs] init-declarator-list[inits] SEMI
        { 
            declare(*$specs, *$inits);
            delete $specs;
            delete $inits;
            $$ = new ast::Declaration();
        }
    ;

declaration-specifiers
    : declaration-specifiers[list] declaration-specifier[spec]
        { $$ = $1; $$->add($2); }
    | declaration-specifier[spec]
        { $$ = new decl::DeclarationSpecs(); $$->add($1);  }
    ;
declaration-specifier
    : storage-class-specifier   { $$ = $1; }
    | type-specifier            { $$ = new decl::TypeDeclSpec($1); }
    | type-qualifier            { $$ = new decl::TypeDeclSpec($1); }
    // | function-specifier
    // | alignment-specifier
    ;

storage-class-specifier
    : TYPEDEF   { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::TYPEDEF); }
    | EXTERN    { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::EXTERN); }
    | STATIC    { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::STATIC); }
    | AUTO      { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::AUTO); }
    | REGISTER  { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::REGISTER); }
    ;

type-specifier
    : VOID      { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::VOID); }
    | CHAR      { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::CHAR); }
    | SHORT     { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::SHORT); }
    | INT       { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::INT); }
    | LONG      { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::LONG); }
    | FLOAT     { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::FLOAT); }
    | DOUBLE    { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::DOUBLE); }
    | SIGNED    { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::SIGNED); }
    | UNSIGNED  { $$ = new types::SimpleTypeSpec(types::SimpleTypeSpec::UNSIGNED); }
    | struct-or-union-spec  { $$ = $1; }
    | enum-spec             { $$ = $1; }
    | typedef-name          { $$ = new types::TypedefName($1); }
    ;

typedef-name : TYPENAME ;

struct-or-union-spec
    : struct-or-union ID                                    { $$ = new types::StructOrUnionSpec($1); }
    | struct-or-union ID LCB struct-declaration-list RCB    { $$ = new types::StructOrUnionSpec($1); }
    | struct-or-union    LCB struct-declaration-list RCB    { $$ = new types::StructOrUnionSpec($1); }
    ;

struct-or-union
    : STRUCT    { $$ = false; }
    | UNION     { $$ = true; }
    ;

struct-declaration-list 
    : struct-declaration
    | struct-declaration-list struct-declaration
    ;

struct-declaration
    : specifier-qualifier-list struct-declarator-list SEMI
    | specifier-qualifier-list SEMI
    // | static_assert-declaration
    ;

specifier-qualifier-list
    : type-specifier    { $$ = new types::TypeQualOrTypeSpecList(); $$->add($1); }
    | type-qualifier    { $$ = new types::TypeQualOrTypeSpecList(); $$->add($1); }
    | specifier-qualifier-list type-specifier       { $$->add($2); }
    | specifier-qualifier-list type-qualifier       { $$->add($2); }
    ;

struct-declarator-list
    : struct-declarator
    | struct-declarator-list COMMA struct-declarator
    ;

struct-declarator
    : declarator
    //| declarator COLON constant-expression
    ;

enum-spec
    : ENUM ID                                           { $$ = new types::EnumSpec(); }
    | ENUM ID LCB enumerator-list trailing-comma RCB    { $$ = new types::EnumSpec(); }
    | ENUM    LCB enumerator-list trailing-comma RCB    { $$ = new types::EnumSpec(); }
    ;

trailing-comma : COMMA | %empty ;

enumerator-list
    : enumerator-list COMMA enumerator
    | enumerator
    ;
enumerator
    : ID ASSIGN constant-expression
    | ID
    ;

type-qualifier
    : CONST     { $$ = new types::TypeQualifier{types::TypeQualifier::CONST}; }
    | RESTRICT  { $$ = new types::TypeQualifier{types::TypeQualifier::RESTRICT}; }
    | VOLATILE  { $$ = new types::TypeQualifier{types::TypeQualifier::VOLATILE}; }
    // | "_Atomic"
    ;

// function-specifier
//     : INLINE
//     // | "_Noreturn"
//     ;

// alignment-specifier
//     : "_Alignas" LPAR type-name RPAR
//     | "_Alignas" LPAR constant-expression RPAR
//     ;

init-declarator-list
    : init-declarator-list[list] COMMA init-declarator[dec]
        { $$ = $list; $$->add($dec); }
        // { $$ = $list;     ; slist_push(&($$), $dec); }
    | init-declarator[dec]
        { $$ = new decl::InitDeclarators(); $$->add($dec); }
        // { $$ = slist_new(); slist_push(&($$), $dec); }
    ;
init-declarator 
    : declarator ASSIGN initializer
    | declarator
    ;

declarator 
    : pointer direct-declarator     { $$ = $2; }
    | direct-declarator
    ;

pointer
    : pointer STAR type-qualifier-list-opt
    | STAR type-qualifier-list-opt
    ;

direct-declarator
    : ID
    | LPAR declarator RPAR  { $$ = $2; }
    | direct-declarator LB type-qualifier-list-opt                              RB
    | direct-declarator LB type-qualifier-list-opt        assignment-expression RB
    | direct-declarator LB type-qualifier-list STATIC     assignment-expression RB
    | direct-declarator LB STATIC type-qualifier-list-opt assignment-expression RB
    | direct-declarator LB type-qualifier-list-opt STAR                         RB
    | direct-declarator LPAR parameter-type-list RPAR
    | direct-declarator LPAR identifier-list-opt RPAR
    ;

identifier-list-opt
    : identifier-list-opt COMMA ID
    | ID
    | %empty
    ;

type-qualifier-list-opt
    : type-qualifier-list
    | %empty
    ;
type-qualifier-list
    : type-qualifier-list type-qualifier
    | type-qualifier
    ;

parameter-type-list
    : parameter-list
    | parameter-list COMMA ELLIPSIS
    ;

parameter-list
    : parameter-declaration
    | parameter-list COMMA parameter-declaration
    ;
parameter-declaration
    : declaration-specifiers declarator
    | declaration-specifiers abstract-declarator-opt
    ;


type-name : specifier-qualifier-list abstract-declarator-opt ;

abstract-declarator-opt
    : abstract-declarator
    | %empty
    ;
abstract-declarator
    : pointer
    | pointer direct-abstract-declarator
    | direct-abstract-declarator
    ;

direct-abstract-declarator
    : LPAR abstract-declarator RPAR
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


initializer
    : assignment-expression
    | LCB initializer-list trailing-comma RCB
    ;

initializer-list
    : designation-opt initializer
    | initializer-list COMMA designation-opt initializer
    ;

designation-opt
    : designator-list ASSIGN
    | %empty
    ;

designator-list
    : designator
    | designator-list designator
    ;

designator
    : LB constant-expression RB
    | DOT ID
    ;


function-definition
    : declaration-specifiers declarator declaration-list-opt compound-stmt[body]
        { $$ = new ast::FunctionDefinition($body); }
    ;

declaration-list-opt
    : declaration-list-opt declaration
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

empty-stmt
    : SEMI              { $$ = NULL; }
    ;

labeled-stmt
    : ID COLON stmt { $$ = $3; }
    ;  

goto-stmt
    : GOTO ID SEMI  { $$ = NULL; }
    ;      

compound-stmt
    : LCB {$<ScopeId>$ = open_scope();}[scope] block-list-opt[block] {close_scope();} RCB
        { $$ = $block; $$->set_scope($<ScopeId>scope); }
    ;
block-list-opt
    : %empty                        { $$ = new ast::Block(); }
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

return-stmt
    : RETURN expression-opt SEMI    { $$ = NULL; }
    ;

continue-stmt
    : CONTINUE SEMI     { $$ = NULL; }
    ;
break-stmt
    : BREAK SEMI        { $$ = NULL; }
    ;
case-stmt
    : CASE constant-expression COLON stmt[body]     { $$ = $body; }
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

expr-stmt
    : expression SEMI   { $$ = new ast::ExpressionStmt($1); }
    ;

expression-opt : expression | %empty ;

expression : comma-expression       { $$ = $1; last_expr = $$; } ;

comma-expression
    : assignment-expression
    | expression COMMA assignment-expression
    ;

assignment-expression 
    : conditional-expression
    | unary-expression assignment-operator assignment-expression  { $$ = $3; }
    ;

assignment-operator
    : ASSIGN
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

conditional-expression
    : or-expression
    | or-expression QUEST expression COLON conditional-expression
    ;

or-expression
    : and-expression
    | or-expression OR and-expression
    ;

and-expression
    : bit-or-expression
    | and-expression AND bit-or-expression
    ;

bit-or-expression
    : bit-xor-expression
    | bit-or-expression BTOR bit-xor-expression
    ;

bit-xor-expression
    : bit-and-expression
    | bit-xor-expression BTXOR bit-and-expression
    ;

bit-and-expression
    : equality-expression
    | bit-and-expression AMPER equality-expression
    ;

equality-expression
    : relational-expression
    | equality-expression EQ  relational-expression
    | equality-expression NEQ relational-expression
    ;

relational-expression
    : shift-expression
    | relational-expression LT shift-expression
    | relational-expression GT shift-expression
    | relational-expression LET shift-expression
    | relational-expression GET shift-expression
    ;

shift-expression
    : additive-expression
    | shift-expression LEFT  additive-expression
    | shift-expression RIGHT additive-expression
    ;

additive-expression
    : multiplicative-expression
    | additive-expression PLUS  multiplicative-expression   { $$ = new ast::Plus($1, $3); }
    | additive-expression MINUS multiplicative-expression   { $$ = new ast::Minus($1, $3); }
    ;

multiplicative-expression
    : cast-expression
    | multiplicative-expression STAR cast-expression    { $$ = new ast::Times($1, $3); }
    | multiplicative-expression OVER cast-expression    { $$ = new ast::Over($1, $3); }
    | multiplicative-expression PERC cast-expression
    ;

cast-expression
    : unary-expression
    | LPAR type-name RPAR cast-expression[value]        { $$ = $value; }
    ;

unary-expression
    : postfix-expression
    | PLUSPLUS   unary-expression   { $$ = new ast::PrefixPlusPlus($2); }
    | MINUSMINUS unary-expression   { $$ = new ast::PrefixMinusMinus($2); }
    | AMPER cast-expression         { $$ = $2; }
    | STAR  cast-expression         { $$ = $2; }
    | PLUS  cast-expression         { $$ = $2; }
    | MINUS cast-expression         { $$ = new ast::InvertSignal($2); }
    | BTNOT cast-expression         { $$ = new ast::BitNot($2); }
    | NOT   cast-expression         { $$ = new ast::Not($2); }
    | SIZEOF unary-expression       { $$ = $2; } 
    | SIZEOF LPAR type-name RPAR    { assert(0); }
    // | _Alignof LPAR type-name RPAR
    ;

postfix-expression
    : primary-expression
    | postfix-expression LB expression RB       // %prec LB
    | postfix-expression LPAR argument-expression-list-opt RPAR
    | postfix-expression DOT   ID
    | postfix-expression ARROW ID
    | postfix-expression PLUSPLUS       { $$ = new ast::PrefixPlusPlus($1);  }
    | postfix-expression MINUSMINUS     { $$ = new ast::PrefixMinusMinus($1);  }
    // | ( type-name ) { initializer-list }
    // | ( type-name ) { initializer-list , }
    ;


argument-expression-list-opt : argument-expression-list | %empty ;
argument-expression-list
    : argument-expression-list COMMA assignment-expression
    | assignment-expression
    ;

primary-expression 
    : ID            { $$ = new ast::Variable(*$1); delete $1; }
    | INT_VAL       { $$ = new ast::IntegerValue($1); }
    | REAL_VAL      { $$ = new ast::FloatingValue($1); }
    | CHAR_VAL      { $$ = new ast::CharValue($1); }
    | STR_VAL       { $$ = new ast::StringValue($1); }
    | LPAR expression RPAR      { $$ = $2; }
    // | generic-expression ??
    ;


%%
