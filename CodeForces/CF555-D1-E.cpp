// Always possible to do it in a 2-edge-connected component
// Make a bridge tree and check condition on tree
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1;

int n, m, q, cc_cnt;
vector<int> g[N], gcc[N];
vector<pair<int,int> > bedges;
int f[N][2];

int pre[N], low[N], id[N], tick;
stack<int> stk;
void dfs1(int u, int p) {
  pre[u] = low[u] = ++tick;
  bool seen_par = false;
  stk.push(u);
  for(int v: g[u]) {
    if(!pre[v]) {
      dfs1(v, u);
      low[u] = min(low[v], low[u]);
      if(low[v] > pre[u]) {
        bedges.emplace_back(u, v);
        ++cc_cnt;
        while(stk.top() != v) {
          id[stk.top()] = cc_cnt;
          stk.pop();
        }
        id[v] = cc_cnt;
        stk.pop();
      }
    } else {
      if(v != p || (v == p && seen_par))
      low[u] = min(pre[v], low[u]);
      if(v == p) seen_par = true;
    }
  }
}

int d[N];
vector<int> anc[N];
bool mark[N];
void init_lca(int u, int p) {
  mark[u] = true;
  for(int v: gcc[u]) if(v != p) {
    d[v] = d[u] + 1;
    anc[v].push_back(u);
    for(int j = 0; 1 << j+1 <= d[v]; j++)
      anc[v].push_back(anc[anc[v][j]][j]);
    init_lca(v, u);
  }
}
int get_lca(int u, int v) {
  if(d[u] < d[v]) swap(u, v);
  for(int j = 17; j >= 0; j--) if(d[u] - d[v] >> j & 1) u = anc[u][j];
  if(u == v) return u;
  for(int j = 17; j >= 0; j--) if(j < anc[u].size() && anc[u][j] != anc[v][j])
    u = anc[u][j], v = anc[v][j];
  return (anc[u].empty() ? -1 : anc[u][0]);
}

bool dfs2(int u, int p) {
  bool ret = true;
  mark[u] = true;
  for(int v: gcc[u]) if(v != p) {
    ret &= dfs2(v, u);
    ret &= !f[v][0] || !f[v][1];
    f[u][0] += f[v][0];
    f[u][1] += f[v][1];
  }
  return ret;
}

void solve() {
  cin >> n >> m >> q;
  for(int i = 0, u, v; i < m; i++) {
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  for(int i = 1; i <= n; i++) if(!pre[i]) {
    dfs1(i, 0);
    if(!stk.empty()) ++cc_cnt;
    while(!stk.empty()) {
      id[stk.top()] = cc_cnt;
      stk.pop();
    }
  }
  for(auto e: bedges) {
    int u, v;
    tie(u, v) = e;
    u = id[u], v = id[v];
    gcc[u].push_back(v);
    gcc[v].push_back(u);
  }
  for(int i = 1; i <= cc_cnt; i++) if(!mark[i]) init_lca(i, 0);
  while(q--) {
    int u, v;
    cin >> u >> v;
    u = id[u], v = id[v];
    int lca = get_lca(u, v);
    if(lca == -1) return void(cout << "No" << endl);
    ++f[u][0];
    --f[lca][0];
    ++f[v][1];
    --f[lca][1];
  }
  fill_n(mark, N, 0);
  bool ans = true;
  for(int i = 1; i <= cc_cnt; i++) if(!mark[i]) ans &= dfs2(i, 0);
  if(ans) cout << "Yes" << endl;
  else cout << "No" << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  solve();
}
