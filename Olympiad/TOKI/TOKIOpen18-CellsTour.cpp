// Construct bipartite graph of x -> y
// The row swap and column swap operations only switch the labels on the nodes
// Graph remains isomorphic
// We want to have to maximum number of points on the diagonal, which means we will select a matching from the bipartite graph
// For each pair, we can make their labels equal in order to put it on the main diagonal (but not needed in implementation, just the idea)
// So, the problem reduces to finding a maximum matching on this bipartite graph
// Can be done in O(VE) = O(N^3) using a recursive greedy algorithm
#include "tour.h"
#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9+1;
const int N = 301;

int n;
vector<string> s;
vector<vector<int> > g;
vector<int> mp;
vector<bool> block;
bool can_match(int u) {
  for(int v: g[u]) if(!block[v]) {
    block[v] = true;
    if(mp[v] == -1 || can_match(mp[v])) {
      mp[v] = u;
      return true;
    }
  }
  return false;
}

int getShortestTour(int _n, vector<string> _s) {
  n = _n, s = _s;
  g = vector<vector<int> >(2*n);
  mp = vector<int>(2*n, -1);
  block = vector<bool>(2*n, 0);
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) if(s[i][j] == '.') {
    g[i].push_back(j + n);
    g[j + n].push_back(i);
  }
  int sz = 0;
  for(int i = 0; i < n; i++) {
    block.assign(2*n, 0);
    if(can_match(i)) ++sz;
  }
  sz = min(n-1, sz);
  return 2*(n-1) - sz;
}
