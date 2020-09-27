#include <stdio.h>

int globalzao = 2;

int test1();
int test2(void);
int test3(int a, int b);
int test4(int a, int);

int test4(int a, int b) {
    return 0;
}

int main() {
    float a = 2, b;
    b = 3 + 7.0;
    if (b > 1)
        a = 77;
    else {
        return 1;
    }
    return 0;
}
