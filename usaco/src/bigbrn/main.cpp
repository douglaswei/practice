/*
ID: weiguoz1
PROG: bigbrn
LANG: C++
*/

#include <cstring>
#include <stdio.h>
#include <iostream>

const size_t MAX_BARN_SIZE = 1001;
const size_t MAX_TREE_NUM = 10001;

char g_square_barn[MAX_BARN_SIZE][MAX_BARN_SIZE];
size_t g_barn_res[MAX_BARN_SIZE][MAX_BARN_SIZE];

size_t square_size = 0;
size_t tree_num = 0;
size_t g_res = 0;

int main(int argc, char* argv[])
{
    FILE* fin = fopen("bigbrn.in", "r");
    FILE* fout = fopen("bigbrn.out", "w");

    memset(g_square_barn, 0, sizeof(g_square_barn));
    memset(g_barn_res, 0, sizeof(g_barn_res));

    fscanf(fin, "%zu %zu", &square_size, &tree_num);

    for (size_t tree_idx = 0; tree_idx < tree_num; ++tree_idx) {
        size_t x = 0;
        size_t y = 0;
        fscanf(fin, "%zu %zu", &x, &y);
        g_square_barn[x][y] = 1;
    }

    for (size_t x = 1; x <= square_size; ++x) {
        for (size_t y = 1; y <= square_size; ++y) {
            if (g_square_barn[x][y] != 0) {
                g_barn_res[x][y] = 0;
                continue;
            }
            size_t res_x = g_barn_res[x - 1][y];
            size_t res_y = g_barn_res[x][y - 1];
            size_t min_res = res_x;
            if (min_res > res_y) {
                min_res = res_y;
            }
            g_barn_res[x][y] = min_res;
            if (g_square_barn[x - min_res][y - min_res] == 0) {
                ++g_barn_res[x][y];
            }
            if (g_res < g_barn_res[x][y]) {
                g_res = g_barn_res[x][y];
            }
        }
    }

    fprintf(fout, "%zu\n", g_res);

    fclose(fin);
    fclose(fout);
}
