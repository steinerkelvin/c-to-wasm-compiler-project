int readln(char*, int);
void print(const char*);
void println(const char*);
void println_int(int);

int main() {
    char buffer[8];
    int red = readln(buffer, 8);
    println_int(red);
    println(buffer);

    return 0;
}
