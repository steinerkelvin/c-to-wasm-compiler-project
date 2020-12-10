void println(const char*);
void println_int(int);

int f1(int x)
{
    println("f1");
    return x;
}

int f2(int x)
{
    println("f2");
    return x;
}

int f3(int x)
{
    println("f3");
    return x;
}

void test()
{
    println("AND 1");
    int r1 = f1(1) && f2(0) && f3(0);
    println_int(r1);
    println("AND 2");
    int r2 = f1(1) && f2(1) && f3(1);
    println_int(r2);

    println("OR 1");
    int r3 = f1(0) || f2(1) || f3(1);
    println_int(r3);
    println("OR 2");
    int r4 = f1(0) || f2(0) || f3(0);
    println_int(r4);
}

int main() {
    test();
    return 0;
}
