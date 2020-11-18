#define DUMP_TOKENS
#include "scanner.cpp"
int yylex(void);
int main() {
    yylex();
    return 0;
}
