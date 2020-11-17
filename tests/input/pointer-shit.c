
int main() {
    int a = 2;
    int b = 7;
    int *pa = &a;
    int **ppa = &pa;
    int *pb = &b;
    **ppa = *pb;
    pa = pb;

    char c = 3;
    char *pc = &c;
    // pa = &c; // ERRO
    // pa = pc; // ERRO
}
