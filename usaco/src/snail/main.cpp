/*
ID: weiguoz1
PROG: snail
LANG: C++
*/

#include <stdio.h>
#include <cstring>
#include <iostream>

const int MAX_WIDTH = 120;
int g_width = 0;
const int MAX_BARRY_NUM = 200;
int g_barry_num = 0;

char g_grid[MAX_WIDTH * MAX_WIDTH];

const char C_VISITED = 'V';
const char C_RESET = '.';
const char C_BARRY = 'B';

enum DIRECTION{
    DIR_BASE = 0xFF,
    DIR_LEFT = 0x00,
    DIR_RIGHT = 0x03,
    DIR_UP = 0x01,
    DIR_DOWN = 0x02,
    DIR_BEG = 0x05
};

struct point_t {
    point_t(int _x = 0, int _y = 0) {x = _x; y = _y;}
    int x;
    int y;
};

void print_grid()
{
    return;
    for (int idx = 0; idx < g_width; ++idx) {
        for (int idy = 0; idy < g_width; ++idy) {
            std::cout<<g_grid[idx * g_width + idy];
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

int run_direction(int direction, point_t pt_beg, point_t& pt_end, int& stop)
{
    stop = 0;
    pt_end = pt_beg;
    int cur = pt_beg.x * g_width + pt_beg.y;
    int len = 0;
    int step = 1;
    int low_bound = 0;
    int up_bound = g_width * g_width;
    if (DIR_LEFT == direction) {
        step = -1;
        low_bound = pt_beg.x * g_width;
    }
    if (DIR_RIGHT == direction) {
        step = 1;
        up_bound = (pt_beg.x + 1) * g_width - 1;
    }
    if (DIR_UP == direction) {
        step = -g_width;
        low_bound = pt_beg.y;
    }
    if (DIR_DOWN == direction) {
        step = g_width;
        up_bound = g_width * g_width - 1;
    }
    cur += step;
    while (cur >= low_bound && cur <= up_bound) {
        if (C_RESET == g_grid[cur]) {
            g_grid[cur] = C_VISITED;
            ++len;
        } else if (C_BARRY == g_grid[cur]) {
            break;
        } else if (C_VISITED == g_grid[cur]) {
            stop = 1;
            break;
        }
        cur += step;
    }
    cur -= step;
    pt_end.x = cur / g_width;
    pt_end.y = cur % g_width;
    return len;
}

void run_back(point_t pt_beg, point_t pt_end)
{
    int step = 1;
    int cur = pt_beg.x * g_width + pt_beg.y;
    int bound = pt_end.x * g_width + pt_end.y;
    if (pt_beg.y == pt_end.y) {
        step = g_width;
    }
    if (cur > bound) {
        step *= -1;
    }
    while (cur != bound) {
        g_grid[cur] = C_RESET;
        cur += step;
    }
}

int get_max_route(int prev_direction, point_t pt_cur);

int try_direction(int direction, point_t pt_beg, int&  max_route)
{
    int stop = 0;
    point_t pt_end;
    int ret = run_direction(direction, pt_beg, pt_end, stop);
    print_grid();
    if (ret > 0) {
        int sub_max_route = 0;
        if (0 == stop) {
            sub_max_route = get_max_route(direction, pt_end);
        }
        if (ret + sub_max_route > max_route) {
            max_route = ret + sub_max_route;
        }
        run_back(pt_end, pt_beg);
        print_grid();
    }
    return 0;
}

int get_max_route(int prev_direction, point_t pt_cur)
{
    int max_route = 0;
    int ret = 0;
    if (DIR_BEG == prev_direction) {
        try_direction(DIR_RIGHT, pt_cur, max_route);
        try_direction(DIR_DOWN, pt_cur, max_route);
    } else {
        if (prev_direction == DIR_LEFT || prev_direction == DIR_RIGHT) {
            try_direction(DIR_UP, pt_cur, max_route);
            try_direction(DIR_DOWN, pt_cur, max_route);
        }
        if (prev_direction == DIR_UP || prev_direction == DIR_DOWN) {
            try_direction(DIR_LEFT, pt_cur, max_route);
            try_direction(DIR_RIGHT, pt_cur, max_route);
        }
    }
    return max_route;
}

int main(int argc, char* argv[])
{
    memset(g_grid, C_RESET, sizeof(g_grid));
    g_grid[0] = C_VISITED;
    FILE* fin = fopen("snail.in", "r");
    FILE* fout = fopen("snail.out", "w");

    fscanf(fin, "%d", &g_width);
    fscanf(fin, "%d", &g_barry_num);
    char c[10];
    int x;
    int y;
    for (int idx = 0; idx < g_barry_num; ++idx) {
        fscanf(fin, "%s", &c);
        y = c[0] - 'A';
        x = atol(&c[1]) - 1;
        g_grid[x * g_width + y] = C_BARRY;
    }
    int max_route = get_max_route(DIR_BEG, point_t(0, 0));
    max_route++;
    fprintf(fout, "%d\n", max_route);
    
    fclose(fin);
    fclose(fout);
    return 0;
}
