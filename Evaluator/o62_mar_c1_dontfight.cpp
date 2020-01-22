// Observe that no matter where the first subtree we choose lies, we want the closest two subtrees after we removed it.
// Split into three cases to prove: A < B < C, B < A < C, B < C < A
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 3e5+1;

class DontFight {

    int n;
    long ans = LINF, a[N];
    vector<int> g[N];

    void set_ans(long A, long B, long C) {
        if(!A || !B || !C) return; // important!
        long diff = max(A, max(B, C)) - min(A, min(B, C));
        ans = min(diff, ans);
    }

    int tick, st[N], en[N], rev[N], sz[N];
    long tot, sum[N];
    set<int> f;

    void dfs1(int u, int p) {
        st[u] = ++tick;
        rev[st[u]] = u;
        if(p != -1) g[u].erase(find(g[u].begin(), g[u].end(), p));

        sz[u] = 1;
        sum[u] = a[u];
        for(int &v: g[u]) {
            dfs1(v, u);
            sz[u] += sz[v];
            sum[u] += sum[v];
            if(sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
        }

        en[u] = tick;
    }

    void dfs2(int u, bool keep) {
        for(int v: g[u]) {
            if(v == g[u][0]) continue;
            dfs2(v, 0);
        }
        if(!g[u].empty()) dfs2(g[u][0], 1);
        for(int rt: g[u]) {
            if(rt == g[u][0]) continue;
            for(int i = st[rt]; i <= en[rt]; i++) {
                int v = rev[i];
                auto it = f.upper_bound((tot - sum[v]) / 2);
                if(it != f.end()) {
                    set_ans(tot - sum[v] - *it, *it, sum[v]);
                }
                if(it != f.begin()) {
                    --it;
                    set_ans(tot - sum[v] - *it, *it, sum[v]);
                }
            }
            for(int i = st[rt]; i <= en[rt]; i++) {
                f.insert(sum[rev[i]]);
            }
        }
        f.insert(sum[u]);
        auto it = f.upper_bound(sum[u] / 2);
        if(it != f.end()) {
            set_ans(tot - sum[u], *it, sum[u] - *it);
        }
        if(it != f.begin()) {
            --it;
            set_ans(tot - sum[u], *it, sum[u] - *it);
        }
//        cerr << "vertex " << u << endl;
//        for(int x: f) cerr << x << ' ';
//        cerr << endl;
        if(!keep) f.clear();
    }


public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1; i <= n; i++) cin >> a[i], tot += a[i];
        for(int i = 1, u, v; i < n; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfs1(1, -1);
        dfs2(1, -1);
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        DontFight *obj = new DontFight();
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
