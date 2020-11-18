#define DUMP_TOKENS
#include "scanner.cpp"

int yydebug;
YYSTYPE yylval;
YYLTYPE yylloc;

int yylex(void);
int main() {
    yylex();
    return 0;
}
