#include <bits/stdc++.h>
using namespace std;
const int N = 1001;

int n, m;
vector<pair<int,int> > edges;
vector<int> g[N];
bool adj[N][N], used[N], blocked[N];

struct DSU {
  vector<int> dsu;
  DSU(int n) {
    dsu = vector<int>(n+1, -1);
  }
  void clear() {
    dsu.assign(n+1, -1);
  }
  int root(int v) { return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v])); }
  void merge(int u, int v) {
      if((u = root(u)) == (v = root(v))) return;
      dsu[u] += dsu[v];
      dsu[v] = u;
  }
};

void dfs(int u, DSU &dsu) {
  used[u] = true;
  if(blocked[u]) return;
  for(int v: g[u]) if(!used[v]) {
    dfs(v, dsu);
    dsu.merge(u, v);
  }
}

void bfs(int x, int y, int rem) {
  vector<int> dist(n+1, N), trace(n+1, -1);
  queue<int> q;

  dist[x] = 0;
  q.push(x);

  while(!q.empty()) {
    int u = q.front();
    q.pop();
    for(int v: g[u]) if(v != rem && (!adj[rem][v] || v == y) && dist[u] + 1 < dist[v]) {
      trace[v] = u;
      dist[v] = dist[u] + 1;
      q.push(v);
    }
  }

  if(dist[y] == N) return;

  vector<int> res;
  for(int v = y; v != -1; v = trace[v]) res.push_back(v);
  for(int v: res) cout << v << ' ';
  cout << rem << endl;
  exit(0);
}

int main() {
  cin >> n >> m;
  for(int i = 0, u, v; i < m; i++) {
    cin >> u >> v;
    edges.emplace_back(u, v);
    g[u].push_back(v);
    g[v].push_back(u);
    adj[u][v] = adj[v][u] = true;
  }

  for(int v = 1; v <= n; v++) {
    DSU dsu1 = DSU(n);
    for(int i = 1; i <= n; i++) used[i] = blocked[i] = false;
    used[v] = true;
    for(int x: g[v]) blocked[x] = true;

    vector<int> ord;
    for(int i = 1; i <= n; i++) ord.push_back(i);
    stable_partition(ord.begin(), ord.end(), [] (int x) { return !blocked[x]; });
    for(int i: ord) if(!used[i]) dfs(i, dsu1);

    for(int x: g[v]) for(int y: g[v]) if(x < y && !adj[x][y] && dsu1.root(x) == dsu1.root(y)) {
        bfs(x, y, v);
    }
  }
  cout << "no" << endl;
}
