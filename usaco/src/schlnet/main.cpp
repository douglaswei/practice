/*
ID: weiguoz1
PROG: schlnet
LANG: C++
*/

#include <stdio.h>
#include <iostream>
#include <vector>

using std::vector;

const int MAX_VERTEX_NUM = 1000;

struct graph_t {
    char square[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    size_t size;
};

class GabowToolkit {
public:
    GabowToolkit()
    {
        _path.reserve(MAX_VERTEX_NUM);
        _stack.reserve(MAX_VERTEX_NUM);
        for (size_t idx = 0; idx < MAX_VERTEX_NUM; ++idx) {
            _index[idx] = 0;
            _scc_num[idx] = 0;
            _in_num[idx] = 0;
            _out_num[idx] = 0;
        }
        _cur_index = 1;
        _cur_scc_idx = 0;
    }
    vector<size_t> _path;
    vector<size_t> _stack;
    size_t _index[MAX_VERTEX_NUM];
    size_t _scc_num[MAX_VERTEX_NUM];
    size_t _cur_index;
    size_t _cur_scc_idx;
    size_t _in_num[MAX_VERTEX_NUM];
    size_t _out_num[MAX_VERTEX_NUM];
};

void func_gabow(const graph_t& graph, GabowToolkit& tk, size_t idx)
{
    tk._index[idx] = tk._cur_index;
    ++tk._cur_index;
    tk._path.push_back(idx);
    tk._stack.push_back(idx);

    for (size_t next = 0; next < graph.size; ++next) {
        if (graph.square[idx][next] == 0) {
            continue;
        }
        if (tk._index[next] == 0) {
            func_gabow(graph, tk, next);
        } else if (tk._scc_num[next] == 0) {
            while (tk._index[tk._stack.back()] > tk._index[next]) {
                if (tk._stack.size() == 1) {
                    std::cout<<"idx:"<<idx<<"next:"<<next<<std::endl;
                    std::cout<<"err:"<<std::endl;
                    for (size_t i = 0; i < tk._path.size(); ++i)
                        std::cout<<tk._path[i]<<std::endl;
                    std::cout<<"stack:"<<tk._stack.back()<<std::endl;
                }
                tk._stack.pop_back();
            }
        }
    }

    if (idx != tk._stack.back()) {
        return;
    }

    tk._cur_scc_idx++;

    tk._stack.pop_back();
    size_t cur = 0;
    do {
        cur = tk._path.back();
        tk._path.pop_back();
        tk._scc_num[cur] = tk._cur_scc_idx;
    } while (cur != idx);
}

void calc_scc(const graph_t& graph, GabowToolkit& tk)
{
    for (size_t idx = 0; idx < graph.size; ++idx) {
        if (tk._index[idx] == 0) {
            func_gabow(graph, tk, idx);
        }
    }
}

int main(int argc, char* argv[])
{
    FILE* fin = fopen("schlnet.in", "r");
    FILE* fout = fopen("schlnet.out", "w");

    graph_t* graph = new graph_t;
    GabowToolkit tk;

    fscanf(fin, "%zu", &graph->size);
    
    for (int i = 0; i < graph->size; ++i) {
        for (int j = 0; j < graph->size; ++j) {
            graph->square[i][j] = 0;
        }
    }

    for (int i = 0; i < graph->size; ++i) {
        size_t cur = 0;
        do {
            fscanf(fin, "%zu", &cur);
            if (cur != 0) {
                graph->square[i][cur - 1] = 1;
            }
        } while (cur != 0);
    }

    calc_scc(*graph, tk);

    size_t res_a = 0;
    size_t res_b = 0;

    for (size_t from = 0; from < graph->size; ++from) {
        for (size_t to = 0; to < graph->size; ++to) {
            if (graph->square[from][to] != 0) {
                if (tk._scc_num[from] != tk._scc_num[to]) {
                    tk._out_num[tk._scc_num[from]]++;
                    tk._in_num[tk._scc_num[to]]++;
                }
            }
        }
    }

    for (size_t idx = 1; idx <= tk._cur_scc_idx; ++idx) {
        if (tk._in_num[idx] == 0) {
            ++res_a;
        }
    }

    if (res_a == 0) {
        res_a = 1;
    }

    if (tk._cur_scc_idx == 1) {
        res_b = 0;
    } else {
        size_t zero_in_num = 0;
        size_t zero_out_num = 0;

        for (size_t idx = 1; idx <= tk._cur_scc_idx; ++idx) {
            if (tk._in_num[idx] == 0) {
                zero_in_num++;
            }
            if (tk._out_num[idx] == 0) {
                zero_out_num++;
            }
        }
        res_b = zero_out_num;
        if (res_b < zero_in_num) {
            res_b = zero_in_num;
        }
    }

    fprintf(fout, "%zu\n%zu\n", res_a, res_b);

    fclose(fin);
    fclose(fout);
}

