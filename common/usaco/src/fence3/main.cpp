/*
ID: weiguoz1
PROG: fence3
LANG: C++
*/

#include <stdio.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <math.h>

struct fence_t{
    int x0;
    int y0;
    int x1;
    int y1;
};

const int MAX_FENCE_NUM = 150;
int g_fence_num = 0;
std::vector<fence_t> g_fences;
int g_up = 0;
int g_down = 0;
int g_left = 0;
int g_right = 0;
float g_center_x = 0;
float g_center_y = 0;
float g_max_dis = __FLT_MAX__;

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define abs(a) ((a) > 0) ? (a) : ((a) * -1)

float calc_dis_point(int x0, int y0, int x1, int y1) {
    return sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2));
}

float calc_dis(int x, int y, fence_t fen)
{
    if ((fen.x0 == fen.x1) && ((y - fen.y0) * (y - fen.y1) <= 0)) {
        return abs(x - fen.x0);
    }
    if ((fen.y0 == fen.y1) && ((x - fen.x0) * (x - fen.x1) <= 0)) {
        return abs(y - fen.y0);
    }
    return min(calc_dis_point(x, y, fen.x0, fen.y0),
            calc_dis_point(x, y, fen.x1, fen.y1));
}

float try_with_step(int step_x, int step_y)
{
    for (int cur_x = g_left; cur_x <= g_right; cur_x += step_x) {
        for (int cur_y = g_down; cur_y <= g_up; cur_y += step_y) {
            float cur_dis = 0;
            for (int idx = 0; idx < g_fences.size(); ++idx) {
                float ret = calc_dis(cur_x, cur_y, g_fences[idx]);
                cur_dis += ret;
            }
            if (cur_dis < g_max_dis) {
                g_max_dis = cur_dis;
                g_center_x = cur_x;
                g_center_y = cur_y;
            }
        }
    }

    int new_left = g_center_x - step_x;
    int new_right = g_center_x + step_x;
    int new_up = g_center_y + step_y;
    int new_down = g_center_y - step_y;
    if (new_left > g_left) g_left = new_left;
    if (new_right < g_right) g_right = new_right;
    if (new_up < g_up) g_up = new_up;
    if (new_down > g_down) g_down = new_down;

    int new_step_x = (step_x > 1) ? (step_x / 5) : step_x;
    if (new_step_x < 1) new_step_x = 1;
    int new_step_y = (step_y > 1) ? (step_y / 5) : step_y;
    if (new_step_y < 1) new_step_y = 1;
    if ((new_step_x != step_x) || (new_step_y != step_y)) {
        try_with_step(new_step_x, new_step_y);
    }
}

int main(int argc, char* argv[])
{
    FILE* fin = fopen("fence3.in", "r");
    FILE* fout = fopen("fence3.out", "w");

    fscanf(fin, "%d", &g_fence_num);
    g_fences.reserve(g_fence_num);
    fence_t fen;
    float x0, y0, x1, y1;
    for (int i = 0; i < g_fence_num; ++i) {
        fscanf(fin, "%d %d %d %d", &fen.x0, &fen.y0, &fen.x1, &fen.y1);
        fen.x0 *= 10;
        fen.x1 *= 10;
        fen.y0 *= 10;
        fen.y1 *= 10;
        if (g_left > fen.x0) g_left = fen.x0;
        if (g_left > fen.x1) g_left = fen.x1;
        if (g_right < fen.x0) g_right = fen.x0;
        if (g_right < fen.x1) g_right = fen.x1;
        if (g_up < fen.y0) g_up = fen.y0;
        if (g_up < fen.y1) g_up = fen.y1;
        if (g_down > fen.y0) g_down = fen.y0;
        if (g_down > fen.y1) g_down = fen.y1;
        g_fences.push_back(fen);
    }

    try_with_step(10, 10);

    fprintf(fout, "%.1f %.1f %.1f\n", g_center_x / 10, g_center_y / 10, g_max_dis / 10);

    fclose(fin);
    fclose(fout);
}
