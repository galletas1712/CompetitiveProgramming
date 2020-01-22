#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 19;

struct LST {
    long t[2*N], lz[2*N];
    void apply(int i) {
        t[i] += lz[i];
        if(i < N) {
            lz[i << 1] += lz[i];
            lz[i << 1 | 1] += lz[i];
        }
        lz[i] = 0;
    }
    void update(int tl, int tr, long v, int i = 1, int l = 0, int r = N-1) {
        apply(i);
        if(r < tl || l > tr) return;
        if(tl <= l && r <= tr) {
            lz[i] += v;
            apply(i);
            return;
        }
        int mid = l+r >> 1;
        update(tl, tr, v, i << 1, l, mid);
        update(tl, tr, v , i << 1 | 1, mid+1, r);
        t[i] = min(t[i << 1], t[i << 1 | 1]);
    }
    long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
        apply(i);
        if(r < tl || l > tr) return LINF;
        if(tl <= l && r <= tr) return t[i];
        int mid = l+r >> 1;
        return min(query(tl, tr, i << 1, l, mid), query(tl, tr, i << 1 | 1, mid+1, r));
    }
};

class TaskF {

    int n, q;
    vector<pair<int,int> > g[N];
    vector<tuple<int,int,int> > ls[N];
    LST *segT;
    long res[N];

    int tick, st[N], en[N];
    long d[N];
    void dfs1(int u, long curr_dist = 0) {
        st[u] = ++tick;
        for(auto p: g[u]) {
            int v, w;
            tie(v, w) = p;
            d[v] = d[u] + w;
            dfs1(v, curr_dist + w);
        }
        en[u] = tick;
        if(st[u] == en[u]) {
            segT->update(st[u], st[u], curr_dist);
        } else {
            segT->update(st[u], st[u], LINF);
        }
    }

    void dfs2(int u) {
        for(auto p: g[u]) {
            int v, w;
            tie(v, w) = p;
            segT->update(st[v], en[v], -2*w);
            dfs2(v);
            segT->update(st[v], en[v], 2*w);
        }
//        cerr << u << ' ' << endl;
//        for(int i = 1; i <= n; i++) cerr << segT->query(i, i) << ' ';
//        cerr << endl;
        for(auto tt: ls[u]) {
            int l, r, i;
            tie(l, r, i) = tt;
            res[i] = segT->query(l, r) + d[u];
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> q;
        for(int i = 2, p, w; i <= n; i++) {
            cin >> p >> w;
            g[p].emplace_back(i, w);
        }
        for(int i = 0, v, l, r; i < q; i++) {
            cin >> v >> l >> r;
            ls[v].emplace_back(l, r, i);
        }
        segT = new LST();
        dfs1(1);
        dfs2(1);
        for(int i = 0; i < q; i++) cout << res[i] << '\n';
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
