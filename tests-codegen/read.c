int readln(char*, int);
void print(const char*);
void println(const char*);
void println_int(int);

int main() {
    char buffer[8];
    int red = readln(buffer, 8);

    print("buffer: [");
    print(buffer);
    println("]");

    print("line size: ");
    println_int(red);

    print("did overflow: ");
    if (red > 8) {
        println("YES");
        return 0;
    }
    println("NO, its fine");

    return 0;
}
