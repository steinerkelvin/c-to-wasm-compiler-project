
int main() {
    int a = 2;
    int b = 7;
    int *pa = &a;
    int **ppa = &pa;
    int *pb = &b;
    // **ppa = *pb; // TODO

    char c = 3;
    // pa = &c; // ERRO
}
