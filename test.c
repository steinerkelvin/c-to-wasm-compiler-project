#include <stdio.h>

int globalzao = 2;
float b = 1;

typedef unsigned int uint;

int test1();
int test2(void);
int test3(int a, int b);
int test4(int a, int);

int test4(int a, int b) { return a + b; }

//testando\
um \
comentario		 \
obscuro

int main() {
    int a = 2, b;
    // char d = 'd';  // TODO PROBLEMA COM '
    const int _01;

    a == b;

    // test3(a, b);

    int c = test4(a, b);
    if (b > 1) {
        a = 77;
    } else
        // return 1;
        a = 100;

    int e = 1;
    int j = 2;
    int v[3] = {e, j, 3};

    int i = 0;
    -i++;

    while (i < 5)
        i++;

    // for(;;)

    for (int i = 0, b, c; i < 7; i++)
        // teste
        a++;

    printf("%d\n", a);

    switch (a) {
    case (107): {
        printf("%d\n", a);
        case 1: ; break;
    }
    }
    // i++++;

    do
        i++;
    while (i < 10);
    return 0;
}

/*
testando um comentário
normal
*/
