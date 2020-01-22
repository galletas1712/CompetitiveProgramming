// Small to large DSU on tree with many dynamic LiChao trees
// Alternatively, use sack with only one fixed LiChao tree
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+10;

struct Line {
    long m, c;
    Line(long m, long c): m(m), c(c) {}
    long get(long x) { return m*x + c; }
};

struct LCT {

    struct Node {
        Line v;
        Node *l, *r;
        Node(Line v): v(v), l(NULL), r(NULL) {}
    };

    typedef Node* pnode;
    pnode root;

    void swap(LCT& r) {
        std::swap(root, r.root);
    }

    void update(pnode &t, int l, int r, Line v) {
        if(!t) {
            t = new Node(v);
            return;
        }
        if(v.get(l) > t->v.get(l) && v.get(r) > t->v.get(r)) return;
        if(l == r || (v.get(l) < t->v.get(l) && v.get(r) < t->v.get(r))) {
            t->v = v;
            return;
        }
        int mid = l+r >> 1;
        if(v.get(mid) < t->v.get(mid)) std::swap(v, t->v);
        if(v.get(l) < t->v.get(l)) update(t->l, l, mid, v);
        else update(t->r, mid+1, r, v);
    }

    long query(pnode t, int l, int r, long x) {
        if(!t) return LINF;
        long ret = t->v.get(x);
        int mid = l+r >> 1;
        if(x <= mid) ret = min(query(t->l, l, mid, x), ret);
        else ret = min(query(t->r, mid+1, r, x), ret);
        return ret;
    }
};

class TaskF {

    int n, a[N], b[N];
    vector<Line> ls[N];
    LCT f[N];
    long dp[N];
    vector<int> g[N];

    void dfs(int u, int p) {
        for(int v: g[u]) if(v != p) {
            dfs(v, u);
            if(ls[v].size() > ls[u].size()) {
                ls[u].swap(ls[v]);
                f[u].swap(f[v]);
            }
        }
        for(int v: g[u]) if(v != p) {
            for(Line l: ls[v]) {
                ls[u].push_back(l);
                f[u].update(f[u].root, -N, N, l);
            }
        }
        if(g[u].size() != 1 || u == 1) {
            dp[u] = f[u].query(f[u].root, -N, N, a[u]);
        }
        ls[u].emplace_back(b[u], dp[u]);
        f[u].update(f[u].root, -N, N, Line(b[u], dp[u]));
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1; i <= n; i++) cin >> a[i];
        for(int i = 1; i <= n; i++) cin >> b[i];
        for(int i = 1, u, v; i < n; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfs(1, 0);
        for(int i = 1; i <= n; i++) cout << dp[i] << ' ';
        cout << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        TaskF *obj = new TaskF();
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
