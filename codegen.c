void println_int(int);
void println_real(float);

int global;

int main()
{
    global = 1;
    int x;
    int y;
    x = 1;
    y = 2;
    global = x + y;
    println_int(global);

    float r;
    r = 4.2;
    if (global) {
        r = 77.7;
    }
    println_real(r);

    int num;
    num = 3;
    while (num) {
        println_int(num);
        num = num - 1;
    }
}
