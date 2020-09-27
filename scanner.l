%option outfile="scanner.c"
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

/*max um 0*/
digit 0|([1-9]+[0-9]*) 
/*palavras*/
alpha [a-zA-Z]+

real (0|([1-9]+[0-9]*))"."[0-9]+

%%

[ \t\n]	  	  				{ }
"/*"[^*/]*"*/"	{ }//comment; aceita qualquer coisa (inclusive nada (*)) entre /* e (*/ ou EOF), com exceção do próprio */ 
"//".*(\\\n.*)*\n			{printf("%s",yytext); }//comment; aceita qualquer coisa (inclusive nada (*)) entre // e \n obs.: se o ultimo caractere da linha for um \ (escape), a linha seguinte também é comentada, uma vez que o caracter escapado é o \n, ou seja, ele deixa de ser considerado um \n e passa a ser um "\n", fazendo parte do comentário. façam o teste aqui no próprio editor, colocando um \ no final dessa linha
auto						{return AUTO;}
break						{return BREAK;}
case						{return CASE;}
char						{return CHAR;}
const						{return CONST;}
continue					{return CONTINUE;}
default						{return DEFAULT;}
do							{return DO;}
double						{return DOUBLE;}
else						{return ELSE;}
enum						{return ENUM;}
extern						{return EXTERN;}
float						{return FLOAT;}
for							{return FOR;}
goto						{return GOTO;}
if							{return IF;}
int							{return INT;}
long						{return LONG;}
register					{return REGISTER;}
return						{return RETURN;}
short						{return SHORT;}
signed						{return SIGNED;}
sizeof						{return SIZEOF;}
static						{return STATIC;}
struct						{return STRUCT;}
switch						{return SWITCH;}
typedef						{return TYPEDEF;}
union						{return UNION;}
unsigned					{return UNSIGNED;}
void						{return VOID;}
volatile					{return VOLATILE;}
while						{return WHILE;}
,							{return COMMA;}
"="     	     			{return ASSIGN;}  
"=="    	       			{return EQ;} 
"<"     	      			{return LT;} 
">"							{return BT;} 
"<="						{return LET;}
">="						{return BET;}
"+"     	      			{return PLUS;}  
"-"     	      			{return MINUS;} 
"*"     	      			{return TIMES;} 
"/"    		      			{return OVER;}    
"("     	      			{return LPAR;} 
")"     	      			{return RPAR;} 
"{"  						{return LCB;}
"}"							{return RCB;}
";"     	      			{return SEMI;} 
{digit} 	      			{return INT_VAL;} 
{alpha} 	      			{return ID;} 
{real}  	      			{return REAL_VAL;} 
\".*\"  	 				{return STR_VAL;}
.       					{printf("LEXICAL ERROR: Unknown symbol %s\n", yytext);
        	 				 exit(EXIT_FAILURE); }


