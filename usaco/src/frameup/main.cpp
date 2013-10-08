/*
ID: weiguoz1
PROG: frameup
LANG: C++
*/

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

const int MAX_HW = 31;

const int MAX_CHAR_NUM = 26;

struct frame_t {
    int left;
    int right;
    int up;
    int down;
};

std::vector<std::string> g_Res;

void dfs(int (&visit)[MAX_CHAR_NUM],
        int (&pri)[MAX_CHAR_NUM][MAX_CHAR_NUM],
        std::vector<int>& path,
        int max_len) {
    int cur = path.back();
    if (path.size() == max_len) {
        char path_res[MAX_CHAR_NUM + 1];
        int pos = 0;
        for (std::vector<int>::iterator it = path.begin();
                it != path.end(); ++it) {
            if (-1 == *it) {
                continue;
            }
            path_res[pos++] = 'A' + (*it);
        }
        path_res[pos] = '\0';
        g_Res.push_back(std::string(path_res));
        return;
    }
    if (cur == -1) {
        for (int idx = 0; idx < MAX_CHAR_NUM; ++idx) {
            int idy = 0;
            int valid = 0;
            while (idy < MAX_CHAR_NUM) {
                if (pri[idx][idy] < 0) {
                    break;
                }
                if (pri[idx][idy] != 0) {
                    valid = 1;
                }
                ++idy;
            }
            if (idy == MAX_CHAR_NUM && valid == 1) {
                path.push_back(idx);
                visit[idx] = 1;
                dfs(visit, pri, path, max_len);
                visit[idx] = 0;
                path.pop_back();
            }
        }
    } else {
        for (int idx = 0; idx < MAX_CHAR_NUM; ++idx) {
            if (1 == visit[idx]) {
                continue;
            }
            int idy = 0;
            int valid = 0;
            while (idy < MAX_CHAR_NUM) {
                if (pri[idx][idy] != 0) {
                    valid = 1;
                }
                if (0 == visit[idy] && pri[idx][idy] < 0) {
                    break;
                }
                ++idy;
            }
            if (MAX_CHAR_NUM == idy && valid != 0) {
                path.push_back(idx);
                visit[idx] = 1;
                dfs(visit, pri, path, max_len);
                visit[idx] = 0;
                path.pop_back();
            }
        }
    }
}

int main() {
    int height, width;

    char frames[MAX_HW][MAX_HW];

    frame_t frame[MAX_CHAR_NUM];

    int pri[MAX_CHAR_NUM][MAX_CHAR_NUM];

    FILE* fin = fopen("frameup.in", "r");
    FILE* fout = fopen("frameup.out", "w");
    fscanf(fin, "%d %d", &height, &width);
    for (int i = 0; i < height; ++i) {
        fscanf(fin, "%s", frames[i]);
    }
    // 初始化优先表
    for (int i = 0; i < MAX_CHAR_NUM; ++i) {
        for (int j = 0; j < MAX_CHAR_NUM; ++j) {
            pri[i][j] = 0;
        }
    }
    for (int i = 0; i < MAX_CHAR_NUM; ++i) {
        frame[i].left = -1;
        frame[i].right = -1;
        frame[i].up = -1;
        frame[i].down = -1;
    }

    //
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            char cur = frames[i][j];
            if (cur > 'Z' || cur < 'A') {
                continue;
            }
            cur = cur - 'A';
            if (frame[cur].left < 0) {
                frame[cur].left = j;
            }
            if (frame[cur].up < 0) {
                frame[cur].up = i;
            }

            if (frame[cur].left > j) {
                frame[cur].left = j;
            }

            if (frame[cur].up > i) {
                frame[cur].up = i;
            }

            if (frame[cur].right < j) {
                frame[cur].right = j;
            }
            
            if (frame[cur].down < i) {
                frame[cur].down = i;
            }
        }
    }

    int char_num = 0;
    // traverse every frame
    for (int idx = 0; idx < MAX_CHAR_NUM; ++idx) {
        int up = frame[idx].up;
        int down = frame[idx].down;
        int left = frame[idx].left;
        int right = frame[idx].right;
        if (up < 0 || down < 0 || left < 0 || right < 0) {
            continue;
        }
        pri[idx][idx] = 1;
        ++char_num;
        char cur = 'A' + idx;

        for (int pos = left; pos <= right; ++pos) {
            char upside = frames[up][pos];
            char downside = frames[down][pos];
            if (upside != cur) {
                int upidx = upside - 'A';
                pri[upidx][idx] = -1;
                pri[idx][upidx] = 1;
            }
            if (downside != cur) {
                int downidx = downside - 'A';
                pri[downidx][idx] = -1;
                pri[idx][downidx] = 1;
            }
        }

        for (int pos = up; pos <= down; ++pos) {
            char leftside = frames[pos][left];
            char rightside = frames[pos][right];
            if (leftside != cur) {
                int leftidx = leftside - 'A';
                pri[leftidx][idx] = -1;
                pri[idx][leftidx] = 1;
            }
            if (rightside != cur) {
                int rightidx = rightside - 'A';
                pri[rightidx][idx] = -1;
                pri[idx][rightidx] = 1;
            }
        }
    }

    int visit[MAX_CHAR_NUM];
    std::vector<int> path;
    path.push_back(-1);
    dfs(visit, pri, path, char_num + 1);
    sort(g_Res.begin(), g_Res.end());

    for (std::vector<std::string>::iterator it = g_Res.begin();
            it != g_Res.end(); ++it) {
        fprintf(fout, "%s\n", it->c_str());
    }
     
    fclose(fin);
    fclose(fout);

    return 0;
}
