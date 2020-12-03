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
    println_real(r);
}
