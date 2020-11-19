int main()
{
    char c;
    int i;
    float r;
    int vec[4];

    int *pa = &i;
    int *pb = &i;

    if (c == i) { // should work
    }
    if (i == r) { // should work
    }
    if (c == r) { // should work
    }

    // if (pa == pb) { // TODO?
    // }

    if (i == vec) { // error

    }
}
