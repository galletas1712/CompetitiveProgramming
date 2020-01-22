// https://ivaniscoding.wordpress.com/2018/08/27/communication-4-airline-route-map/
#include "Alicelib.h"
#include <cassert>
#include <cstdio>
#include <vector>
#include <utility>
#include <iostream>
const int K = 10;

void Alice( int N, int M, int A[], int B[] ){
    if(N == 1) {
        InitG(N, M);
        return;
    }
    if(N == 2) {
        InitG(N, M);
        if(M) MakeG(0, 0, 1);
        return;
    }
	std::vector<std::pair<int, int> > edges;
	for(int i = 0; i < M; i++) {
		edges.emplace_back(A[i], B[i]); // original graph
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < K; j++) {
			if(!((i + 1) >> j & 1)) continue; // make graph 1-indexed, so connect to corresponding nodes
			edges.emplace_back(i, N + j); // links between bit vertices and original vertices
		}
	}
	for(int j = 0; j < K - 1; j++) edges.emplace_back(N + j, N + j + 1); // links between bit vertices
	for(int i = 0; i < N; i++) {
		edges.emplace_back(i, N + K); // N + 10 is the special vertex
	}
	edges.emplace_back(N + K, N + K + 1); // N + 11 is the one-degree vertex
	int curr_idx = 0;
	InitG(N + K + 2, edges.size());
	for(auto p: edges) MakeG(curr_idx++, p.first, p.second);
}
