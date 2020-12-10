void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

void test()
{
    int i;
    for (i = 0; i < 3; i = i + 1) {
        print_int_pad(i, 4);
        if (i == 0) {
            println("top");
        } else if (i == 1) {
            println("middle");
        } else {
            println("bottom");
        }
    }
}

int main()
{
    test();
    return 0;
}
