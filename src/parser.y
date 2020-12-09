%output "generated_parser.cpp"
%defines "generated_parser.hpp"
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.
%define parse.trace

%define api.value.type union
%locations

%code requires {
#include <cstdint>
#include <string>
#include "strtable.hpp"
#include "types.hpp"
#include "declarations.hpp"
#include "ast.hpp"
}

%{
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "parsing.hpp"
#include "strtable.hpp"
#include "types.hpp"
#include "symtable.hpp"
#include "declarations.hpp"
#include "ast.hpp"
#include "operations.hpp"
#include "global.hpp"

int yylex(void);
void yyerror(char const *s);
%}

%type <ast::Program*> program
%type <ast::Declaration*> function-definition
%type <ast::Block*> declaration

%type <decl::TypeQualifier*> type-qualifier
%type <decl::TypeSpecifier*> type-specifier
%type <decl::TypeQualOrTypeSpecList*> specifier-qualifier-list
%type <size_t> typedef-name
%type <bool> struct-or-union
%type <decl::StructOrUnionSpec*> struct-or-union-spec
%type <decl::EnumSpec*> enum-spec

%type <decl::DeclarationSpecs*> declaration-specifiers
%type <decl::DeclarationSpec*> declaration-specifier
%type <decl::StorageClassSpec*> storage-class-specifier
%type <decl::InitDeclarators*> init-declarator-list
%type <decl::Declarator*> init-declarator
%type <decl::Declarator*> declarator
%type <size_t> pointer
%type <decl::Declarator*> direct-declarator
%type <decl::ParameterDeclarations*> parameter-type-list parameter-list
%type <decl::ParameterDeclaration*> parameter-declaration
%type <decl::AbstractDeclarator*> abstract-declarator-opt abstract-declarator direct-abstract-declarator
%type <ast::Expr*> initializer

%type <ast::Statement*> stmt
%type <ast::Block*> compound-stmt block-list-opt
%type <ast::Statement*> block-item
%type <ast::Statement*> empty-stmt
// %type <ast::Statement*> labeled-stmt
// %type <ast::Statement*> goto-stmt
%type <ast::Statement*> if-stmt
%type <ast::Statement*> switch-stmt
%type <ast::Statement*> case-stmt
%type <ast::Statement*> default-stmt
%type <ast::Statement*> break-stmt
%type <ast::Statement*> continue-stmt
%type <ast::Statement*> return-stmt
%type <ast::Statement*> while-stmt
%type <ast::Statement*> do-while-stmt
%type <ast::Statement*> for-stmt
%type <ast::ExprStmt*> expr-stmt

%type <ast::Expr*> expr-opt
%type <ast::Statement*> expr-opt-stmt
%type <ast::Statement*> for-init

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
%type <ast::Exprs*> argument-expression-list-opt argument-expression-list

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

%token <char> CHAR_VAL <uint64_t> INT_VAL <double> REAL_VAL
%token <StrRef> STR_VAL
%token <std::string*> ID <size_t> TYPENAME

%precedence RPAR
%precedence ELSE

%%

all : program { root = $1; } ;

program
    : %empty                        { $$ = new ast::Program(); }
    | program function-definition   { $$ = $1; $$->add($2); }
    | program declaration
        {
            $$ = $1;
            auto init_block = $2;
            if (!init_block->get_children().empty()) {
                auto init_node = new ast::Initialization(init_block);
                $$->add(init_node);
            }
        }
    ;

declaration
    : declaration-specifiers SEMI
        {
            $$ = new ast::Block();
        }
    | declaration-specifiers[specs] init-declarator-list[inits] SEMI
        {
            auto inits = declare(*$specs, *$inits);
            delete $specs;
            delete $inits;
            $$ = new ast::Block();
            for (auto init_expr : inits) {
                auto stmt = new ast::ExprStmt(init_expr);
                $$->add(stmt);
            }
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
    | type-specifier            { $$ = new decl::TypeDeclSpec($1); $$->merge_pos_from($1); }
    | type-qualifier            { $$ = new decl::TypeDeclSpec($1); $$->merge_pos_from($1); }
    // | function-specifier
    // | alignment-specifier
    ;

storage-class-specifier
    : TYPEDEF   { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::TYPEDEF);  $$->set_pos(@$); }
    | EXTERN    { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::EXTERN);   $$->set_pos(@$); }
    | STATIC    { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::STATIC);   $$->set_pos(@$); }
    | AUTO      { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::AUTO);     $$->set_pos(@$); }
    | REGISTER  { $$ = new decl::StorageClassSpec(decl::StorageClassSpec::REGISTER); $$->set_pos(@$); }
    ;

type-specifier
    : VOID      { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::VOID);    $$->set_pos(@$); }
    | CHAR      { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::CHAR);    $$->set_pos(@$); }
    | SHORT     { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::SHORT);   $$->set_pos(@$); }
    | INT       { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::INT);     $$->set_pos(@$); }
    | LONG      { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::LONG);    $$->set_pos(@$); }
    | FLOAT     { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::FLOAT);   $$->set_pos(@$); }
    | DOUBLE    { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::DOUBLE);  $$->set_pos(@$); }
    | SIGNED    { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::SIGNED);  $$->set_pos(@$); }
    | UNSIGNED  { $$ = new decl::SimpleTypeSpec(decl::SimpleTypeSpec::UNSIGNED);$$->set_pos(@$); }
    | struct-or-union-spec  { $$ = $1; }
    | enum-spec             { $$ = $1; }
    | typedef-name          { $$ = new decl::TypedefName($1); $$->set_pos(@$); }
    ;

typedef-name : TYPENAME ;

struct-or-union-spec
    : struct-or-union ID                                    { $$ = new decl::StructOrUnionSpec($1); $$->set_pos(@$); }
    | struct-or-union ID LCB struct-declaration-list RCB    { $$ = new decl::StructOrUnionSpec($1); $$->set_pos(@$); }
    | struct-or-union    LCB struct-declaration-list RCB    { $$ = new decl::StructOrUnionSpec($1); $$->set_pos(@$); }
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
    : type-specifier    { $$ = new decl::TypeQualOrTypeSpecList(); $$->add($1); }
    | type-qualifier    { $$ = new decl::TypeQualOrTypeSpecList(); $$->add($1); }
    | specifier-qualifier-list type-specifier       { $$ = $1; $$->add($2); }
    | specifier-qualifier-list type-qualifier       { $$ = $1; $$->add($2); }
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
    : ENUM ID                                           { $$ = new decl::EnumSpec(); }
    | ENUM ID LCB enumerator-list trailing-comma RCB    { $$ = new decl::EnumSpec(); }
    | ENUM    LCB enumerator-list trailing-comma RCB    { $$ = new decl::EnumSpec(); }
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
    : CONST     { $$ = new decl::TypeQualifier(decl::TypeQualifier::CONST);    }
    | RESTRICT  { $$ = new decl::TypeQualifier(decl::TypeQualifier::RESTRICT); }
    | VOLATILE  { $$ = new decl::TypeQualifier(decl::TypeQualifier::VOLATILE); }
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

// TODO

init-declarator-list
    : init-declarator[dec]
        { $$ = new decl::InitDeclarators(); $$->add($dec); }
    | init-declarator-list[list] COMMA init-declarator[dec]
        { $$ = $list; $$->add($dec); }
    ;
init-declarator 
    : declarator ASSIGN initializer { $$ = $1; $$->set_init($3); }
    | declarator
    ;

declarator
    : pointer direct-declarator     { $$ = $2; $$->add(decl::pointer_type_builder($1)); }
    | direct-declarator
    ;

// Apenas conta o número de estrelinhas
pointer
    : pointer STAR type-qualifier-list-opt  { $$++; }
    | STAR type-qualifier-list-opt          { $$ = 1; }
    ;

direct-declarator
    : ID                    { $$ = new decl::Declarator(*$1); delete $1; }
    | LPAR declarator RPAR  { $$ = $2; }
    | direct-declarator LB type-qualifier-list-opt        assignment-expression[exp] RB     { $$ = $1; $$->add(decl::vector_type_builder($exp, @exp)); }
    | direct-declarator LB type-qualifier-list-opt                                   RB     { $$ = $1; $$->add(decl::pointer_type_builder(1)); }
    // | direct-declarator LB type-qualifier-list STATIC     assignment-expression[exp] RB     { $$ = $1; $$->add(decl::vector_type_builder($exp, @exp)); }
    // | direct-declarator LB STATIC type-qualifier-list-opt assignment-expression[exp] RB     { $$ = $1; $$->add(decl::vector_type_builder($exp, @exp)); }
    // | direct-declarator LB type-qualifier-list-opt STAR                              RB
    // | direct-declarator LPAR identifier-list-opt RPAR
    | direct-declarator LPAR                     RPAR       { $$ = $1; $$->add(decl::function_type_builder(NULL)); }
    | direct-declarator LPAR parameter-type-list RPAR       { $$ = $1; $$->add(decl::function_type_builder($3)); delete $3; }
    ;

// identifier-list-opt
//     : identifier-list-opt COMMA ID
//     | ID
//     | %empty
//     ;

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
    | parameter-list COMMA ELLIPSIS     { fprintf(stderr, "NOT IMPLEMENTED ERROR (%d): variadic functions not implemented.\n", yylineno); exit(1); }
    ;

parameter-list
    : parameter-declaration                         { $$ = new decl::ParameterDeclarations(); $$->add($1); }
    | parameter-list COMMA parameter-declaration    { $$ = $1; $$->add($3); }
    ;
parameter-declaration
    : declaration-specifiers declarator                 { $$ = new decl::ParameterDeclaration($1, $2); }
    | declaration-specifiers abstract-declarator-opt    { $$ = decl::ParameterDeclaration::from($1, $2); }
    ;


type-name : specifier-qualifier-list abstract-declarator-opt ;

abstract-declarator-opt
    : %empty                { $$ = new decl::AbstractDeclarator(); }
    | abstract-declarator
    ;
abstract-declarator
    : pointer                               { $$ = new decl::AbstractDeclarator(); $$->add(decl::pointer_type_builder($1)); }
    | pointer direct-abstract-declarator    { $$ = $2;                             $$->add(decl::pointer_type_builder($1)); }
    | direct-abstract-declarator
    ;

direct-abstract-declarator
    : LPAR abstract-declarator RPAR     { $$ = $2; }
    |                            LB type-qualifier-list-opt assignment-expression[exp] RB   { $$ = new decl::AbstractDeclarator(); $$->add(decl::vector_type_builder($exp, @exp)); }
    | direct-abstract-declarator LB type-qualifier-list-opt assignment-expression[exp] RB   { $$ = $1;                             $$->add(decl::vector_type_builder($exp, @exp)); }
    // |                            LB type-qualifier-list-opt                            RB
    // | direct-abstract-declarator LB type-qualifier-list-opt                            RB
    // |                            LB STATIC type-qualifier-list-opt assignment-expression RB
    // | direct-abstract-declarator LB STATIC type-qualifier-list-opt assignment-expression RB
    // |                            LB type-qualifier-list STATIC     assignment-expression RB
    // | direct-abstract-declarator LB type-qualifier-list STATIC     assignment-expression RB
    // |                            LB STAR RB                  // Conflito (mas sinceramente, me parece inútil)
    // | direct-abstract-declarator LB STAR RB                  // Conflito (mas sinceramente, me parece inútil)
    |                            LPAR                             RPAR      { $$ = new decl::AbstractDeclarator(); $$->add(decl::function_type_builder(NULL)); }
    | direct-abstract-declarator LPAR                             RPAR      { $$ = $1;                             $$->add(decl::function_type_builder(NULL)); }
    |                            LPAR parameter-type-list[params] RPAR      { $$ = new decl::AbstractDeclarator(); $$->add(decl::function_type_builder($params)); delete $params; }
    | direct-abstract-declarator LPAR parameter-type-list[params] RPAR      { $$ = $1;                             $$->add(decl::function_type_builder($params)); delete $params; }
    ;


initializer     // TODO assignment
    : assignment-expression
    // | LCB initializer-list trailing-comma RCB   { $$ = new ast::Expr(); }
    ;

// initializer-list
//     : designation-opt initializer
//     | initializer-list COMMA designation-opt initializer
//     ;

// designation-opt
//     : designator-list ASSIGN
//     | %empty
//     ;

// designator-list
//     : designator
//     | designator-list designator
//     ;

// designator
//     : LB constant-expression RB
//     | DOT ID
//     ;


function-definition
    :   declaration-specifiers[specs]
        declarator[decl]
        // declaration-list-opt     // obscure arcane syntax
        LCB
        <std::pair<symtb::VarRef,ScopeId>*>{
            // Declares the function in the outer scope,
            // and opens a new scope
            $$ = decl::declare_function($specs, $decl);
        }[func_ref_scope]
        block-list-opt[body]
        { symtb::close_scope(); }
        RCB
        {
            auto [func_ref, scope_id] = *$func_ref_scope;
            $body->set_scope(scope_id);
            $$ = new ast::FuncDef($body, func_ref);
            delete $func_ref_scope;
        }
    ;

// declaration-list-opt
//     : declaration-list-opt declaration
//     | %empty
//     ;


stmt
    : empty-stmt
    | compound-stmt     { $$ = $1; }
    // | labeled-stmt
    // | goto-stmt
    | return-stmt
    | if-stmt
    | while-stmt
    | do-while-stmt
    | for-stmt
    | break-stmt
    | continue-stmt
    | switch-stmt
    | case-stmt
    | default-stmt
    | expr-stmt         { $$ = $1; }
    ;

empty-stmt
    : SEMI              { $$ = new ast::EmptyStmt(); }
    ;

// labeled-stmt
//     : ID COLON stmt { $$ = $3; }
//     ;
// goto-stmt
//     : GOTO ID SEMI  { $$ = NULL; }
//     ;

compound-stmt
    :   LCB
        <ScopeId>{ $$ = symtb::open_scope(); }[scopeid]
        block-list-opt[block]
        { symtb::close_scope(); }
        RCB
            { $$ = $block; $$->set_scope($scopeid); }
    ;
block-list-opt
    : %empty                        { $$ = new ast::Block(); }
    | block-list-opt block-item     { $$ = $1; $$->add($2); }
    ;
block-item
    : declaration                   { $$ = $1; }
    | stmt
    ;

if-stmt
    : IF LPAR expression[expr] RPAR stmt[body]                  { auto cond = ops::check_bool($expr, @expr); $$ = new ast::IfStmt    (cond, $body);         }
    | IF LPAR expression[expr] RPAR stmt[body] ELSE stmt[block] { auto cond = ops::check_bool($expr, @expr); $$ = new ast::IfElseStmt(cond, $body, $block); }
    ;

return-stmt
    : RETURN SEMI               { $$ = new ast::Return(); }
    | RETURN expression SEMI    { $$ = ops::return_value($2, @$); }
    ;

continue-stmt
    : CONTINUE SEMI     { $$ = new ast::Continue(); }
    ;
break-stmt
    : BREAK SEMI        { $$ = new ast::Break(); }
    ;
case-stmt
    : CASE constant-expression COLON stmt[body]     { $$ = $body; } // TODO
    ;
default-stmt
    : DEFAULT COLON stmt[body]                      { $$ = $body; } // TODO
    ;

while-stmt
    : WHILE LPAR expression[expr] RPAR stmt[body]         { auto cond = ops::check_bool($expr, @expr); $$ = new ast::WhileStmt(cond, $body); }
    ;

do-while-stmt
    : DO stmt[body] WHILE LPAR expression[expr] RPAR SEMI   { auto cond = ops::check_bool($expr, @expr); $$ = new ast::DoWhileStmt(cond, $body); }
    ;

for-stmt
    : FOR LPAR for-init[init] expr-opt[cond] SEMI expr-opt-stmt[incr] RPAR stmt[body]
        {
            using opt_expr_t = std::optional<ast::Expr*>;
            std::optional<ast::Expr*> cond_opt =
                $cond
                    ? opt_expr_t(ops::check_bool($cond, @cond))
                    : opt_expr_t();
            $$ = new ast::ForStmt($init, cond_opt, $incr, $body);
        }
    ;

for-init
    : expr-opt-stmt SEMI
    // | declaration
    ;

switch-stmt
    : SWITCH LPAR expression RPAR stmt[body]    { $$ = $body; }
    ;

expr-stmt
    : expression SEMI   { $$ = new ast::ExprStmt($1); }
    ;

expr-opt-stmt
    : expr-opt
        {
            auto expr = $1;
            if (expr) {
                $$ = new ast::ExprStmt(expr);
            } else {
                $$ = new ast::EmptyStmt();
            }
        }
    ;

expr-opt
    : expression
    | %empty    { $$ = NULL; }
    ;

expression : comma-expression       { $$ = $1; last_expr = $$; } ;

comma-expression
    : assignment-expression
    | expression COMMA assignment-expression
    ;

assignment-expression
    : conditional-expression
    | unary-expression ASSIGN  assignment-expression  { $$ = ops::unify_assign($1, $3, @$); }
    // // TODO ?
    // | unary-expression STARASS assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"*="); $$ = $3; }
    // | unary-expression OVERASS assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"/="); $$ = $3; }
    // | unary-expression MODASS  assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"%="); $$ = $3; }
    // | unary-expression PLUSASS assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"+="); $$ = $3; }
    // | unary-expression MINASS  assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"-="); $$ = $3; }
    // | unary-expression SLASS   assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"<<="); $$ = $3; }
    // | unary-expression SRASS   assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),">>="); $$ = $3; }
    // | unary-expression ANDASS  assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"&="); $$ = $3; }
    // | unary-expression XORASS  assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"^="); $$ = $3; }
    // | unary-expression ORASS   assignment-expression  { ops::assign_verify($1->get_type(),$3->get_type(),"|="); $$ = $3; }
    ;

constant-expression : conditional-expression ;

conditional-expression
    : or-expression
    // | or-expression QUEST expression COLON conditional-expression    // TODO
    ;

or-expression
    : and-expression
    | or-expression OR and-expression      { $$ = ops::unify_logic($1, $3, ast::And::builder, "||", @$); }
    ;

and-expression
    : bit-or-expression
    | and-expression AND bit-or-expression { $$ = ops::unify_logic($1, $3, ast::Or::builder,  "&&", @$); }
    ;

bit-or-expression
    : bit-xor-expression
    | bit-or-expression BTOR bit-xor-expression     { res_type = ops::unify_bitwise_old($1->get_type(), $3->get_type(), "|"); $$->set_pos(@$); }
    ;

bit-xor-expression
    : bit-and-expression
    | bit-xor-expression BTXOR bit-and-expression   { res_type = ops::unify_bitwise_old($1->get_type(), $3->get_type(), "^"); $$->set_pos(@$); }
    ;

bit-and-expression
    : equality-expression
    | bit-and-expression AMPER equality-expression      { res_type = ops::unify_bitwise_old($1->get_type(), $3->get_type(), "&"); $$->set_pos(@$); }
    ;

equality-expression
    : relational-expression
    | equality-expression EQ  relational-expression     { $$ = ops::unify_comp($1, $3, ast::Equal   ::builder, "==" , @$); }
    | equality-expression NEQ relational-expression     { $$ = ops::unify_comp($1, $3, ast::NotEqual::builder, "!=" , @$); }
    ;

relational-expression
    : shift-expression
    | relational-expression LT shift-expression     { $$ = ops::unify_comp($1, $3, ast::Less        ::builder, "<" , @$); }
    | relational-expression GT shift-expression     { $$ = ops::unify_comp($1, $3, ast::Greater     ::builder, ">" , @$); }
    | relational-expression LET shift-expression    { $$ = ops::unify_comp($1, $3, ast::LessEqual   ::builder, "<=", @$); }
    | relational-expression GET shift-expression    { $$ = ops::unify_comp($1, $3, ast::GreaterEqual::builder, ">=", @$); }
    ;

shift-expression
    : additive-expression
    | shift-expression LEFT  additive-expression    { res_type = ops::unify_bitwise_old($1->get_type(), $3->get_type(), "<<"); $$->set_pos(@$); }
    | shift-expression RIGHT additive-expression    { res_type = ops::unify_bitwise_old($1->get_type(), $3->get_type(), ">>"); $$->set_pos(@$); }
    ;

additive-expression
    : multiplicative-expression
    | additive-expression PLUS  multiplicative-expression   { $$ = ops::unify_additive($1, $3, ast::Plus ::builder, "+", @$); }
    | additive-expression MINUS multiplicative-expression   { $$ = ops::unify_additive($1, $3, ast::Minus::builder, "-", @$); }
    ;

multiplicative-expression
    : cast-expression
    | multiplicative-expression STAR cast-expression    { $$ = ops::unify_multi($1, $3, ast::Times::builder, "*", @$); }
    | multiplicative-expression OVER cast-expression    { $$ = ops::unify_multi($1, $3, ast::Over ::builder, "/", @$); }
    | multiplicative-expression PERC cast-expression    { $$ = ops::unify_multi($1, $3, ast::Mod  ::builder, "%", @$); }
    ;

cast-expression
    : unary-expression
    | LPAR type-name RPAR cast-expression[value]        { $$ = $value; }
    ;

unary-expression
    : postfix-expression
    | PLUSPLUS   unary-expression   { $$ = ops::make_unary($2, ast::PrefixPlusPlus  ::builder, "++", @$); }
    | MINUSMINUS unary-expression   { $$ = ops::make_unary($2, ast::PrefixMinusMinus::builder, "--", @$); }
    | AMPER cast-expression         { $$ = ops::address_of($2, @$);   }
    | STAR  cast-expression         { $$ = ops::derreference($2, @$); }
    | PLUS  cast-expression         { $$ = ops::make_unary($2, ast::Expr            ::retsame, "+" , @$); }
    | MINUS cast-expression         { $$ = ops::make_unary($2, ast::InvertSignal    ::builder, "-" , @$); }
    | BTNOT cast-expression         { $$ = ops::make_btnot($2, ast::BitNot          ::builder, "~" , @$); }
    | NOT   cast-expression         { $$ = ops::make_unary($2, ast::Not             ::builder, "!" , @$); }
    | SIZEOF unary-expression       { abort(); }
    | SIZEOF LPAR type-name RPAR    { abort(); }
    // | _Alignof LPAR type-name RPAR
    ;

postfix-expression
    : primary-expression
    | postfix-expression[value] LB expression[index] RB                         { $$ = ops::index_access($value, $index, @$); }
    | postfix-expression[value] LPAR argument-expression-list-opt[args] RPAR    { $$ = ops::function_call($value, $args, @$); }
    | postfix-expression DOT   ID
    | postfix-expression ARROW ID
    | postfix-expression PLUSPLUS       { $$ = ops::make_unary($1, ast::PrefixPlusPlus  ::builder, "++", @$); }
    | postfix-expression MINUSMINUS     { $$ = ops::make_unary($1, ast::PrefixMinusMinus::builder, "--", @$); }
    // | ( type-name ) { initializer-list }
    // | ( type-name ) { initializer-list , }
    ;

argument-expression-list-opt
    : %empty        { $$ = NULL; }
    | argument-expression-list
    ;
argument-expression-list
    : assignment-expression
        { $$ = new ast::Exprs(); $$->add($1); }
    | argument-expression-list COMMA assignment-expression
        { $$ = $1; $$->add($3); }
    ;

primary-expression
    : ID[name] {
            auto ref = parsing::get_var(*($name));
            $$ = new ast::Variable(ref);
            $$->set_type(ref.get().type);
            $$->set_pos(@$);
            delete $1;
        }
    | INT_VAL       { $$ = new ast::IntegerValue($1);  $$->set_pos(@$); }
    | REAL_VAL      { $$ = new ast::RealValue($1); $$->set_pos(@$); }
    | CHAR_VAL      { $$ = new ast::CharValue($1);     $$->set_pos(@$); }
    | STR_VAL       { $$ = new ast::StringValue($1);   $$->set_pos(@$); }
    | LPAR expression RPAR      { $$ = $2; }
    // | generic-expression ??
    ;


%%
