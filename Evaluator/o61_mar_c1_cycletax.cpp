//https://www.cs.princeton.edu/courses/archive/fall03/cs528/handouts/two_stremlined.pdf
#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9+1;
const int N = 1010;

int n, m, S, T;
vector<int> g[N];
vector<pair<int,int> > edges;

int tick, pre[N], low[N];
void dfs1(int u, int p) {
    pre[u] = ++tick;
    low[u] = u;
    for(int v: g[u]) {
        if(v == p) continue;
        if(pre[v]) {
            if(pre[v] < pre[low[u]]) low[u] = v;
            continue;
        }
        dfs1(v, u);
        if(pre[low[v]] < pre[low[u]]) low[u] = low[v];
    }
//    cerr << u << ' ' << low[u] << endl;
}

bool mark[N];
list<int>::iterator it[N];
list<int> L;
void dfs2(int u) {
    mark[u] = true;
    for(int v: g[u]) {
        if(mark[v]) continue;
        if(v == T) {
            L.push_back(T);
            it[T] = prev(L.end());
        } else {
            if(*it[low[v]] < 0) {
                it[v] = L.insert(it[u], v);
                *it[u] = u;
            } else {
                it[v] = L.insert(next(it[u]), v);
                *it[u] = -u;
            }
        }
//        cerr << "curr " << u << ' ' << v << endl;
//        for(int x: L) cerr << x << ' ';
//        cerr << endl;
        dfs2(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    int mn = INF;
    for(int i = 0, u, v, c; i < m; i++) {
        cin >> u >> v >> c;
        if(c < mn) {
            S = u;
            T = v;
            mn = c;
        }
        g[u].push_back(v);
        g[v].push_back(u);
        edges.emplace_back(u, v);
    }
    for(int &v: g[S]) if(v == T) swap(v, g[S][0]);
    L.push_back(-S);
    it[S] = prev(L.end());
    dfs1(S, 0);
    dfs2(S);

    vector<int> ord;
    copy(L.begin(), L.end(), back_inserter(ord));
    vector<int> rev(n+1);
    for(int i = 0; i < ord.size(); i++) {
        rev[abs(ord[i])] = i;
    }

    cout << mn << ' ' << 1 << '\n';
    for(auto e: edges) {
        int u, v;
        tie(u, v) = e;
        if(rev[v] < rev[u]) swap(u, v);
        if(u == S && v == T) cout << v << ' ' << u << ' ' << 1 << '\n';
        else cout << u << ' ' << v << ' ' << 0 << '\n';
    }
}
