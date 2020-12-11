// https://www.cs.utexas.edu/users/djimenez/utsa/cs1713-3/c/life.txt

void sleep(int);
int readln(char*, int);
void _ln();
void print(const char*);
void println(const char*);
void print_int(int);
void print_real(float);
void println_int(int);
void println_real(float);
void print_int_pad(int, int);
void print_real_pad(float, int);

int ___rand_counter;
int rand()
{
    ___rand_counter = ___rand_counter + 1;
    return ___rand_counter;
}
void srand(int s) { ___rand_counter = s; }

int board[32][32];

int limit(int x)
{
    int i = x;
    if (i < 0)
        i = i + 32;
    if (i >= 32)
        i = i - 32;
    return i;
}

int adjacent_to(int i, int j)
{
    int k, l, count;

    count = 0;

    /* go around the cell */
    for (k = -1; k <= 1; k = k + 1) {
        for (l = -1; l <= 1; l = l + 1) {
            /* only count if at least one of k, l isn't zero */
            if (k || l) {
                int ii = limit(i + k);
                int jj = limit(j + l);
                if (board[ii][jj]) {
                    count = count + 1;
                }
            }
        }
    }
    return count;
}

int newboard[32][32];
void play()
{
    int i, j;

    /* for each cell, apply the rules of Life */
    for (i = 0; i < 32; i = i + 1) {
        for (j = 0; j < 32; j = j + 1) {
            int a = adjacent_to(i, j);
            if (a == 2)
                newboard[i][j] = board[i][j];
            if (a == 3)
                newboard[i][j] = 1;
            if (a < 2)
                newboard[i][j] = 0;
            if (a > 3)
                newboard[i][j] = 0;
        }
    }

    /* copy the new board back into the main board */
    for (i = 0; i < 32; i = i + 1) {
        for (j = 0; j < 32; j = j + 1) {
            board[i][j] = newboard[i][j];
        }
    }
}

void print_board()
{
    int i, j;

    /* for each row */
    for (j = 0; j < 32; j = j + 1) {
        // print_int_pad(j, 4);
        // print("|");
        /* print each column position... */
        for (i = 0; i < 32; i = i + 1) {
            if (board[i][j]) {
                print("x");
            } else {
                print(" ");
            }
        }
        // print("|");
        _ln();
    }
}

void nope() {}

int main()
{
    int i, j;

    // for (i = 0; i < 32; i = i + 1) {
    //     for (j = 0; j < 32; j = j + 1) {
    //         board[i][j] = rand() % 5;
    //     }
    // }

    int x = 4;
    int y = 4;

    board[x + 0][y] = 1;
    board[x + 1][y] = 1;
    board[x + 2][y] = 1;
    board[x + 1][y + 1] = 1;

    x = 22;
    y = 10;

    board[x + 0][y] = 1;
    board[x + 1][y] = 1;
    board[x + 2][y] = 1;
    board[x + 3][y] = 1;
    board[x + 4][y] = 1;
    board[x + 5][y + 1] = 1;

    x = 24;
    y = 24;

    board[x + 0][y] = 1;
    board[x + 1][y] = 1;
    board[x + 2][y] = 1;
    board[x + 3][y] = 1;
    board[x + 4][y] = 1;
    board[x + 5][y - 1] = 1;
    board[x + 5][y - 3] = 1;
    board[x + 0][y - 1] = 1;
    board[x + 0][y - 2] = 1;
    board[x + 1][y - 3] = 1;

    for (i = 0; i < 500; i = i + 1) {
        print_board();
        play();

        /* clear the screen using VT100 escape codes */
        // print("\033[H\033[J");
        _ln();
        int k;
        sleep(200);
    }

    return 0;
}
