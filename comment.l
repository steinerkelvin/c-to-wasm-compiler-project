%{
#include <stdio.h>

int var = 1;//verifica se está na declaração das variáveis
char* id_atual;/*guarda o id que será modificado a seguir*/

%}

/*max um 0*/
digit 0|([1-9]+[0-9]*) 
/*palavras*/
alpha [a-zA-Z]+
/**/
real (0|([1-9]+[0-9]*))"."[0-9]+

%option yylineno
%option outfile="scanner.c"

%%


"/*"[^*/]*"*/"	{printf("%s\n",yytext); }//comment; aceita qualquer coisa (inclusive nada (*)) entre /* e (*/ ou EOF), com exceção do próprio */ 
"//".*(\\\n.*)*\n	{printf("%s\n",yytext);}//comment; aceita qualquer coisa (inclusive nada (*)) entre // e \n obs.: se o ultimo caractere da linha for um \ (escape), a linha seguinte também é comentada, uma vez que o caracter escapado é o \n, ou seja, ele deixa de ser considerado um \n e passa a ser um "\n", fazendo parte do comentário. façam o teste aqui no próprio editor, colocando um \ no final dessa linha
[ \t\n]	  	  { }
begin         {	
				printf("%d: %s -> BEGIN\n",yylineno,yytext);
				var = 0;
			  }
bool          {printf("%d: %s -> BOOL\n",yylineno,yytext);}
else          {printf("%d: %s -> ELSE\n",yylineno,yytext);}   
end           {printf("%d: %s -> END\n",yylineno,yytext);}   
false         {printf("%d: %s -> FALSE\n",yylineno,yytext);}   
if            {printf("%d: %s -> IF\n",yylineno,yytext);} 
int           {printf("%d: %s -> INT\n",yylineno,yytext);} 
program       {printf("%d: %s -> PROGRAM\n",yylineno,yytext);}
read   		  {printf("%d: %s -> READ\n",yylineno,yytext);}
real          {printf("%d: %s -> REAL\n",yylineno,yytext);}
repeat        {printf("%d: %s -> REPEAT\n",yylineno,yytext);}
string        {printf("%d: %s -> STRING\n",yylineno,yytext);}
then          {printf("%d: %s -> THEN\n",yylineno,yytext);}
true          {printf("%d: %s -> TRUE\n",yylineno,yytext);}
until         {printf("%d: %s -> UNTIL\n",yylineno,yytext);}
var           {printf("%d: %s -> VAR\n",yylineno,yytext);}
write         {printf("%d: %s -> WRITE\n",yylineno,yytext);}
":="          {printf("%d: %s -> ASSIGN\n",yylineno,yytext);}  
"="           {printf("%d: %s -> EQ\n",yylineno,yytext);} 
"<"           {printf("%d: %s -> LT\n",yylineno,yytext);}  
"+"           {printf("%d: %s -> SUM\n",yylineno,yytext);}  
"-"           {printf("%d: %s -> MINUS\n",yylineno,yytext);} 
"*"           {printf("%d: %s -> TIMES\n",yylineno,yytext);} 
"/"           {printf("%d: %s -> DIV\n",yylineno,yytext);}    
"("           {printf("%d: %s -> PARINI\n",yylineno,yytext);}   
")"           {printf("%d: %s -> PAREND\n",yylineno,yytext);} 
";"           {printf("%d: %s -> SEMI\n",yylineno,yytext);} 
{digit}       {printf("%d: %s -> INT_VAL\n",yylineno,yytext);} 
{alpha}       {printf("%d: %s -> ID\n",yylineno,yytext);} 
{real}        {printf("%d: %s -> REAL\n",yylineno,yytext);} 

