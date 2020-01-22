/*
 * fw[u] = maximum number of twin pairs in subtree of u, where u is white
 * dp[u][k] = maximum number of twin pairs in subtree of, where u is black and k white vertices are still reachable from the top
 * Normal O(n^3) sol will TLE
 * Need to only consider states that are visited in order to AC
 * Actually official solution is O(n^2)
 * Used a bit of sack optimization just for fun, but doesn't really make any difference
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 5001, INF = 1e9+1;

void setmax(int &a, int b) {
    a = max(a, b);
    if(a < 0) a = -INF;
}

int n;
vector<int> g[N];

int sz[N];
void dfs1(int u, int p) {
    sz[u] = 1;
    for(int &v: g[u]) {
        if(v == p) continue;
        dfs1(v, u);
        sz[u] += sz[v];
        if(g[u][0] == p || sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
    }
}

int dp[N][N], fw[N]; // dp assumes black
void dfs2(int u, int p) {
    if(g[u][0] == p) {
        dp[u][0] = dp[u][1] = fw[u] = 0;
        return;
    }

    int curr_sz = 0;

    dfs2(g[u][0], u);
    setmax(dp[u][1], fw[g[u][0]]);
    for(int k = 0; k <= sz[g[u][0]]; k++)
        setmax(dp[u][k], dp[g[u][0]][k]);
    curr_sz += sz[g[u][0]];

    for(int v: g[u]) {
        if(v == p || v == g[u][0]) continue;
        dfs2(v, u);
        vector<int> org, nw;
        for(int k = 0; k <= curr_sz; k++) {
            if(dp[u][k] >= 0) {
                org.push_back(k);
            }
        }
        for(int k = 0; k < sz[v]; k++) {
            if(dp[v][k] >= 0) {
                nw.push_back(k);
            }
        }

        curr_sz += sz[v];
        vector<int> tmp(curr_sz + 1, -INF);

        for(int k1: org) {
            for(int k2: nw) {
                setmax(tmp[k1 + k2], dp[v][k2] + k2 * k1 + dp[u][k1]);
            }
            setmax(tmp[k1 + 1], fw[v] + k1 + dp[u][k1]);
        }
        for(int i = 0; i < tmp.size(); i++) {
            setmax(dp[u][i], tmp[i]);
        }
    }

    for(int v: g[u]) { // handle fw
        if(v == p) continue;
        int tmp = fw[v] + 1;
        for(int k = 0; k < sz[v]; k++) {
            setmax(tmp, dp[v][k] + k);
        }
        fw[u] += tmp;
    }
    setmax(dp[u][1], fw[u]);
}

int solve(istream& cin) {
    cin >> n;

    for(int i = 0; i <= n; i++) {
        g[i].clear();
        for(int j = 0; j <= n; j++) dp[i][j] = -INF;
    }
    fill(fw, fw+n+1, 0);

    for(int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1, -1);
    dfs2(1, -1);
    int ans = max(*max_element(dp[1], dp[1] + n + 1), fw[1]);
    return ans;
}

void solve(istream& cin, ostream& cout) {
    int t;
    cin >> t;
    while(t--) {
        int res = solve(cin);
        cout << res << endl;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    ifstream in("tricolor.in");
    ofstream out("tricolor.out");
    solve(in, out);
}
