
char func_char(char a) { return a; }
int func_int(int a) { return a; }
float func_real(float a) { return a; }

void func_void() {}

int main()
{
    char c = 1;
    int i = 2;
    float r = 3.0;
    func_char(i);
    func_char(r);
    func_int(c);
    func_int(r);
    func_real(c);
    func_real(i);

    // func_char(func_void()); // ERRO
    // func_int(func_void()); // ERRO
    // func_real(func_void()); // ERRO
}
