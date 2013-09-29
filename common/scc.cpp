/*
 * tarjan
 *
 * algorithm tarjan:
 * for each v in V:
 *     if v.index is not defined:
 *     scc(v)
 *
 * scc(v):
 *     v.index = index
 *     v.low = low
 *     ++index;
 *     s.push(v)
 *
 *     for w in v.neighbors:
 *         if w.index is not defined:
 *             scc(w)
 *             v.low = min(v.low, w.low)
 *         else if w in s:
 *             v.low = min(v.low, w.low)
 *
 *     if v.index == v.low:
 *         start new scc component
 *         repeat
 *             w = s.pop
 *             add w to scc
 *         until w == v
 *         output scc component
 * end scc
 */

const size_t MAX_SIZE = 1024;

struct graph_t {
    char square[MAX_SIZE][MAX_SIZE];
    size_t size;
};

class TarjanTk {
public:
    TarjanTk()
    {
        for (size_t idx = 0; idx < MAX_SIZE; ++idx) {
            _index[idx] = 0;
            _low[idx] = 0;
            _in_stack[idx] = 0;
            _cur_idx = 0;
        }
    }

    vector<size_t> _stack;
    size_t _index[MAX_SIZE];
    size_t _low[MAX_SIZE];
    size_t _in_stack[MAX_SIZE];
    int _cur_idx;
};

void func_tarjan(
        vector<<vector<size_t>>& vec_res,
        graph_t& graph,
        TarjanTk& tk,
        int index)
{
    ++tk._cur_idx;
    tk._in_stack[idx] = 1;
    tk._index[index] = tk.cur_idx;
    tk._low[index] = tk.cur_idx;
    tk._stack.push_back(index);
    for (size_t next = 0; next < graph.size; ++next) {
        if (graph.square[index][next] == 0) {
            continue;
        }
        if (graph.index[next] == 0) { // not visited yet
            func_tarjan(vec, graph, tk, next);
            if (tk._low[next] < tk._low[index]) {
                tk._low[index] = tk._low[next];
            }
        } else if (tk._in_stack[next] == 1) {
            // if next is in stack, get the min low
            if (tk._low[next] < tk._low[index]) {
                tk._low[index] = tk._low[next];
            }
        }
    }

    if (tk._index[index] == tk._low[index]) {
        vector<size_t> tmp_scc;
        do {
            size_t cur = tk._stack.back();
            tmp_scc.push_back(cur);
            tk._stack.pop_back();
            tk._in_stack[cur] = 0;
        } while (cur != index)
        vec_res.push_back(tmp_scc);
    }

}

vector<vector<size_t>> scc_tarjan(graph_t& graph)
{
    TarjanTk tarjan_tk;
    vector<vector<size_t>> vec_res;
    for (size_t idx = 0; idx < graph.size; ++idx) {
        if (tarjan_tk.index[idx] == 0) { // not visited yet
            func_tarjan(vec_res, graph, tarjan_tk, idx);
        }
    }
    return vec_res;
}

/*
 * gabow:
 * step1:
 *  find a unvisited node v, gotp step2(v);
 * step2:
 *  push v into s1, s2
 *  for w in v.neighhors:
 *      if w is unvisited:
 *          step2(w)
 *      if w is visited but not removed:
 *          maintain s2
 *      if v is on top of s2;
 *          save scc;
 */

