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
    num = 12;
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
    for (i = 0; ; i = i + 1) {
        if (i % 3 == 0) {
            continue;
        }
        println_int(i);
        if (i == 10) {
            break;
        }
    }
}
