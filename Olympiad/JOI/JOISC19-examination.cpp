// Sort by S+T and use a merge sort to maintain 2D sums of S and T
// However, notice that we are only querying for sums in prefix (just negate indices), so we can use fenwick in place of each segment tree in each dimension
// Also notice that for other problems, only either querying sums or querying in prefixes need to hold for us to be able to do this
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

struct fenwick {
    vector<int> f;
    fenwick(int n) {
        f.resize(n + 10);
    }
    void update(int i) {
        assert(i >= 0);
        ++i;
        while(i < f.size()) ++f[i], i += i & -i;
    }
    int query(int i) {
        ++i;
        int ret = 0;
        while(i > 0) ret += f[i], i -= i & -i;
        return ret;
    }
};

struct info {
    int sum, x, y, tp, id;
    info(int sum, int x, int y, int tp, int id): sum(sum), x(x), y(y), tp(tp), id(id) {}
    bool operator<(const info& rhs) const {
        return make_tuple(sum, x, y, tp, id) < make_tuple(rhs.sum, rhs.x, rhs.y, rhs.tp, rhs.id);
    }
};

class Examination {

    int n, q, res[N];
    vector<int> ls[N], xs;
    vector<info> a;
    fenwick *f[N];

    void compress() {
        sort(xs.begin(), xs.end());
        xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
        for(int i = 0; i < n; i++) {
            a[i].x = lower_bound(xs.begin(), xs.end(), a[i].x) - xs.begin();
        }
    }

    void ins(int i, int v) {
        ++i;
        while(i <= xs.size()) ls[i].push_back(v), i += i & -i;
    }

    void build() {
        for(int i = 1; i <= xs.size(); i++) {
            sort(ls[i].begin(), ls[i].end());
            ls[i].resize(unique(ls[i].begin(), ls[i].end()) - ls[i].begin());
            f[i] = new fenwick(ls[i].size());
        }
    }

    void update(int i, int v) {
        assert(i >= 0);
        ++i;
        while(i <= xs.size()) {
            auto it = lower_bound(ls[i].begin(), ls[i].end(), v);
            assert(*it == v);
            f[i]->update(it - ls[i].begin());
            i += i & -i;
        }
    }

    int query(int i, int v) {
        ++i;
        int ret = 0;
        while(i > 0) {
            auto it = upper_bound(ls[i].begin(), ls[i].end(), v);
            ret += f[i]->query(it - ls[i].begin() - 1);
            i -= i & -i;
        }
        return ret;
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> q;
        for(int i = 0, x, y; i < n; i++) {
            cin >> x >> y;
            x *= -1, y *= -1;
            xs.push_back(x);
            a.emplace_back(x + y, x, y, 0, i);
        }
        compress();
        for(info tt: a) {
            if(tt.tp == 0) ins(tt.x, tt.y);
        }
        build();

        for(int i = 0, x, y, z; i < q; i++) {
            cin >> x >> y >> z;
            x *= -1, y *= -1, z *= -1;
            auto itx = upper_bound(xs.begin(), xs.end(), x);
            int posx = itx - xs.begin() - 1;
            a.emplace_back(z, posx, y, 1, i);
        }
        sort(a.begin(), a.end());
        for(info tt: a) {
            if(tt.tp == 0) update(tt.x ,tt.y);
            else res[tt.id] = query(tt.x, tt.y);
        }
        for(int i = 0; i < q; i++) {
            cout << res[i] << '\n';
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Examination *obj = new Examination();
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
