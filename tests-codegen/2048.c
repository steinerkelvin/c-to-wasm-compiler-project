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

// const char EMPTY_CELL[] = "        ";
// const char TABLE_WIDTH = 5 + 4 * 8;

// int score = 0;
// int record = 0;

char* EMPTY_CELL;
int TABLE_WIDTH;

int score;
int record;

int table[4][4]; // = {0};

// int DEFAULT_BG_256COLOR = 249;
// int DEFAULT_FG_256COLOR = 235;

// //                       2    4    8   16   32   64
// int TILE_COLORS[12] = {255, 252, 215, 202, 196, 160,
//                        229, 228, 227, 226, 220, 233};
// //                     128  258  512 1024 2048

// void set_bg_256color(int color) { print("\033[48;5;%dm", color); }
// void reset_bg_color() { set_bg_256color(DEFAULT_BG_256COLOR); }
// void reset_original_bg_color() { system("echo \"\\033[49m\""); }
// void set_fg_256color(int color) { print("\033[38;5;%dm", color); }
// void reset_fg_color() { set_fg_256color(DEFAULT_FG_256COLOR); }
// void reset_original_fg_color() { system("echo \"\\033[39m\""); }

// void set_bg_256color(int color) {}
// void reset_bg_color() {}
// void reset_original_bg_color() {}
// void set_fg_256color(int color) {}
// void reset_fg_color() {}
// void reset_original_fg_color() {}

// // determina a cor do background de uma peça
// int get_tile_bg(int n) {
//     int i = log2(n) - 1;
//     if (i <= 11)
//         return TILE_COLORS[i];
//     else
//         return TILE_COLORS[11];
// }

// // determina a cor do texto de uma peça
// int get_tile_fg(int n) {
//     int i = log2(n);
//     if (i <= 11)
//         return 235;
//     else
//         return 255;
// }

void clear_screen()
{
    // int i;
    // for (i = 0; i < 40; i = i + 1) {
    //     _ln();
    // }
}

// printa uma string n vezes
void repeatPrint(char c[], int n)
{
    int i;
    for (i = 0; i < n; i = i + 1) {
        print(c);
    }
}

// printa um espaçamento vertical de uma linha
void table_vpadding(int r)
{
    print("|");
    int c;
    for (c = 0; c < 4; c = c + 1) {
        // int v = table[r][c];
        // if (v)
        // set_bg_256color(get_tile_bg(v));
        repeatPrint(" ", 8);
        // if (v)
        //     reset_bg_color();
        print("|");
    }
    // print("\n");
    _ln();
}

// printa linha horizontal com largura da tabela
void table_hline()
{
    repeatPrint("-", TABLE_WIDTH);
    // print("\n");
    _ln();
}

// printa a tabela
void render_table()
{
    table_hline();

    int r, c;
    for (r = 0; r < 4; r = r + 1) {
        // espacamento
        table_vpadding(r);

        print("|");
        for (c = 0; c < 4; c = c + 1) {
            int v = table[r][c];
            if (v) {
                // set_bg_256color(get_tile_bg(v));
                // set_fg_256color(get_tile_fg(v));
                print("  ");
                print_int_pad(v, 4);
                print("  ");
                // reset_bg_color();
                // reset_fg_color();
                print("|");
            } else {
                repeatPrint(" ", 8);
                print("|");
            }
        }
        // print("\n");
        _ln();

        table_vpadding(r);

        table_hline();
    }
}

// printa informação sobre o jogo
void render_info()
{
    print("SCORE: ");
    print_int_pad(score, -8);
    print("  RECORD: ");
    print_int_pad(record, -8);
    _ln();
}

void render_info_2() { println("CONTROLS: W A S D; Q to quit"); }

void render_game()
{
    render_info();
    _ln();
    render_table();
    _ln();
    render_info_2();
}

// conta a quantidade de espa�os vazios da tabela
int count_empty_tiles()
{
    int i, j, c = 0;
    for (i = 0; i < 4; i = i + 1) {
        for (j = 0; j < 4; j = j + 1) {
            if (table[i][j] == 0) {
                c = c + 1;
            }
        }
    }
    return c;
}

// insere uma peça de valor 2 ou 4 em uma posi��o aleat�ria
void insert_random_tile()
{
    int empt, n;
    empt = count_empty_tiles();
    if (empt) {
        n = rand() % empt;
        int i, j, c = 0;
        for (i = 0; i < 4; i = i + 1) {
            for (j = 0; j < 4; j = j + 1) {
                if (table[i][j] == 0) {
                    if (c == n) {
                        // table[i][j] = rand() % 10 ? 2 : 4; // TODO
                        int r = rand();
                        if (r % 10 == 0) {
                            table[i][j] = 4;
                        } else {
                            table[i][j] = 2;
                        }
                    }
                    c = c + 1;
                }
            }
        }
    }
}

// inicializa a tabela inserindo duas peças
void init_table()
{
    insert_random_tile();
    insert_random_tile();
}

void clean_table()
{
    // memset(table, 0, sizeof(table));
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int j;
        for (j = 0; j < 4; j = j + 1) {
            table[i][j] = 0;
        }
    }
}

void update_record(int n) { record = n; }

void add_score(int n)
{
    // score += n;  // TODO
    score = score + n; // TODO
    if (score > record) {
        update_record(score);
    }
}

int move_table_hor(int dir)
{
    int moved = 0;

    // para cada linha da tabela
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int n = 0, cur_tile = 0;
        // vetor para a nova linha
        int new_row[4]; // = {0, 0, 0, 0};
        int j;
        for (j = 0; j < 4; j = j + 1) {
            new_row[j] = 0;
        }
        // para cada colula
        for (j = 0; j < 4; j = j + 1) {
            // determina o valor do �ndice da coluna baseado na direção do
            // movimento
            int c;
            if (dir == +1) { // direita
                c = 3 - j;
            } else if (dir == -1) { // esquerda
                c = j;
            }
            // se n�o houver peça ativa e encontrar uma peça
            if (!cur_tile && table[i][c]) {
                cur_tile = table[i][c];
                // se houver peça ativa e encontrar uma peça
            } else if (cur_tile && table[i][c]) {
                // se a peça encontrada for igual a ativa
                if (table[i][c] == cur_tile) {
                    // adiciona à nova coluna uma peça com o dobro do valor das
                    // peças encontradas
                    new_row[n] = cur_tile * 2;
                    n = n + 1;
                    add_score(cur_tile * 2);
                    cur_tile = 0;
                    // se for diferente
                } else if (table[i][c] != cur_tile) {
                    // adiciona à nova coluna a peça ativa e seta a peça ativa
                    // para peça encontrada
                    new_row[n] = cur_tile;
                    n = n + 1;
                    cur_tile = table[i][c];
                }
            }
        }
        if (cur_tile) {
            new_row[n] = cur_tile;
            n = n + 1;
        }
        // escreve a nova coluna na tabela de acordo com a dirte��o do movimento
        for (j = 0; j < 4; j = j + 1) {
            int c;
            if (dir == +1)
                c = 3 - j;
            else if (dir == -1)
                c = j;
            if (!moved && table[i][c] != new_row[j])
                moved = 1;
            table[i][c] = new_row[j];
        }
    }
    return moved;
}

int move_table_vert(int dir)
{
    int moved = 0;

    // para cada coluna da tabela
    int i;
    for (i = 0; i < 4; i = i + 1) {
        int n = 0, cur_tile = 0;
        // vetor para a nova linha
        int new_col[4]; // = {0, 0, 0, 0};
        int j;
        for (j = 0; j < 4; j = j + 1) {
            new_col[j] = 0;
        }
        // para linha colula
        for (j = 0; j < 4; j = j + 1) {
            int r;
            // determina o valor do �ndice da linha baseado na direção do
            // movimento
            if (dir == +1) // cima
                r = j;
            else if (dir == -1) // baixo
                r = 3 - j;
            // se n�o houver peça ativa e encontrar uma peça
            if (!cur_tile && table[r][i]) {
                cur_tile = table[r][i];
                // se houver peça ativa e encontrar uma peça
            } else if (cur_tile && table[r][i]) {
                // se a peça encontrada for igual a ativa
                if (table[r][i] == cur_tile) {
                    // adiciona à nova coluna uma peça com o dobro do valor das
                    // peças encontradas
                    new_col[n] = cur_tile * 2;
                    n = n + 1;
                    add_score(cur_tile * 2);
                    cur_tile = 0;
                    // se for diferente
                } else if (table[r][i] != cur_tile) {
                    // adiciona à nova coluna a peça ativa e seta a peça ativa
                    // para peça encontrada
                    new_col[n] = cur_tile;
                    n = n + 1;
                    cur_tile = table[r][i];
                }
            }
        }
        if (cur_tile) {
            new_col[n] = cur_tile;
            n = n + 1;
        }

        // escreve a nova coluna na tabela de acordo com a direçãos do movimento
        for (j = 0; j < 4; j = j + 1) {
            int r;
            if (dir == +1) {
                r = j;
            } else if (dir == -1) {
                r = 3 - j;
            }
            if (!moved && table[r][i] != new_col[j])
                moved = 1;
            table[r][i] = new_col[j];
        }
    }
    return moved;
}

void reset_game()
{
    score = 0;
    clean_table();
    init_table();
    clear_screen();
    render_game();
}

int main()
{
    EMPTY_CELL = "        ";
    TABLE_WIDTH = 5 + 4 * 8;
    score = 0;
    record = 0;

    // srand(time(NULL));
    srand(3);

    clean_table();
    init_table();

    // reset_bg_color();
    // reset_fg_color();

    // clear_screen();
    render_game();

    char buffer[32];
    while (1) {
        print("> ");
        int linesize = readln(buffer, 1024);
        if (linesize >= 1024) {
            println("input line too long");
            return 1;
        }

        int ret = 1; // const int ret = scanf(" %c", &ch); // TODO
        if (ret != 1) {
            continue;
        }

        char ch;
        ch = buffer[0];

        if (ch == 'R' || ch == 'r') {
            println("Resetting...");
            reset_game();
        } else if (ch == 'Q' || ch == 'q') {
            println("Quitting...");
            break;
        } else {
            int moved = 0;
            if (ch == 'w' || ch == 'W') {
                moved = move_table_vert(+1);
            } else if (ch == 's' || ch == 'S') {
                moved = move_table_vert(-1);
            } else if (ch == 'd' || ch == 'D') {
                moved = move_table_hor(+1);
            } else if (ch == 'a' || ch == 'A') {
                moved = move_table_hor(-1);
            }
            if (moved) {
                insert_random_tile();
            }

            // clear_screen();
            render_game();

            if (count_empty_tiles() == 0) {
                clear_screen();
                render_info();
                render_table();
                println("GAME OVER");
                println("press R to start a new game");

                char ch;
                int ret; // const int ret = scanf(" %c", &ch); // TODO
                if (ret != 1) {
                    break;
                }
                if (ch == 'R' || ch == 'r') {
                    reset_game();
                    continue;
                } else {
                    break;
                }
            }
        }
    }

    // reset_original_bg_color();
    // reset_original_fg_color();
    clear_screen();

    return 0;
}
