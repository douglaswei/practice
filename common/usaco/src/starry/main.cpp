/*
ID: weiguoz1
PROG: starry
LANG: C++
*/

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <functional>

const int MAX_WIDTH_HEIGHT = 101;
const int MAX_CLUSTER_STAR_NUM = 161;

int g_width;
int g_height;

struct node_t{
    node_t(int x, int y) {
        _x = x;
        _y = y;
    }
    int _x;
    int _y;
};

class Cluster{
private:
    int _width;
    int _height;
    size_t _sign;
    char _stars[MAX_WIDTH_HEIGHT * MAX_WIDTH_HEIGHT + 1];
public:
    void reset() {
        _width = 0;
        _height = 0;
        memset(_stars, 0, sizeof(_stars));
        _stars[MAX_WIDTH_HEIGHT * MAX_WIDTH_HEIGHT] = '\0';
    }
    void set(int x, int y, char c) {
        _stars[x * MAX_WIDTH_HEIGHT + y] = c;
        if (x > _height) {
            _height = x;
        }
        if (y > _width) {
            _width = y;
        }
    }
    char get(int x, int y) {
        return _stars[x * MAX_WIDTH_HEIGHT + y];
    }
    void rotate90(Cluster& res) {
        res.reset();
        for (int x = 0; x <= _height; ++x) {
            for (int y = 0; y <= _width; ++y) {
                char src = get(x, y);
                if (src != 0) {
                    res.set(y, _height - x, src);
                }
            }
        }
    }
    void fold(Cluster& res) {
        res.reset();
        for (int i = 0; i <= _height; ++i) {
            for (int j = 0; j <= _width; ++j) {
                char tmp = get(i, j);
                if (tmp != 0) {
                    res.set(i, _width - j, tmp);
                }
            }
        }
    }
    void uniform(Cluster& res) {
        res.reset();
        unsigned int l_height = 0;
        unsigned int l_width = -1;
        int con = 1;
        int i = 0;
        int j = 0;
        while (i <= _height) {
            j = 0;
            while (j <= _width) {
                if (_stars[i * MAX_WIDTH_HEIGHT + j] != 0) {
                    break;
                }
                ++j;
            }
            if (j <= l_width) {
                l_width = j;
            }
            if (j > g_width && con == 1) {
                ++l_height;
            } else {
                con = 0;
            }
            ++i;
        }
        for (int x = 0; x <= _height; ++x) {
            for (int y = 0; y <= _width; ++y) {
                char tmp = get(x, y);
                if (tmp != 0) {
                    res.set(x - l_height, y - l_width, tmp);
                }
            }
        }
    }
    size_t gen_sign(){
        _sign = MAX_WIDTH_HEIGHT * MAX_WIDTH_HEIGHT;
        for (int i = 0; i < MAX_WIDTH_HEIGHT * MAX_WIDTH_HEIGHT; ++i) {
            _sign = ((_sign << 5) ^ (_sign >> 27)) ^ _stars[i];
        }
        return _sign;
    }
    void get_sign(size_t (&sign)[8]) {
        Cluster rot[2];
        rot[0].reset();
        rot[1].reset();
        Cluster fol;

        for (int i = 0; i <= _height; ++i) {
            for (int j = 0; j <= _width; ++j) {
                if (get(i,j) != 0) {
                    rot[0].set(i, j, get(i, j));
                }
            }
        }

        int idx = 0;
        Cluster* cur = &rot[0];
        Cluster* next = &rot[1];
        Cluster* tmp;
        while (idx < 8) {
            cur->rotate90(*next);
            tmp = cur;
            cur = next;
            next = tmp;

            cur->fold(fol);
            sign[idx] = cur->gen_sign();
            sign[idx + 1] = fol.gen_sign();
            idx += 2;
        }
    }
};

void get_cluster(char (&org_graph)[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT],
        int x,
        int y,
        Cluster& cluster)
{
    cluster.reset();

    char mask[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT];
    memset(mask, 0, sizeof(mask));
    mask[x][y] = 1;

    std::vector<node_t> visit;
    visit.push_back(node_t(x, y));

    node_t cur_node(0, 0);
    int idx = 0;
    while (idx < visit.size()) {
        cur_node = visit[idx];
        cluster.set(cur_node._x, cur_node._y, 1);
        for (int i = cur_node._x - 1; i < cur_node._x + 2; ++i) {
            for (int j = cur_node._y - 1; j < cur_node._y + 2; ++j) {
                if (i >= 0 && i < g_height
                            && j >= 0 && j < g_width
                            && org_graph[i][j] != '0'
                            && mask[i][j] == 0) {
                    visit.push_back(node_t(i, j));
                    mask[i][j] = 1;
                }
            }
        }
        ++idx;
    }
}

void explore(char (&org_graph)[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT],
        char (&graph)[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT],
        int x,
        int y,
        std::map<size_t, char>& cmap)
{
    static char star = 'a';
    Cluster cluster;
    cluster.reset();
    // (x, y) -> cluster
    get_cluster(org_graph, x, y, cluster);
    // cluster uniform
    Cluster uniform_cluster;
    cluster.uniform(uniform_cluster);
    // cluster -> sign
    char flag;
    size_t sign[8];
    uniform_cluster.get_sign(sign);
    if (cmap.find(sign[0]) != cmap.end()) {
        flag = cmap[sign[0]];
    } else {
        flag = star;
        for (int i = 0; i < 8; i++) {
            cmap[sign[i]] = star;
        }
        ++star;
    }
    // rewrite graph
    for (int i = 0; i < g_height; ++i) {
        for (int j = 0; j < g_width; ++j) {
            if (cluster.get(i, j) != 0) {
                graph[i][j] = flag;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    char org_graph[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT];
    char graph[MAX_WIDTH_HEIGHT][MAX_WIDTH_HEIGHT];
    FILE* fin = fopen("starry.in", "r");
    FILE* fout = fopen("starry.out", "w");
    fscanf(fin, "%d", &g_width);
    fscanf(fin, "%d", &g_height);
    for (int i = 0; i < g_height; ++i) {
        fscanf(fin, "%s", org_graph[i]);
        memset(graph[i], '0', strlen(org_graph[i]));
        graph[i][strlen(org_graph[i])] = '\0';
    }

    std::map<size_t, char> map_sign_c;
    Cluster cluster;
    for (int i = 0; i < g_height; ++i) {
        for (int j = 0; j < g_width; ++j) {
            if (org_graph[i][j] != '0' && graph[i][j] == '0') {
                explore(org_graph, graph, i, j, map_sign_c);
            }
        }
    }
    for (int i = 0; i < g_height; ++i) {
        fprintf(fout, "%s\n", graph[i]);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
