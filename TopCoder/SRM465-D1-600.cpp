// Reduce to minimum weight bipartite vertex cover
// Can be solved using maximum flow, by using a similar network to bipartite maximum cardinality matching
#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9+1;

struct edge {
  int u, v, cap, flow;
  edge(int u, int v, int cap): u(u), v(v), cap(cap), flow(0) {}
};

struct GreenWarfare { // s = n-2, t = n-1;
  int n;
  vector<int> ptr, lvl;
  vector<vector<int> > g;
  vector<edge> edges;

  bool bfs() {
    queue<int> q;
    lvl = vector<int>(n, -1);
    q.push(n-2);
    lvl[n-2] = 0;
    while(!q.empty()) {
      int u = q.front();
      q.pop();
      for(int e: g[u]) if(edges[e].cap - edges[e].flow > 0 && lvl[edges[e].v] == -1) {
        lvl[edges[e].v] = lvl[u] + 1;
        q.push(edges[e].v);
      }
    }
    return lvl[n-1] != -1;
  }
  int dfs(int u, int pushed) {
    if(!pushed) return 0;
    if(u == n-1) return pushed;
    for(; ptr[u] < int(g[u].size()); ++ptr[u]) {
      int e = g[u][ptr[u]];
      int v = edges[e].v;
      if(edges[e].cap - edges[e].flow <= 0 || lvl[v] != lvl[u] + 1) continue;
      int possible_push = dfs(v, min(pushed, edges[e].cap - edges[e].flow));
      edges[e].flow += possible_push;
      edges[e^1].flow -= possible_push;
      if(possible_push) return possible_push;
    }
    return 0;
  }
  void add_edge(int u, int v, int cap) {
    edges.emplace_back(u, v, cap);
    edges.emplace_back(v, u, 0);
    g[u].push_back(edges.size() - 2);
    g[v].push_back(edges.size() - 1);
  }

  int dist2(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  }
  int minimumEnergyCost(vector<int> canonX, vector<int> canonY, vector<int> baseX, vector<int> baseY, vector<int> plantX, vector<int> plantY, int energySupplyRadius) {
    n = int(baseX.size()) + int(plantX.size()) + 2;
    g = vector<vector<int> >(n);
    ptr = vector<int>(n, 0);
    vector<int> baseE(int(baseX.size()), INF), plantE(int(plantX.size()), INF);

    for(int i = 0; i < int(baseX.size()); i++) {
      for(int j = 0; j < int(canonX.size()); j++) {
        baseE[i] = min(dist2(baseX[i], baseY[i], canonX[j], canonY[j]), baseE[i]);
      }
    }
    for(int i = 0; i < int(plantX.size()); i++) {
      for(int j = 0; j < int(canonX.size()); j++) {
        plantE[i] = min(dist2(plantX[i], plantY[i], canonX[j], canonY[j]), plantE[i]);
      }
    }
    for(int i = 0; i < int(baseX.size()); i++) add_edge(n-2, i, baseE[i]);
    for(int i = 0; i < int(plantX.size()); i++) add_edge(int(baseX.size()) + i, n-1, plantE[i]);
    for(int i = 0; i < int(baseX.size()); i++) for(int j = 0; j < int(plantX.size()); j++)
      if(dist2(baseX[i], baseY[i], plantX[j], plantY[j]) <= energySupplyRadius * energySupplyRadius)
        add_edge(i, int(baseX.size()) + j, INF);
    int ans = 0;
    while(bfs()) {
      ptr = vector<int>(n, 0);
      while(int pushed = dfs(n-2, INF)) ans += pushed;
    }
    return ans;
  }
};
