/*
ID: weiguoz1
PROG: fc
LANG: C++
*/

#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>


struct node_t {
    double _x;
    double _y;
    double _angle;
};

const int MAX_NODE_NUM = 10000;

const double MAX_ANGLE = __FLT_MAX__;
const double MIN_ANGLE = __FLT_MIN__;

class MyCompare {
public:
    MyCompare(node_t* nodes) {
        _nodes = nodes;
    }

    node_t* _nodes;

    bool operator() (const int& l, const int& r) {
        if (_nodes[l]._angle < _nodes[r]._angle) {
            return true;
        } else if ( _nodes[l]._angle == _nodes[r]._angle) {
            double l_dis = pow(_nodes[l]._x - _nodes[0]._x, 2)
                    + pow(_nodes[l]._y - _nodes[0]._y, 2);
            double r_dis = pow(_nodes[r]._x - _nodes[0]._x, 2)
                    + pow(_nodes[r]._y - _nodes[0]._y, 2);
            if (l_dis > r_dis) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    FILE* fin = fopen("fc.in", "r");
    FILE* fout = fopen("fc.out", "w");

    int node_num;
    node_t nodes[MAX_NODE_NUM];
    std::vector<int> fence;

    int tmp_idx = 0;

    fscanf(fin, "%d", &node_num);
    for (int i = 0; i < node_num; ++i) {
        fscanf(fin, "%lf %lf", &nodes[i]._x, &nodes[i]._y);
        if (nodes[tmp_idx]._x > nodes[i]._x) {
            tmp_idx = i;
        } else if (nodes[tmp_idx]._x == nodes[i]._x
                && nodes[tmp_idx]._y < nodes[i]._y ) {
            tmp_idx = i;
        }
    }
    for (int i = 0; i < node_num; ++i) {
        double den = nodes[i]._x - nodes[tmp_idx]._x;
        double mol = nodes[i]._y - nodes[tmp_idx]._y;
        nodes[i]._angle = atan2(mol, den);
    }

    fence.push_back(tmp_idx);

    for (int i = 0; i < node_num; ++i) {
        if (i != tmp_idx) {
            fence.push_back(i);
        }
    }

    sort(fence.begin() + 1, fence.end(), MyCompare(nodes));

    /*
    int valid = false;
    int idx = 0;

    while (!valid && fence.size() > 3) {
        valid = true;
        while (idx < fence.size()) {
            int prev = idx - 1;
            int next = idx + 1;
            if (prev < 0) {
                prev = fence.size() - 1;
            }
            if (next >= fence.size()) {
                next = 0;
            }
            int cur = fence[idx];
            prev = fence[prev];
            next = fence[next];
            double lx = nodes[prev]._x - nodes[cur]._x;
            double ly = nodes[prev]._y - nodes[cur]._y;
            double rx = nodes[next]._x - nodes[cur]._x;
            double ry = nodes[next]._y - nodes[cur]._y;
            double res = lx * ry - ly * rx;
            if (res >= 0 && idx != 0) {
                fence.erase(fence.begin() + idx);
                valid = false;
                break;
            }
            ++idx;
        }
    }
    */

    /*
    int idx = 0;
    while (idx < fence.size()) {
        int prev = idx - 1;
        int next = idx + 1;
        if (prev < 0) {
            prev = fence.size() - 1;
        }
        if (next >= fence.size()) {
            next = 0;
        }
        int cur = fence[idx];
        prev = fence[prev];
        next = fence[next];
        double lx = nodes[prev]._x - nodes[cur]._x;
        double ly = nodes[prev]._y - nodes[cur]._y;
        double rx = nodes[next]._x - nodes[cur]._x;
        double ry = nodes[next]._y - nodes[cur]._y;
        double res = lx * ry - ly * rx;
        if (res >= 0 && idx != 0) {
            fence.erase(fence.begin() + idx);
            continue;
        }
        ++idx;
    }
    
    double length = 0;
    for (int idx = 0; idx < fence.size(); ++idx) {
        int next = idx + 1;
        if (next >= fence.size()) {
            next -= fence.size();
        }
        int cur = fence[idx];
        next = fence[next];
        double cur_len = pow(nodes[cur]._x - nodes[next]._x, 2)
                + pow(nodes[cur]._y - nodes[next]._y, 2);
        cur_len = sqrt(cur_len);
        length += cur_len;
    }
    */

    std::vector<int> vec_res;

    int idx = 0;
    while (idx < fence.size()) {
        int cur = fence[idx];
        if (idx < 3) {
            vec_res.push_back(cur);
            ++idx;
            continue;
        }
        cur = vec_res.back();
        int next = fence[idx];
        int prev = vec_res[vec_res.size() - 2];
        double lx = nodes[prev]._x - nodes[cur]._x;
        double ly = nodes[prev]._y - nodes[cur]._y;
        double rx = nodes[next]._x - nodes[cur]._x;
        double ry = nodes[next]._y - nodes[cur]._y;
        double res = lx * ry - ly * rx;
        if (res >= 0) {
            vec_res.pop_back();
            continue;
        } else {
            vec_res.push_back(next);
        }
        ++idx;
    }
    
    double length = 0;
    for (int idx = 0; idx < vec_res.size(); ++idx) {
        int next = idx + 1;
        if (next >= vec_res.size()) {
            next -= vec_res.size();
        }
        int cur = vec_res[idx];
        next = vec_res[next];
        double cur_len = pow(nodes[cur]._x - nodes[next]._x, 2)
                + pow(nodes[cur]._y - nodes[next]._y, 2);
        cur_len = sqrt(cur_len);
        length += cur_len;
    }

    fprintf(fout, "%.2f\n", length);

    fclose(fin);
    fclose(fout);

    return 0;

}
