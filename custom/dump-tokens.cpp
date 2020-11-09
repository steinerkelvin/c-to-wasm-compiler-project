#define DUMP_TOKENS
#include "../src/scanner.cpp"
int yylex(void);
int main() {
    yylex();
    return 0;
}
