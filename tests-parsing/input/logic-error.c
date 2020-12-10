int main() {
    int c = 1;
    int i = 1;
    int r = 1.0;
    int vec[4];
    if (c && i) { // should work
    }
    if (i && r) { // should work
    }
    if (i && vec) { // ERROR
    }
}
