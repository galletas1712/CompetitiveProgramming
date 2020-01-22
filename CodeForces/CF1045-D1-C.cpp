#include <bits/stdc++.h>
using namespace std;
const int N = 2e5+1;

int n, m, q, ccs;
vector<int> g[N], bcg[2*N];

int pre[N], lo[N];
stack<int> stk;
void dfs(int u, int par) {
  static int tick = 0;
  pre[u] = lo[u] = ++tick;
  stk.push(u);
  for(int v: g[u]) if(v != par) {
    if(!pre[v]) {
      dfs(v, u);
      lo[u] = min(lo[v], lo[u]);
      if(lo[v] >= pre[u]) {
        bcg[u].push_back(n + ccs);
        while(bcg[n + ccs].empty() || bcg[n + ccs].back() != v) {
          bcg[n + ccs].push_back(stk.top());
          stk.pop();
        }
        ++ccs;
      }
    } else {
      lo[u] = min(pre[v], lo[u]);
    }
  }
}

int d[N], sum[N];
vector<int> anc[N];
void init_lca(int u, int par) {
  for(int v: bcg[u]) if(v != par) {
    d[v] = d[u] + 1;
    sum[v] = sum[u] + (v >= n);
    anc[v].push_back(u);
    for(int j = 0; 1 << j+1 <= d[v]; j++)
      anc[v].push_back(anc[anc[v][j]][j]);
    init_lca(v, u);
  }
}

int get_lca(int u, int v) {
  if(d[u] < d[v]) swap(u, v);
  for(int j = 18; j >= 0; j--) if(d[u] - d[v] >> j & 1) u = anc[u][j];
  if(u == v) return u;
  for(int j = 18; j >= 0; j--) if(j < anc[u].size() && anc[u][j] != anc[v][j])
    u = anc[u][j], v = anc[v][j];
  return anc[u][0];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n >> m >> q;
  for(int i = 0, u, v; i < m; i++) {
    cin >> u >> v;
    --u, --v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(0, -1);
  init_lca(0, -1);
  for(int i = 0, u, v; i < q; i++) {
    cin >> u >> v;
    --u, --v;
    int lca = get_lca(u, v);
    cout << sum[u] + sum[v] - sum[lca] - (lca ? sum[anc[lca][0]] : 0) << '\n';
  }
}
