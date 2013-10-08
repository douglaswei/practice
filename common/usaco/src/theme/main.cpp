/*
ID: weiguoz1
PROG: theme
LANG: C++
*/

#include <stdio.h>
#include <iostream>

const int MAX_STR_LEN = 5000;
char g_melody[MAX_STR_LEN + 1];
int g_melody_size;

size_t hash_func(char* str, int len)
{
    size_t res = len;
    for (int i = 0; i < len; ++i) {
        char c = 0;
        if (i == 0) {
            c = 0;
        } else {
            c = str[i] - str[i - 1];
        }
        res = ((res << 5) ^ (res >> 27)) ^ c;
    }
    return res;
}

int find(int len){
    size_t hash_res[MAX_STR_LEN];
    for(int beg = 0; beg < g_melody_size - len + 1; ++beg) {
        hash_res[beg] = hash_func(g_melody + beg, len);
        int cmp_beg = 0;
        while (cmp_beg <= beg - len) {
            if (hash_res[beg] == hash_res[cmp_beg]) {
                return 0;
            }
            ++cmp_beg;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    FILE* fin = fopen("theme.in", "r");
    FILE* fout = fopen("theme.out", "w");

    fscanf(fin, "%d", &g_melody_size);
    for (int i = 0; i < g_melody_size; ++i) {
        int tmp_melody;
        fscanf(fin, "%d", &(tmp_melody));
        g_melody[i] = tmp_melody;
    }
    int min = 5;
    int max = g_melody_size / 2;
    int ret = 0;
    while (max >= min) {
        int middle = (min + max) / 2;
        if (find(middle) == 0) {
            ret = middle;
            min = middle + 1;
        } else {
            max = middle - 1;
        }
        continue;
    }
    fprintf(fout, "%d\n", ret);
    return 0;
}
