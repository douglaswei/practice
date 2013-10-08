/*
ID: weiguoz1
PROG: window
LANG: C++
*/

#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

struct window_t {
    int x1;
    int x2;
    int y1;
    int y2;
};

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define max(a, b) ((a) > (b)) ? (a) : (b)

void set_window(window_t& window, int x1, int y1, int x2, int y2) {
    window.x1 = min(x1, x2);
    window.y1 = min(y1, y2);
    window.x2 = max(x1, x2);
    window.y2 = max(y1, y2);
}

const int MAX_WINDOW_NUM = 62;

std::list<char> g_list_window;
std::vector<char> g_vec_window;

window_t g_windows[MAX_WINDOW_NUM];

#define GET_IDX(ch) (((ch)>='A' && (ch)<='Z') ? ((ch) - 'A') : (((ch) >= 'a' && (ch) <= 'z') ? ((ch) - 'a' + 26) : ((ch) - '0' + 52)))

FILE* fin = fopen("window.in", "r");
FILE* fout = fopen("window.out", "w");

void create(char idx, int x1, int y1, int x2, int y2) {
    set_window(g_windows[idx], x1, y1, x2, y2);
    g_list_window.remove(idx);
    g_list_window.push_front(idx);
}

void del(char idx) {
    g_list_window.remove(idx);
}

void top(char idx) {
    g_list_window.remove(idx);
    g_list_window.push_front(idx);
}

void bottom(char idx) {
    g_list_window.remove(idx);
    g_list_window.push_back(idx);
}

int cal_cut(int x1, int y1, int x2, int y2, int level) {
    int pos = g_vec_window[level];
    if (level < 0) {
        return (x2 - x1) * (y2 - y1);
    }
    if (x2 <= g_windows[pos].x1 || x1 >= g_windows[pos].x2 || y2 <= g_windows[pos].y1 || y1 >= g_windows[pos].y2) {
        return cal_cut(x1, y1, x2, y2, level - 1);
    }
    --level;
    int res = 0;
    if (x1 < g_windows[pos].x1) {
        res += cal_cut(x1, y1, g_windows[pos].x1, y2, level);
        x1 = g_windows[pos].x1;
    }
    if (x2 > g_windows[pos].x2) {
        res += cal_cut(g_windows[pos].x2, y1, x2, y2, level);
        x2 = g_windows[pos].x2;
    }
    if (y1 < g_windows[pos].y1) {
        res += cal_cut(x1, y1, x2, g_windows[pos].y1, level);
        y1 = g_windows[pos].y1;
    }
    if (y2 > g_windows[pos].y2) {
        res += cal_cut(x1, g_windows[pos].y2, x2, y2, level);
    }
    return res;
}

void s(char idx) {
//¼ÆËã¾ØÐÎÇÐ¸î
    g_vec_window.clear();
    for (std::list<char>::iterator iter = g_list_window.begin(); iter != g_list_window.end(); ++iter) {
        g_vec_window.push_back(*iter);
    }
    int pos = 0;
    for (; pos < g_vec_window.size(); ++pos) {
        if (g_vec_window[pos] == idx) {
            break;
        }
    }
    int base = (g_windows[idx].x2 - g_windows[idx].x1) * (g_windows[idx].y2 - g_windows[idx].y1);
    int cut_res = cal_cut(g_windows[idx].x1, g_windows[idx].y1, g_windows[idx].x2, g_windows[idx].y2, pos - 1);
    double res = (double) cut_res * 100 / (double) base;
    fprintf(fout, "%.3f\n", res);
}

int parse_command() {
    int x1, y1, x2, y2;
    char c = fgetc(fin);
    fgetc(fin);
    if (feof(fin)) {
        return 1;
    }
    char idx = getc(fin);
    idx = GET_IDX(idx);
    switch (c) {
        case 'w':
            fscanf(fin, ",%d,%d,%d,%d)", &x1, &y1, &x2, &y2);
            fgetc(fin);
            create(idx, x1, y1, x2, y2);
            break;
        case 't':
            fgetc(fin);
            fgetc(fin);
            top(idx);
            break;
        case 'b':
            fgetc(fin);
            fgetc(fin);
            bottom(idx);
            break;
        case 'd':
            fgetc(fin);
            fgetc(fin);
            del(idx);
            break;
        case 's':
            fgetc(fin);
            fgetc(fin);
            s(idx);
            break;
        default:
            break;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    int stop = 0;
    while (stop != 1) {
        stop = parse_command();
    }

    fclose(fin);
    fclose(fout);
}
