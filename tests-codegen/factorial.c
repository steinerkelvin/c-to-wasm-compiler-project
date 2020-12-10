void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

int fac(int n)
{
    if (n <= 1) {
        return 1;
    }
    return n * fac(n - 1);
}

int fac_iter(int n)
{
    int acc = 1;
    int i;
    for (i = 2; i <= n; i = i+1) {
        acc = acc * i;
    }
    return acc;
}

int main()
{
    int x = fac(6);
    int y = fac_iter(6);
    println_int(x);
    println_int(y);
    println_int(x == y);
    return 0;
}
