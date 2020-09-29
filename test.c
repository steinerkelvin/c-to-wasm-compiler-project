#include <stdio.h>

int globalzao = 2;
float b = 1;

int test1();
int test2(void);
int test3(int a, int b);
int test4(int a, int);

int test4(int a, int b) {
    return a + b;
}

//testando\
um \
comentario         \
obscuro

int main() {
    float a = 2, b;
    a == b;
    test4(a, b);
    int c = test4(a, b);
    if (b > 1)
        a = 77;
    else {
        return 1;
    }
    int i = 0;
    - i++;

    while(i < 5)
        i++;

    for(;;)

    for(int a=0,b,c;i<7;i++)
        a++;

    // const int k = 0 + 2;
    switch(0){
        case 0: 
            break;
        // case k + 1:
        //     a++; break;
    } 
    //i++++;

    do i++;
    while(i < 10);
    return 0;
}

/*
testando um comentário 
normal
*/
