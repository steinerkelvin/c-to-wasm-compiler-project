#include <stdio.h>

int globalzao = 2;
float b = 1;

typedef unsigned int uint;

int test1();
int test2(void);
int test3(int a, int b);
int test4(int a, int);

int test4(int a, int b) { return a + b; }

int test5(int, int);
int test5(a, b) { return a - b; }

// abstract-declarator
int test6(int *, int *);
int test7(int *[8]);

int test8(int (*cb[])());
int test8(int (*cb[2])()){
    return cb[0]();
}

int voidp_to_int(void *p) {
    int *ip = (int *)p;
    return *ip;
}

void varargs(int, ...);
void varargs(int dang, ...) {}

//testando\
um \
comentario		 \
obscuro

typedef struct ponto {
    double x;
    double y;
} ponto_t;

int main() {
    char *p = "Este programa não imprime \"Hello World\"";

    ponto_t ponto = {.x = 10.0, .y = 0.0};
    ponto.y = 20;

    int vec[4][4] = {{0}};
    - vec[1][1]++;

    char k = 'A';
    k = '\0';
    k = '\n';
    k = '\'';

    int a = 2, b;
    const int _01;

    int *ap = &a;
    *ap = 7;

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
    case 1:;
        break;
    }
    }
    // i++++;

    do
        i++;
    while (i < 10);

    int flag = 0;
start:
    if (flag)
        goto end;
    flag = 1;
end:
    ;

    return 0;
}

/*
testando um comentário
normal
*/
