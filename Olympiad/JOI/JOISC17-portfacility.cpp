// Problem is to find the number of ways to color the edges such that those in the same set don't partially overlap (fully containing is fine)
// Build a graph of segments that cannot be with each other. Equivalent to finding 2-coloring of this graph
// If graph is not bipartite, we return 0 (check using undirected SCC = DSU)
// Sort segments by left bound. If the answer is valid, then the segments that partially overlap this one should form a mountain shape.
// We merge all of them is DSU and add an optimization to never merge them again. This gives O(n log n) amortized but with a pretty big constant.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e6+1;

struct seg {
    int l, r, i;
    bool operator<(const seg &rhs) const {
        return r < rhs.r;
    }
    seg(int l, int r, int i): l(l), r(r), i(i) {}
};

class PortFacility {

    int n, dsu[N];
    vector<seg> a;
    set<seg> st;

    int root(int v) {
        return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v]));
    }

    void merge(int u, int v) {
        if((u = root(u)) == (v = root(v))) return;
        dsu[u] += dsu[v];
        dsu[v] = u;
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0, l, r; i < n; i++) {
            cin >> l >> r;
            a.emplace_back(l, r, i);
        }
        sort(a.begin(), a.end(), [] (seg x, seg y) { return x.l < y.l; });

        fill(dsu, dsu+2*n+1, -1);
        for(seg s: a) {
            while(!st.empty() && st.begin()->r < s.l) st.erase(st.begin());
            auto lim = st.upper_bound(seg(-1, s.r, -1));
//            cerr << "main" << s.l << ' ' << s.r << ' ' << s.i << endl;
            for(auto it = st.begin(); it != lim; ++it) {
//                cerr << it->l << ' ' << it->r << ' ' << it->i << endl;
                merge(s.i, it->i + n);
                merge(s.i + n, it->i);
                if(root(it->i) == root(prev(lim)->i)) break;
            }
            st.insert(s);
        }
        int cnt = 0;
        for(int i = 0; i < n; i++) {
            if(root(i) == root(i + n)) {
                cout << 0 << endl;
                return;
            }
            cnt += dsu[i] < 0;
        }
        long ans = 1;
        while(cnt--) ans = (ans <<= 1) % MOD;
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        PortFacility *obj = new PortFacility();
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
