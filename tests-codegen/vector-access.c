void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

int main() {
    int vec[5];
    int len = 5;

    int i = 0;
    for (i = 0; i < len; i = i + 1) {
        int n = i + 2;
        vec[i] = n * n;
    }
    for (i = len-1; i >= 0; i = i - 1) {
        println_int(vec[i]);
    }

    return 0;
}
