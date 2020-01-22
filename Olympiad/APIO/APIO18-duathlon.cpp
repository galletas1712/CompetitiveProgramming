/*
 * Count complements
 * Consider some cut vertex u
 * We will only count invalid triples that cross u's BCCs
 * All three can't be in different BCCs, otherwise it's always valid
 * Therefore, two must be in one BCC and one must be in the other.
 * With this, there are two invalid orderings for each triple that satisfies the above constraints
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1;

class Duathlon {

    int n, m;
    vector<int> g[N];
    long ans;

    int tick, ccs, tot, pre[N], low[N], d[N];
    vector<int> stk;
    vector<int> bcg[N];
    void dfs1(int u, int par) {
        pre[u] = low[u] = ++tick;
        stk.push_back(u);
        ++tot;
        for(int v: g[u]) {
            if(v == par) continue;
            if(!pre[v]) {
                dfs1(v, u);
                low[u] = min(low[v], low[u]);
                if(low[v] >= pre[u]) {
                    ++ccs;
                    bcg[u].push_back(n + ccs);
                    vector<int> curr;
                    while(curr.empty() || curr.back() != v) {
                        curr.push_back(stk.back());
                        stk.pop_back();
                    }
                    bcg[n + ccs] = curr;
                }
            } else {
                low[u] = min(pre[v], low[u]);
            }
        }
    }

    long sz[N];
    void dfs2(int u, int par) {
        sz[u] = u <= n;
        for(int v: bcg[u]) {
            dfs2(v, u);
            sz[u] += sz[v];
            if(u > n) {
                ans -= 1ll * bcg[u].size() * sz[v] * (sz[v] - 1);
            }
        }
        if(u > n) {
            ans -= 1ll * bcg[u].size() * (tot - sz[u]) * (tot - sz[u] - 1);
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m;
        for(int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        for(int i = 1; i <= n; i++) {
            if(pre[i]) continue;
            dfs1(i, 0);
            dfs2(i, 0);
            ans += 1ll * tot * (tot - 1) * (tot - 2);
            tot = 0;
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Duathlon *obj = new Duathlon();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::istream& in(std::cin);
    std::ostream& out(std::cout);
    solver.solve(in, out);
    return 0;
}
