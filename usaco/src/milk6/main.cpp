/*
ID: weiguoz1
PROG: milk6
LANG: C++
*/

#include <stdio.h>
#include <limits.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iostream>

const int MAX_NODE_NUM = 33;
const int MAX_EDGE_NUM = 1000;
const unsigned long long MAX_FLOW = ULLONG_MAX;

namespace aug_path {

int dfs_rec(
        int (&graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        int n,
        int* visit,
        std::vector<int>& path)
{
   int from = path.back();
   if (from == n) {
       return MAX_FLOW;
   }
   int next_flow = 0;
   path.push_back(0);
   for (int to = 2; to <= n; ++to) {
       int cur_flow = graph[from][to];
       if ((cur_flow > 0) && (0 == visit[to])) {
           path.back() = to;
           visit[to] = 1;
           next_flow = dfs_rec(graph, n, visit, path);
           if (next_flow > 0) {
               return (next_flow > cur_flow ? cur_flow : next_flow);
           } else {
               visit[to] = 0;
           }
       }
   }
   path.pop_back();
   return 0;
}

int dfs(
        int (&graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        int n,
        std::vector<int>& path)
{
    int visit[MAX_NODE_NUM];
    memset(visit, 0, sizeof(visit));
    visit[1] = 1;
    path.clear();
    path.push_back(1);
    while (true) {
        if (path.size() == 0) {
            break;
        }
        int cur = path.back();
        if (cur == n) {
            int flow = MAX_FLOW;
            for (int i = 0; i < path.size() - 1; ++i) {
                int from = path[i];
                int to = path[i + 1];
                flow = flow > graph[from][to] ? graph[from][to] : flow;
            }
            return flow;
        }
        int valid_path = 0;
        for (int to = 2; to <= n; ++to) {
            if ((visit[to] == 0) && (graph[cur][to] > 0)) {
                path.push_back(to);
                visit[to] = 1;
                valid_path = 1;
                break;
            }
        }
        if (0 == valid_path) {
            int valid_next = 0;
            while (0 == valid_next) {
                cur = path.back();
                path.pop_back();
                visit[cur] = 0;
                if (path.size() == 0) {
                    break;
                }
                int prev = path.back();
                for (int next = cur + 1; next <= n; ++next) {
                    if ((visit[next] == 0) && (graph[prev][next] > 0)) {
                        visit[next] = 1;
                        path.push_back(next);
                        valid_next = 1;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

unsigned long long bfs(
        unsigned long long (&graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        int n,
        std::vector<int>& path)
{
    path.clear();
    int prev[MAX_NODE_NUM];
    memset(prev, 0, sizeof(prev));
    std::vector<int> visit_vec;
    visit_vec.reserve(MAX_NODE_NUM);
    visit_vec.push_back(1);
    std::vector<int>::iterator it = visit_vec.begin();
    while (it != visit_vec.end()) {
        if (*it == n) {
            break;
        }
        int from = *it;
        for (int to = 2; to <= n; ++to) {
            unsigned long long cur_flow = graph[from][to];
            if (to != 1 && prev[to] == 0 && cur_flow > 0) {
                prev[to] = from;
                visit_vec.push_back(to);
            }
        }
        ++it;
    }
    if (it == visit_vec.end()) {
        return 0;
    }
    int cur = n;
    unsigned long long flow = MAX_FLOW;
    while (true) {
        path.push_back(cur);
        if (1 == cur) {
            break;
        }
        int prev_node = prev[cur];
        unsigned long long cur_flow = graph[prev_node][cur];
        if (cur_flow < flow) {
            flow = cur_flow;
        }
        cur = prev_node;
    }
    std::reverse(path.begin(), path.end());
    return flow;
}

unsigned long long calc_aug_path(
        unsigned long long (&graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        int n,
        int* path,
        int& path_len)
{
    std::vector<int> vec_path;

    // dfs_rec
    /* dfs_rec
    int visit[MAX_NODE_NUM];
    memset(visit, 0, sizeof(visit));
    vec_path.push_back(1);
    visit[1] = 1;
    int flow = dfs_rec(graph, n, visit, vec_path);
    for (std::vector<int>::iterator it = vec_path.begin();
            it != vec_path.end(); ++it) {
        path[path_len++] = *it;
    }
    path_len = vec_path.size();
    return flow;
    dfs_rec */

    // dfs
    /* dfs
    int flow = dfs(graph, n, vec_path);
    for (std::vector<int>::iterator it = vec_path.begin();
            it != vec_path.end(); ++it) {
        path[path_len++] = *it;
    }
    path_len = vec_path.size();
    return flow;
    dfs */

    // bfs
    /* bfs
    bfs */
    unsigned long long flow = bfs(graph, n, vec_path);

    for (std::vector<int>::iterator it = vec_path.begin();
            it != vec_path.end(); ++it) {
        path[path_len++] = *it;
    }
    path_len = vec_path.size();
    return flow;
}

} // namespace aug_path

unsigned long long calc_max_flow(
        int n,
        unsigned long long (&graph)[MAX_NODE_NUM][MAX_NODE_NUM])
{
    unsigned long long total_flow = 0;
    int path[MAX_NODE_NUM];
    while (true) {
        int path_len = 0;
        unsigned long long flow = aug_path::calc_aug_path(graph, n, path, path_len);
        if (0 == flow) {
            break;
        }
        total_flow += flow;
        for (int i = 0; i < path_len - 1; ++i) {
            int from = path[i];
            int to = path[i+1];
            graph[from][to] -= flow;
            graph[to][from] += flow;
        }
    }
    return total_flow;
}

class MyCompare {
public:
    MyCompare(const unsigned long long cap[MAX_EDGE_NUM][3]) {
        for (int i = 0; i < MAX_EDGE_NUM; ++i) {
            _cap[i] = cap[i][2];
        }
    }
    int _cap[MAX_EDGE_NUM];
    bool operator() (const int& l, const int& r) {
        return (_cap[l] > _cap[r]);
    }
};

void calc_min_cut(
        const int n,
        const int m,
        const unsigned long long max_flow,
        unsigned long long (&org_graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        unsigned long long (&graph)[MAX_NODE_NUM][MAX_NODE_NUM],
        unsigned long long (&edge)[MAX_EDGE_NUM][3],
        std::vector<int>& cut)
{
    // bfs;
    std::vector<int> cut_candidate;
    MyCompare my_compare(edge);

    int min_cut_num = (max_flow % (500000 * 1001)) / 500000;
    for (int from = 1; from <= n; ++from) {
        for (int to = 1; to <= n; ++to) {
            if (org_graph[from][to] > 0 && 0 == graph[from][to]) {
                for (int i = 0; i < m; ++i) {
                    if (edge[i][0] == from && edge[i][1] == to) {
                        cut_candidate.push_back(i);
                    }
                }
            }
        }
    }

    int base = cut_candidate.size();
    int num = min_cut_num;

    // traverse C(num, c)
    int cur[MAX_EDGE_NUM];
    // init cur
    int init_stat = 1;
    for (int i = 0; i < base; ++i) {
        cur[i] = 0;
    }
    while (true) {
        if (1 == init_stat) {
            for (int i = 0; i < num; ++i) {
                cur[i] = 1;
            }
            init_stat = 0;
        } else {
            int idx = base - 1;
            int num_one = 0;
            // r_find the first 0
            while (idx >= 0) {
                if (0 == cur[idx]) {
                    break;
                } else {
                    ++num_one;
                }
                --idx;
            }
            if (idx < 0) {
                break;
            }
            --idx;
            // r_find the first 1
            while (idx >= 0) {
                if (1 == cur[idx]) {
                    break;
                }
                --idx;
            }
            if (idx < 0) {
                break;
            }
            cur[idx] = 0;
            ++idx;
            while (idx < base) {
                if (num_one >= 0) {
                    cur[idx] = 1;
                    --num_one;
                } else {
                    cur[idx] = 0;
                }
                ++idx;
            }
        }
        unsigned long long flow_sum = 0;
        // 统计此种组合的流量
        for (int i = 0; i< base; ++i) {
            if (cur[i] != 0) {
                flow_sum += edge[cut_candidate[i]][2];
            }
        }
        if (flow_sum == max_flow) {
            for (int i = 0; i <= n; ++i) {
                for (int j = 0; j <= n; ++j) {
                    graph[i][j] = org_graph[i][j];
                }
            }
            for (int i = 0; i < base; ++i) {
                if (cur[i] != 0) {
                    int edge_idx = cut_candidate[i];
                    int from = edge[edge_idx][0];
                    int to = edge[edge_idx][1];
                    graph[from][to] -= edge[edge_idx][2];
                }
            }
            int path[MAX_NODE_NUM];
            int path_len = 0;
            int flow = aug_path::calc_aug_path(graph, n, path, path_len);
            if (0 == flow) {
            // 填充cut，准备返回
                for (int i = 0; i < base; ++i) {
                    if (cur[i] != 0) {
                        cut.push_back(cut_candidate[i] + 1);
                    }
                }
                sort(cut.begin(), cut.end());
                return;
            }
        }
    }

}

int main()
{
    int n;
    int m;
    unsigned long long org_graph[MAX_NODE_NUM][MAX_NODE_NUM];
    unsigned long long graph[MAX_NODE_NUM][MAX_NODE_NUM];
    unsigned long long edge[MAX_EDGE_NUM][3];
    std::vector<int> cut;

    FILE* fin = fopen("milk6.in", "r");
    FILE* fout = fopen("milk6.out", "w");
    if (NULL == fin || NULL == fout) {
        return -1;
    }
    int from, to;
    unsigned long long capacity;
    fscanf(fin, "%d %d", &n, &m);

    for(int i = 0; i < MAX_NODE_NUM; ++i) {
        for(int j = 0; j < MAX_NODE_NUM; ++j) {
            org_graph[i][j] = 0;
            graph[i][j] = 0;
        }
    }
    for (int i = 0; i < m; ++i) {
        fscanf(fin, "%d %d %llu", &from, &to, &capacity);
        capacity = 500000 + i + 500000 * 1001 * capacity;
        org_graph[from][to] += capacity;
        graph[from][to] += capacity;
        edge[i][0] = from;
        edge[i][1] = to;
        edge[i][2] = capacity;
    }

    unsigned long long total_flow = calc_max_flow(n, graph);

    calc_min_cut(n, m, total_flow, org_graph, graph, edge, cut);

    total_flow = total_flow / 500000 / 1001;
    fprintf(fout, "%u ", total_flow);
    fprintf(fout, "%u\n", cut.size());

    if (cut.size() > 0) {
        for (std::vector<int>::iterator it = cut.begin(); it != cut.end(); ++it) {
            fprintf(fout, "%d\n", *it);
        }
    }

    fclose(fin);
    fclose(fout);
}
