void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

// int mat[4][4];

int main()
{
    int mat[4][4];
    {
        int i;
        for (i = 0; i < 4; i = i + 1) {
            int j;
            for (j = 0; j < 4; j = j + 1) {
                int ii = i + 1;
                int jj = (j + 1);
                mat[i][j] = 1000 * ii + jj * jj;
            }
        }
    }

    {
        int i;
        for (i = 0; i < 4; i = i + 1) {
            print_int_pad(i, 2);
            print(" | ");
            int j;
            for (j = 0; j < 4; j = j + 1) {
                print_int_pad(mat[i][j], 8);
            }
            _ln();
        }
    }

    return 0;
}
