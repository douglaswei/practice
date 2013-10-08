/*
ID: weiguoz1
PROG: schlnet
LANG: C++
*/

#include <stdio.h>
#include <iostream>

const int MAX_VERTEX_NUM = 1000;

int g_index = 1;
int g_scc_index = 1;
int g_vertex_num = 0;

int g_rec_index[MAX_VERTEX_NUM];
int g_rec_lowlink[MAX_VERTEX_NUM];
int g_rec_scc[MAX_VERTEX_NUM];
char g_links[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

std::vector<int> g_vector;

void mark_scc(int idx) {
    int cont = 1;
    while (cont) {
        int des = g_vector.back();
        if (des == idx) {
            cont = 0;
            g_rec_scc[des] = g_scc_index;
        }
        g_vector.pop_back();
    }
    ++g_scc_index;
}

void get_scc(int idx) {
    g_rec_index[idx] = g_index;
    g_rec_lowlink[idx] = g_index;
    ++g_index;
    g_vector.push_back(idx);
    for (int idy = 0; idy < g_vertex_num; ++idy) {
        if (g_links[idx][idy] != 0) {
            if (g_rec_index[idy] <= 0) {
                get_scc(idy);
                if (g_rec_lowlink[idx] > g_rec_lowlink[idy]) {
                    g_rec_lowlink[idx] = g_rec_lowlink[idy];
                }
            } else if (g_rec_scc[idx] <= 0) {
                if (g_rec_lowlink[idx] > g_rec_index[idy]) {
                    g_rec_index[idx] = g_rec_index[idy];
                }
            }
        }
    }

    if (g_rec_lowlink[idx] == g_rec_index[idx]) {
        mark_scc[idx];
    }
}

void calc_scc() {
    g_vector.reserve(MAX_VERTEX_NUM);
    for (int idx = 0; idx < g_vertex_num; ++idx) {
        if (g_rec_lowlink[idx] <= 0) {
            get_scc(idx);
        }
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

