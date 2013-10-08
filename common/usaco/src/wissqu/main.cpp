/*
ID: weiguoz1
PROG: wissqu
LANG: C++
*/

#include <stdio.h>
#include <iostream>

const int SQUARE_SIZE = 4;

const int CALVE_NUM = 5;

const int SOLUTION_STEPS = 16;

const char MASK_VISIT = 0x80;

char g_grid[SQUARE_SIZE + 2][SQUARE_SIZE + 2] = {0};
char g_grid_org[SQUARE_SIZE + 2][SQUARE_SIZE + 2] = {0};

int g_calve_num[CALVE_NUM] = {0};

int g_solution_num = 0;

int g_solution[SOLUTION_STEPS][3];

void init() {
    for (int i = 1; i <= SQUARE_SIZE; ++i) {
        for (int j = 1; j <= SQUARE_SIZE; ++j) {
            g_grid_org[i][j] = g_grid[i][j];
            int des_idx = (g_grid[i][j] - 'A' + 1) % CALVE_NUM;
            g_calve_num[des_idx]++;
        }
    }
}

bool is_char_ok(char ch, int pos_x, int pos_y) {
    if ( (g_grid[pos_x][pos_y] != g_grid_org[pos_x][pos_y])
        || (g_grid[pos_x - 1][pos_y - 1] == ch)
        || (g_grid[pos_x - 1][pos_y] == ch)
        || (g_grid[pos_x - 1][pos_y + 1] == ch)
        || (g_grid[pos_x][pos_y - 1] == ch)
        || (g_grid[pos_x][pos_y] == ch)
        || (g_grid[pos_x][pos_y + 1] == ch)
        || (g_grid[pos_x + 1][pos_y - 1] == ch)
        || (g_grid[pos_x + 1][pos_y] == ch)
        || (g_grid[pos_x + 1][pos_y + 1] == ch )) {
            return false;
    }
    return true;
}

void dfs(int step) {
    if (step >= SOLUTION_STEPS) {
        ++g_solution_num;
        return;
    }
    for (char id_ch = 'A'; id_ch <= 'E'; ++id_ch) {
        if ((step == 0) && (id_ch < 'D')) {
            continue;
        }
        int id_des = id_ch - 'A';
        if (g_calve_num[id_des] == 0) {
            continue;
        }
        --g_calve_num[id_des];
        for (int grid_x = 1; grid_x <= SQUARE_SIZE; ++grid_x) {
            for (int grid_y = 1; grid_y <= SQUARE_SIZE; ++grid_y) {
                if (!is_char_ok(id_ch, grid_x, grid_y)) {
                    continue;
                }
                if (g_solution_num == 0) {
                    g_solution[step][0] = id_ch;
                    g_solution[step][1] = grid_x;
                    g_solution[step][2] = grid_y;
                }
                g_grid[grid_x][grid_y] = id_ch;
                dfs(step + 1);
                g_grid[grid_x][grid_y] = g_grid_org[grid_x][grid_y];
            }
        }
        ++g_calve_num[id_des];
    }
}

int main(int argc, char* argv[])
{
    FILE* fin = fopen("wissqu.in", "r");
    FILE* fout = fopen("wissqu.out", "w");

    for (int i = 1; i <= SQUARE_SIZE; ++i) {
        for (int j = 1; j <= SQUARE_SIZE; ++j) {
            g_grid[i][j] = fgetc(fin);
        }
        fgetc(fin);
    }

    init();
    dfs(0);

    for (int i = 0; i < SOLUTION_STEPS; ++i) {
        fprintf(fout, "%c %d %d\n", g_solution[i][0], g_solution[i][1], g_solution[i][2]);
    }
    fprintf(fout, "%d\n", g_solution_num);

    fclose(fin);
    fclose(fout);
}
