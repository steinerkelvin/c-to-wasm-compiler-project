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
    return n;
}

int main()
{
    int x = fac(6);
    println_int(x);
    return 0;
}
