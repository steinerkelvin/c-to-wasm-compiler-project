
int factorial(int n){
    if (n < 1) {
        return 1;
    }
    return n * factorial(n-1);
}

int main() {
    int fac5 = factorial(5);
    return fac5;
}
