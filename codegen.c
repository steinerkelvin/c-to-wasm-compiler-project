void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

int global;

int main()
{
    global = 1;
    int x = 1;
    int y = 2;
    global = x + y;
    println_int(global);

    float r = 4.2;
    if (global) {
        r = 77.7;
    }
    println_real(r);

    int num = 12;
    while (num) {
        num = num - 1;
        if (num % 2) {
            continue;
        }
        if (num == 4) {
            while (1) {
                break;
            }
            println_int(777);
            break;
        }
        println_int(num);
    }

    int i;
    for (i = 0; i < 12; i = i + 1) {
        if (i % 3 != 0) {
            continue;
        }
        print_int_pad(i, 8);
    }
    _ln();

    println("FATORIAL");
    int acc = 1;
    for (i = 1; i <= 6; i = i + 1) {
        acc = acc * i;
        print_int_pad(acc, 0-6);
    }
    _ln();
}
