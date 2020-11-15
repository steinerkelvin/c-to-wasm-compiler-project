#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>

void printf(const char *);

const char EMPTY_CELL[] = "        ";
const char TABLE_WIDTH = 5 + 4 * 8;

int score = 0;
int record = 0;

int table[4][4] = {0};

// int DEFAULT_BG_256COLOR = 249;
// int DEFAULT_FG_256COLOR = 235;

// //                       2    4    8   16   32   64
// int TILE_COLORS[12] = {255, 252, 215, 202, 196, 160,
//                        229, 228, 227, 226, 220, 233};
// //                     128  258  512 1024 2048

// void set_bg_256color(int color) { printf("\033[48;5;%dm", color); }
// void reset_bg_color() { set_bg_256color(DEFAULT_BG_256COLOR); }
// void reset_original_bg_color() { system("echo \"\\033[49m\""); }
// void set_fg_256color(int color) { printf("\033[38;5;%dm", color); }
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

void clear_screen() {
    // system("clear");
}

// printa uma string n vezes
void repeatPrint(char c[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%s", c);
    }
}

// printa um espaçamento vertical de uma linha
void table_vpadding(int r) {
    printf("|");
    int c;
    for (c = 0; c < 4; c++) {
        // int v = table[r][c];
        // if (v)
            // set_bg_256color(get_tile_bg(v));
        repeatPrint(" ", 8);
        // if (v)
        //     reset_bg_color();
        printf("|");
    }
    printf("\n");
}

// printa linha horizontal com largura da tabela
void table_hline() {
    repeatPrint("-", TABLE_WIDTH);
    printf("\n");
}

// printa a tabela
void render_table() {
    table_hline();

    int r, c;
    for (r = 0; r < 4; r++) {
        // espacamento
        table_vpadding(r);

        printf("|");
        for (c = 0; c < 4; c++) {
            int v = table[r][c];
            if (v) {
                // set_bg_256color(get_tile_bg(v));
                // set_fg_256color(get_tile_fg(v));
                printf("  %4d  ", v);
                // reset_bg_color();
                // reset_fg_color();
                printf("|");
            } else {
                repeatPrint(" ", 8);
                printf("|");
            }
        }
        printf("\n");

        table_vpadding(r);

        table_hline();
    }
}

// printa informação sobre o jogo
void render_info() { printf("SCORE: %-4d  RECORD: %-4d\n", score, record); }

void render_info_2() { printf("\nPress Q to quit.\n"); }

void render_game() {
    render_info();
    render_table();
    render_info_2();
}

// conta a quantidade de espa�os vazios da tabela
int count_empty_tiles() {
    int i, j, c = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (table[i][j] == 0) {
                c++;
            }
        }
    }
    return c;
}

// insere uma peça de valor 2 ou 4 em uma posi��o aleat�ria
void insert_random_tile() {
    int empt, n;
    empt = count_empty_tiles();
    if (empt) {
        n = rand() % empt;
        int i, j, c = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (table[i][j] == 0) {
                    if (c == n) {
                        table[i][j] = rand() % 10 ? 2 : 4;
                    }
                    c++;
                }
            }
        }
    }
}

// inicializa a tabela inserindo duas peças
void init_table() {
    insert_random_tile();
    insert_random_tile();
}

void clean_table() { memset(table, 0, sizeof(table)); }

void update_record(int n) { record = n; }

void add_score(int n) {
    score += n;
    if (score > record) {
        update_record(score);
    }
}

int move_table_hor(int dir) {
    int new_row[4], cur_tile;
    int i, j, c, n;
    int moved = 0;

    // para cada linha da tabela
    for (i = 0; i < 4; i++) {
        n = 0, cur_tile = 0;
        // limpa o vetor para a nova linha
        memset(new_row, 0, 4 * sizeof(new_row[0]));
        // para cada colula
        for (j = 0; j < 4; j++) {
            // determina o valor do �ndice da coluna baseado na direção do
            // movimento
            if (dir == +1) // direita
                c = 3 - j;
            else if (dir == -1) // esquerda
                c = j;
            // se n�o houver peça ativa e encontrar uma peça
            if (!cur_tile && table[i][c]) {
                cur_tile = table[i][c];
                // se houver peça ativa e encontrar uma peça
            } else if (cur_tile && table[i][c]) {
                // se a peça encontrada for igual a ativa
                if (table[i][c] == cur_tile) {
                    // adiciona à nova coluna uma peça com o dobro do valor das
                    // peças encontradas
                    new_row[n++] = cur_tile * 2;
                    add_score(cur_tile * 2);
                    cur_tile = 0;
                    // se for diferente
                } else if (table[i][c] != cur_tile) {
                    // adiciona à nova coluna a peça ativa e seta a peça ativa
                    // para peça encontrada
                    new_row[n++] = cur_tile;
                    cur_tile = table[i][c];
                }
            }
        }
        if (cur_tile)
            new_row[n++] = cur_tile;

        // escreve a nova coluna na tabela de acordo com a dirte��o do movimento
        for (j = 0; j < 4; j++) {
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

int move_table_vert(int dir) {
    int new_col[4], cur_tile;
    int i, j, r, n;
    int moved = 0;

    // para cada coluna da tabela
    for (i = 0; i < 4; i++) {
        n = 0, cur_tile = 0;
        // limpa o vetor para a nova linha
        memset(new_col, 0, 4 * sizeof(new_col[0]));
        // para linha colula
        for (j = 0; j < 4; j++) {
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
                    new_col[n++] = cur_tile * 2;
                    add_score(cur_tile * 2);
                    cur_tile = 0;
                    // se for diferente
                } else if (table[r][i] != cur_tile) {
                    // adiciona à nova coluna a peça ativa e seta a peça ativa
                    // para peça encontrada
                    new_col[n++] = cur_tile;
                    cur_tile = table[r][i];
                }
            }
        }
        if (cur_tile)
            new_col[n++] = cur_tile;

        // escreve a nova coluna na tabela de acordo com a direçãos do movimento
        for (j = 0; j < 4; j++) {
            if (dir == +1)
                r = j;
            else if (dir == -1)
                r = 3 - j;
            if (!moved && table[r][i] != new_col[j])
                moved = 1;
            table[r][i] = new_col[j];
        }
    }
    return moved;
}

void reset_game() {
    clean_table();
    init_table();
    clear_screen();
    render_game();
}

int main() {
    // srand(time(NULL));
    srand(0);

    clean_table();
    init_table();

    // reset_bg_color();
    // reset_fg_color();

    clear_screen();
    render_game();

    while (1) {
        char ch;
        const int ret = scanf(" %c", &ch);
        if (ret != 1)
            continue;

        if ((char)ch == 'R' || (char)ch == 'r')
            reset_game();
        else if ((char)ch == 'Q' || (char)ch == 'q') {
            break;
        } else {
            int moved = 0;
            switch (ch) {
            case 'w':
            case 'W':
                moved = move_table_vert(+1);
                break;
            case 's':
            case 'S':
                moved = move_table_vert(-1);
                break;
            case 'd':
            case 'D':
                moved = move_table_hor(+1);
                break;
            case 'a':
            case 'A':
                moved = move_table_hor(-1);
                break;
            }
            if (moved) {
                insert_random_tile();
            }
            clear_screen();
            render_game();
            if (count_empty_tiles() == 0) {
                clear_screen();
                render_info();
                render_table();
                printf("GAME OVER\n");
                printf("press R to start a new game\n");

                char ch;
                const int ret = scanf(" %c", &ch);
                if (ret != 1) {
                    break;
                }
                if ((char)ch == 'R' || (char)ch == 'r') {
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
