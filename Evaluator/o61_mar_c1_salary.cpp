#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1;

int n, a[N];
vector<int> g[N];
multiset<int> f[N];
void dfs(int u) {
    for(int v: g[u]) {
        dfs(v);
        if(f[v].size() > f[u].size()) swap(f[u], f[v]);
        for(int x: f[v]) f[u].insert(x);
        f[v].clear();
    }
    auto it = f[u].upper_bound(a[u]);
    if(it == f[u].end()) f[u].insert(a[u]);
    else {
        f[u].erase(it);
        f[u].insert(a[u]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for(int i = 0, x; i < n; i++) cin >> a[i];
    for(int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v);
    }
    dfs(0);
    cout << n - f[0].size() << endl;
}
