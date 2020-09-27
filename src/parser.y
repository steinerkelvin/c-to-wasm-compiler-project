%output "parser.c"          // File name of generated parser.
%defines "parser.h"         // Produces a 'parser.h'
%define parse.error verbose // Give proper messages when a syntax error is found.
%define parse.lac full      // Enable LAC to improve syntax error handling.

%{
#include <stdio.h>
int yylex(void);
void yyerror(char const *s);
%}

%token AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INT LONG REGISTER RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE COMMA ASSIGN EQ LT BT LET BET PLUS MINUS TIMES OVER LPAR RPAR SEMI INT_VAL REAL_VAL ID STR_VAL LCB RCB
%left EQ LT
%left PLUS MINUS// Ops associativos a esquerda.
%left TIMES OVER// Mais para baixo, maior precedencia.
%precedence UMINUS// Menos unario mais precedencia que binario.
%precedence RPAR
%precedence ELSE

//FALTA FAZER AS PRECEDÊNCIAS E ASSOCIATIVIDADE

%%
program: header funcsect;//estrutura do programa; PRECISA ANALISAR: include, define, ifndef, end, etc.

header: %empty | header vardecllist | header funcdec;//pode declarar variáveis e funções a qualquer momento, alternadamente ou em conjunto;
//==================================IMPORTANTE: PRECISA FAZER A REGRA DO STRUCT================================
funcdec: %empty | funcdec func;//declaração de funções
func: typespec ID LPAR arglist RPAR SEMI;//declaração de uma função
typespec: VOID | DOUBLE | INT | CHAR | FLOAT | ID; //CONST, SHORT, LONG, STATIC, UNSIGNED, *POINTER*, ENUM, EXTERN, SIGNED(?), VOLATILE(?)
arglist: arglist COMMA arglist | arg;//lista de argumentos de uma declaração de função
arg: %empty | typespec ID;//argumento para qualquer função (declaração ou definição); VER CASO DO VOID SEM ID (PODE TER MAIS DE UM VOID?)

funcsect: %empty | funcsect funcdef | funcsect vardecllist;//pode declarar variáveis e definir funções a qualquer momento, alternadamente ou em conjunto
funcdef: typespec ID LPAR arglist RPAR LCB stmtlist RCB; //definição de função

//program: PROGRAM ID SEMI varssect stmtsect;
//varssect: VAR optvardecl;
//optvardecl: %empty | vardecllist;
vardecllist: vardecllist vardecl | vardecl | %empty;//declaração de uma ou mais variáveis
vardecl: typespec ID SEMI | typespec assignstmt;//declaração ou definição

//================================= REVISEI ATÉ AQUI E PARECE OK (-Allan) =======================================
//stmtsect: BEGIN_ stmtlist END;
stmtlist: %empty | stmtlist stmt | stmt;
stmt: ifstmt | assignstmt | vardecllist | funccall | return;//while, switch, do-while, for
funccall: funccall ID LPAR paramlist RPAR SEMI | %empty;//chamada de função;
paramlist: paramlist COMMA paramlist | expr | funccall | %empty;//lista de parâmetros ou apenas um parâmetro ou nenhum; pode ser nova chamada de função; PRECISA REVER ISSO AQUI
return: RETURN ret SEMI;
ret: ID | expr | %empty;
ifstmt: IF LPAR expr RPAR stmtlist | IF LPAR expr RPAR stmtlist ELSE stmtlist;//precisa fazer, bem como o while, switch, do-while, for
//repeatstmt: REPEAT stmtlist UNTIL expr;
assignstmt: ID ASSIGN expr SEMI;
//readstmt: READ ID SEMI;
//writestmt: WRITE expr SEMI;
expr: expr LT expr//falta o pow e provavelmente outras coisas
| expr BT expr
| expr LET expr
| expr BET expr
| expr EQ expr
| expr PLUS expr
| expr MINUS expr
| expr TIMES expr
| expr OVER expr
| MINUS expr %prec UMINUS
| LPAR expr RPAR
| INT_VAL
| REAL_VAL
| STR_VAL
| ID;

%%

int main(void) {
    if (yyparse() == 0) printf("PARSE SUCCESSFUL!\n");
    else                printf("PARSE FAILED!\n");
    return 0;
}


