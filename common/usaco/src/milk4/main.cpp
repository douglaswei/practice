/*
ID: weiguoz1
PROG: milk4
LANG: C++
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

const int MAX_PAIL_NUM = 100;
int g_pail_num;
std::vector<int> vec_cap;
int measure;
FILE* fin = fopen("milk4.in", "r");
FILE* fout = fopen("milk4.out", "w");

// 0 for can fix
int can_fix(int cap, const std::vector<int> vec, int num) {
    if (num == 0) {
        return cap;
    }
    int max_num = cap / vec[num - 1];
    for (int i = max_num; i > 0; --i) {
        int ret = can_fix(cap - vec[num - 1] * i, vec, num - 1);
        if (0 == ret) {
            return 0;
        }
    }
    return -1;
}

// 0 for valuable status
int get_next_status(int(& mask)[MAX_PAIL_NUM], int num, int& beg) {
    if (beg) {
        for (int i = 0; i < num; ++i) {
            mask[i] = 1;
        }
        beg = 0;
        return 0;
    }
    int num_1 = 0;
    int view_0 = 0;
    int idx = g_pail_num - 1;
    for (; idx >= 0; --idx) {
        if (mask[idx] == 0) {
            view_0 = 1;
        }
        if (mask[idx] == 1) {
            ++num_1;
            if (view_0 == 1) {
                break;
            }
        }
    }
    if (idx < 0) {
        return -1;
    }
    mask[idx] = 0;
    while (num_1 > 0) {
        mask[++idx] = 1;
        --num_1;
    }
    idx++;
    while (idx < g_pail_num) {
        mask[idx++] = 0;
    }
    return 0;
}

int try_num(int cap, int num) {
    // init 
    int mask[MAX_PAIL_NUM] = {0};
    std::vector<int> vec_try;
    vec_try.reserve(num);
    
    int beg = 1;
    while (true) {
        int res = get_next_status(mask, num, beg);
        if (res != 0) {
            break;
        }
        vec_try.clear();
        for (int i = 0; i < g_pail_num; ++i) {
            if (mask[i]) {
                vec_try.push_back(vec_cap[i]);
            }
        }
        if (0 == can_fix(cap, vec_try, vec_try.size())) {
            fprintf(fout, "%d", vec_try.size());
            for (int i = 0; i < vec_try.size(); ++i) {
                fprintf(fout, " %d", vec_try[i]);
            }
            fprintf(fout, "%s", "\n");
            return 0;
        }
    }
    return -1;
}

int main(int argc, char* argv[])
{
    fscanf(fin, "%d", &measure);
    fscanf(fin, "%d", &g_pail_num);

    int tmp_cap;
    std::vector<int> vec_tmp;
    for (int i = 0; i < g_pail_num; ++i) {
        fscanf(fin, "%d", &tmp_cap);
        vec_tmp.push_back(tmp_cap);
    }

    sort(vec_tmp.begin(), vec_tmp.end());

    for (int i = 0; i < vec_tmp.size(); ++i) {
        tmp_cap = vec_tmp[i];
        if (0 == can_fix(tmp_cap, vec_cap, vec_cap.size())) {
            continue;
        }
        vec_cap.push_back(tmp_cap);
    }

    for (int i = 1; i <= g_pail_num; ++i) {
        int res = try_num(measure, i);
        if (res == 0) {
            break;
        }
    }

    fclose(fin);
    fclose(fout);
}
