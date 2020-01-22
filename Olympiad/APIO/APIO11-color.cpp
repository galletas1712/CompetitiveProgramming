/*
Look at the image from http://momo-funnycodes.blogspot.com/2013/05/apio-2011-table-coloring.html
We can see that if we fix the upper left hand corner, we can reduce the problem to boolean satisfiability with xors
This can be done using a technique similar to when checking if a graph is bipartite
*/
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 2e5+1;

int n, m, k, chk[N]; // chk contains prefix xor aggregate on path. Chechking done like finding odd cycles in graph
bool can[2] = {1, 1}, mark[N];
vector<pair<int,int > > g[N];

bool dfs(int u, int x) { // x = a[1][1]
    mark[u] = true;
    bool ret = true;
    for(auto p: g[u]) {
        int v, w;
        tie(v, w) = p;
        if(mark[v]) {
            if(chk[u] ^ chk[v] != w ^ x) return false; // look at the table and you will see why we need an ^x at the end
            continue;
        }
        chk[v] = chk[u] ^ w ^ x;
        ret &= dfs(v, x);
    }
    return ret;
}

void add_edge(int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
}

long solve(int c) {
    fill(mark, mark+n+m+1, 0);
    fill(chk, chk+n+m+1, 0);
    int tot = 0;
    for(int i = 0; i <= n+m; i++) {
        if(i == 1 || i == n+1 || mark[i]) continue;
        if(!dfs(i, (!i && c))) { // only need c in component with root (i == 0 corresponds to a[1][1])
            return 0;
        }
        tot += i != 0;
    }
    long ret = 1;
    while(tot--) ret *= 2, ret %= int(1e9);
    return ret;
}

void solve(istream& cin) {
    cin >> n >> m >> k;
    for(int i = 0, x, y, c; i < k; i++) {
        cin >> x >> y >> c;
        if(x == 1 && y == 1) can[c ^ 1] = false;
        else if(x == 1) {
            add_edge(0, y+n, c);
        }
        else if(y == 1) {
            add_edge(0, x, c);
        }
        else add_edge(x, y+n, c ^ (x % 2 == 0 && y % 2 == 0));
    }
    long ans = 0;
    if(can[0]) ans += solve(0);
    if(can[1]) ans += solve(1);
    cout << ans % int(1e9) << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve(cin);
}
