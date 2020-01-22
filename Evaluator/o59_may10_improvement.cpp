// Sort by weight and find the number of pairs that need to go through that particular edge
// We need the number of special vertices in the whole component - the number of special vertices connected to the edge in the subtree
// Can compute these two values separately in order to handle same edge weights
// Use regular DSU
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1, MAXV = 1e6+1;

struct DSU {
    int dsu[N], f[N];
    DSU(int n, bool special[]) {
        fill(dsu, dsu + n, -1);
        for(int i = 0; i < n; i++) {
            if(special[i]) f[i] = 1;
            else f[i] = 0;
        }
    }
    int root(int v) { return dsu[v] < 0 ? v : dsu[v] = root(dsu[v]); }
    void merge(int u, int v) {
        if((u = root(u)) == (v = root(v))) return;
        dsu[u] += dsu[v];
        f[u] += f[v];
        dsu[v] = u;
        f[v] = 0;
    }
    int get_size(int v) {
        return f[root(v)];
    }
};

struct edge {
    int u, v, w, id;
    edge(int u, int v, int w, int id): u(u), v(v), w(w), id(id) {}
};

class Improvement {

    int n, m, res_whole[N], res_bottom[N];
    bool special[N];
    vector<int> g[N];
    vector<edge> edges, lse[MAXV];
    DSU *dsu;

    int d[N];
    void init_dfs(int u, int p) {
        for(int v: g[u]) {
            if(v == p) continue;
            d[v] = d[u] + 1;
            init_dfs(v, u);
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m;
        assert(n != 1);
        for(int i = 0, v; i < m; i++) {
            cin >> v;
            special[v] = true;
        }
        for(int i = 0, u, v, w; i < n - 1; i++) {
            cin >> u >> v >> w;
            g[u].push_back(v);
            g[v].push_back(u);
            edges.emplace_back(u, v, w, i);
            lse[w].emplace_back(u, v, w, i);
        }

        dsu = new DSU(n, special);
        for(int w = 0; w < MAXV; w++) {
            for(edge e: lse[w]) {
                dsu->merge(e.u, e.v);
            }
            for(edge e: lse[w]) {
                assert(dsu->root(e.u) == dsu->root(e.v));
                res_whole[e.id] = dsu->get_size(e.u);
            }
        }

        init_dfs(0, -1);
        auto cmp = [&] (edge e1, edge e2) {
            if(e1.w != e2.w) return e1.w < e2.w;
            return min(d[e1.u], d[e1.v]) > min(d[e2.u], d[e2.v]);
        };
        sort(edges.begin(), edges.end(), cmp);
        dsu = new DSU(n, special);

        for(edge e: edges) {
//            cerr << e.u << ' ' << e.v << ' ' << d[e.u] << ' ' << d[e.v] << endl;
            if(d[e.u] > d[e.v]) swap(e.u, e.v);
            res_bottom[e.id] = dsu->get_size(e.v);
            dsu->merge(e.u, e.v);
        }

        long ans = -LINF;
        for(int i = 0; i < n - 1; i++) {
            int idx = edges[i].id;
//            cerr << idx << ' ' << edges[i].u << ' ' << edges[i].v << ' ' << res_whole[idx] << ' ' << res_bottom[idx] << endl;
            ans = max(1ll * res_bottom[idx] * (res_whole[idx] - res_bottom[idx]), ans);
        }
        vector<int> ids;
        for(int i = 0; i < n - 1; i++) {
            int idx = edges[i].id;
            if(1ll * res_bottom[idx] * (res_whole[idx] - res_bottom[idx]) != ans) continue;
            ids.push_back(idx);
        }

        sort(ids.begin(), ids.end());
        cout << ids.size() << ' ' << ans << endl;
        for(int i: ids) cout << i << ' ';
        cout << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Improvement *obj = new Improvement();
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
