
int test(int a, int b) { return a + b; }

int main() {
    int a = 2;
    int b = 7;
    // test(a); // ERRO
    test(a, b);
    int *pa = &a;
    int **ppa = &pa;
    int *pb = &b;
    // **ppa = *pb; // TODO

    char c = 3;
    // pa = &c; // ERRO
}
