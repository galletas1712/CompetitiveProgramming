#include "Boblib.h"
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
const int K = 10;

void Bob(int N, int M, int A[], int B[]) {
    if(N == 1) {
        InitMap(N, M);
        return;
    }
    if(N == 2) {
        InitMap(N, M);
        for(int i = 0; i < M; i++) MakeMap(A[i], B[i]);
        return;
    }
    std::vector<int> g[N];
    for(int i = 0; i < M; i++) {
        g[A[i]].push_back(B[i]);
        g[B[i]].push_back(A[i]);
    }

    //find special vertex
    std::vector<int> deg1;
    for(int i = 0; i < N; i++) if(g[i].size() == 1) deg1.push_back(i);
    assert(deg1.size() <= 2); // there can only be at most two vertices is degree one: max bit and very special vertex
    int very_special_vertex = deg1[0]; // vertex of degree 1 that points to the special vertex
    int special_vertex = g[deg1[0]][0];
    if(deg1.size() == 2) {
        if(g[special_vertex].size() != N - K - 1) {
            very_special_vertex = deg1[1];
            special_vertex = g[deg1[1]][0];
        }
    }

    //find bit vertices
    std::vector<int> bit_vertices;
    std::vector<bool> is_bit_vertex(N);
    for(int i = 0; i < N; i++) if(i != special_vertex) {
        if(std::find(g[i].begin(), g[i].end(), special_vertex) == g[i].end()) {
            bit_vertices.push_back(i);
            is_bit_vertex[i] = true;
        }
    }
    assert(bit_vertices.size() == K);

    // determine order of bit vertices and add 1 << ord to each of the original vertices at the same time
    int mx_bit_vertex = *min_element(bit_vertices.begin(), bit_vertices.end(), [&] (int i, int j) { return g[i].size() < g[j].size(); });
    // observe that maximum bit vertex will be the one with the minimum degree
    std::vector<int> vals(N);
    for(int ord = K - 1, curr = mx_bit_vertex; ord >= 0; ord--) {
        for(int v: g[curr]) {
            assert(v != very_special_vertex && v != special_vertex);
            if(is_bit_vertex[v]) continue;
            vals[v] |= 1 << ord;
        }

        for(int v: g[curr]) {
            if(is_bit_vertex[v]) {
                g[v].erase(std::find(g[v].begin(), g[v].end(), curr)); // total erase complexity is n^2
                curr = v;
                break;
            }
        }
    }

    auto in_org_graph = [&] (int v) -> bool { return v != very_special_vertex && v != special_vertex && !is_bit_vertex[v]; };
    std::vector<std::pair<int,int> > ans;
    for(int i = 0; i < M; i++) {
        if(in_org_graph(A[i]) && in_org_graph(B[i])) ans.emplace_back(vals[A[i]] - 1, vals[B[i]] - 1);
    }
    InitMap(N - K - 2, ans.size());
    for(auto p: ans) MakeMap(p.first, p.second);
}
