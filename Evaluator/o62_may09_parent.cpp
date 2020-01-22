#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1;

struct info {
   int op, u, v;
   info(int op, int u, int v): op(op), u(u), v(v) {}
};

struct DSU {
    int f[N], master[N];
    DSU() {
        fill(f, f + N, -1);
        for(int i = 0; i < N; i++) master[i] = i;
    }
    int root(int v) {
        return (f[v] < 0 ? v : f[v] = root(f[v]));
    }
    int find_master(int v) {
        return master[root(v)];
    }
    void change_master(int v) {
        master[root(v)] = v;
    }
    void merge(int u, int v) {
        swap(u, v);
        if((u = root(u)) == (v = root(v))) {
            assert(false);
            return;
        }
        f[u] += f[v];
        f[v] = u;
    }
} dsu;

int n, m;
vector<info> ops;
vector<int> g[N];

int d[N];
vector<int> anc[N];
bool mark[N];
void init_dfs(int u, int p) {
    mark[u] = true;
    for(int v: g[u]) {
        if(v == p) continue;
        d[v] = d[u] + 1;
        anc[v].push_back(u);
        for(int j = 0; 1 << j+1 <= d[v]; j++) {
            anc[v].push_back(anc[anc[v][j]][j]);
        }
        init_dfs(v, u);
    }
}

int get_lca(int u, int v) {
    if(d[u] < d[v]) swap(u, v);
    int diff = d[u] - d[v];
    for(int j = 0; j < 19; j++) {
        if(diff >> j & 1) u = anc[u][j];
    }
    if(u == v) return u;
    for(int j = anc[u].size() - 1; j >= 0; j--) {
        if(j < anc[u].size() && anc[u][j] != anc[v][j]) {
            u = anc[u][j];
            v = anc[v][j];
        }
    }
    return anc[u][0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        info tmp(-1, -1, -1);
        cin >> tmp.op >> tmp.u;
        if(tmp.op != 2) cin >> tmp.v;
        ops.push_back(tmp);
    }
    for(int i = 0; i < m; i++) {
        if(ops[i].op == 1) {
            g[ops[i].u].push_back(ops[i].v);
            g[ops[i].v].push_back(ops[i].u);
        }
    }
    for(int i = 1; i <= n; i++) {
        if(!mark[i]) init_dfs(i, -1);
    }
    init_dfs(1, -1);
    for(int i = 0; i < m; i++) {
        if(ops[i].op == 1) {
            dsu.merge(ops[i].u, ops[i].v); // order is very important here
        } else if(ops[i].op == 2) {
            dsu.change_master(ops[i].u);
        } else {
            int master = dsu.find_master(ops[i].u);
            assert(dsu.find_master(ops[i].v) == master);
            int lca_u = get_lca(ops[i].u, master);
            int lca_v = get_lca(ops[i].v, master);
            bool isU = d[ops[i].u] + d[master] - 2*d[lca_u] < d[ops[i].v] + d[master] - 2*d[lca_v];
            cout << (isU ? ops[i].u : ops[i].v) << '\n';
        }
    }
}
