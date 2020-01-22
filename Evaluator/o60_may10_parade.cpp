#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class Parade {

    long modPow(long b, long e) {
        if(e == 0) return 1;
        if(e == 1) return b;
        return modPow(b*b % MOD, e >> 1) * modPow(b, e & 1) % MOD;
    }

    int n;
    vector<int> g[N];
    long fact[N], invf[N];

    int sz[N];
    long ans, dp[N];

    void dfs1(int u, int p) {
        sz[u] = 1;
        if(p >= 0) g[u].erase(find(g[u].begin(), g[u].end(), p));
        for(int v: g[u]) {
            dfs1(v, u);
            sz[u] += sz[v];
        }
        dp[u] = fact[sz[u] - 1];
        for(int v: g[u]) {
            dp[u] *= dp[v] * invf[sz[v]] % MOD;
            dp[u] %= MOD;
        }
//        cerr << u << ' ' << dp[u] << endl;
    }

    void dfs2(int u) {
//        cerr << "answer " << u << ' ' << dp[u] << endl;
        ans = (ans + dp[u]) % MOD;
        for(int v: g[u]) {
            long org_dp_u = dp[u], org_dp_v = dp[v], org_sz_u = sz[u], org_sz_v = sz[v];
            dp[u] = dp[u] * fact[sz[v]] % MOD * modPow(dp[v], MOD - 2) % MOD;
            dp[u] = dp[u] * fact[sz[u] - sz[v] - 1] % MOD * invf[sz[u] - 1] % MOD;
            sz[u] -= sz[v];
            dp[v] = dp[v] * dp[u] % MOD * invf[sz[u]] % MOD;
            dp[v] = dp[v] * fact[sz[v] + sz[u] - 1] % MOD * invf[sz[v] - 1] % MOD;
            sz[v] += sz[u];
//            cerr << dp[u] << ' ' << sz[u] << ' ' << dp[v] << ' ' << sz[v] << endl;
            dfs2(v);
            dp[u] = org_dp_u;
            dp[v] = org_dp_v;
            sz[u] = org_sz_u;
            sz[v] = org_sz_v;
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1, u, v; i < n; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        fact[0] = invf[0] = 1;
        for(int i = 1; i <= n; i++) {
            fact[i] = fact[i-1] * i % MOD;
            invf[i] = modPow(fact[i], MOD - 2);
        }
        dfs1(0, -1);
        dfs2(0);
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Parade *obj = new Parade();
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
