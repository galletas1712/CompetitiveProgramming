// Compute SA and LCP and find the biggest range where the LCP is equal to the length of the string
// Separate each query into finding the number of active elements within a range with time <= r and <= l-1
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 4e5+10, M = 5e5+10;

struct node {
    int bound, type, l, r, id;
    node(int bound, int type, int l, int r, int id): bound(bound), type(type), l(l), r(r), id(id) {}
    bool operator<(const node &rhs) const {
        return bound < rhs.bound || (bound == rhs.bound && (type == 0) > (rhs.type == 0)); // check comparator
    }
};

struct fenwick {
    int f[N];
    void update(int i) {
        ++i;
        while(i < N) ++f[i], i += i & -i;
    }
    int query(int i) {
        ++i;
        int ret = 0;
        while(i > 0) {
            ret += f[i];
            i -= i & -i;
        }
        return ret;
    }
};

class TaskE {

    int n, q, len, suff[N], res[M];
    string s[N];
    string concat;

    int sa[N], pos[N], tmp[N], lcp[N];
    void buildSA() {
        int gap;
        auto cmp = [&] (int i, int j) {
            if(pos[i] != pos[j]) return pos[i] < pos[j];
            i += gap;
            j += gap;
            return i < len && j < len ? pos[i] < pos[j] : i > j;
        };
        for(int i = 0; i < len; i++) {
            sa[i] = i;
            pos[i] = concat[i];
        }
        for(gap = 1; ; gap <<= 1) {
            sort(sa, sa + len, cmp);
            for(int i = 1; i < len; i++) tmp[i] = tmp[i-1] + cmp(sa[i-1], sa[i]);
            for(int i = 0; i < len; i++) pos[sa[i]] = tmp[i];
            if(tmp[len - 1] == len - 1) break;
        }
        for(int i = 0, k = 0; i < len; i++, k = (k ? k - 1 : 0)) {
            if(pos[i] == len - 1) {
                k = 0;
                continue;
            }
            int j = sa[pos[i] + 1];
            while(max(i + k, j + k) < len && concat[i + k] == concat[j + k]) ++k;
            lcp[pos[i]] = k;
        }
//        for(int i = 0; i < len; i++) cerr << i << ' ' << lcp[i] << ' ' << concat.substr(sa[i]) << endl;
    }

    int t[2*N];
    void build() {
        for(int i = 0; i < len; i++) t[len + i] = lcp[i];
        for(int i = len - 1; i >= 1; i--) t[i] = min(t[i << 1], t[i << 1 | 1]);
    }

    int query(int l, int r) {
        int ret = INF;
        for(l += len, r += len + 1; l < r; l >>= 1, r >>= 1) {
            if(l & 1) ret = min(t[l++], ret);
            if(r & 1) ret = min(t[--r], ret);
        }
        return ret;
    }

    int lb[N], rb[N];
    vector<node> ops;
    fenwick *fen;

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> q;
        for(int i = 0; i < n; i++) {
            cin >> s[i];
            concat.push_back('?');
            suff[i] = concat.length();
            for(char c: s[i]) concat.push_back(c);
        }
        len = concat.length();
        buildSA();
        build();
        for(int i = 0; i < n; i++) {
            for(int j = suff[i]; j < suff[i] + s[i].length(); j++) {
                ops.emplace_back(i, 0, pos[j], pos[j], -1);
//                cerr << "insert " << i << ' ' << j << ' ' << pos[j] << endl;
            }
        }
        for(int i = 0; i < n; i++) {
            int l = 0, r = pos[suff[i]] - 1;
            while(l < r) {
                int mid = l+r >> 1;
                if(query(mid, pos[suff[i]] - 1) >= s[i].length()) r = mid;
                else l = mid + 1;
            }
            if(query(l, pos[suff[i]] - 1) >= s[i].length()) lb[i] = l;
            else lb[i] = pos[suff[i]];

            l = pos[suff[i]], r = len - 1;
            while(l < r) {
                int mid = l+r+1 >> 1;
                if(query(pos[suff[i]], mid) >= s[i].length()) l = mid;
                else r = mid - 1;
            }
            if(query(pos[suff[i]], l) >= s[i].length()) rb[i] = l + 1;
            else rb[i] = pos[suff[i]];
//            cerr << "precompute " << i << ' ' << lb[i] << ' ' << rb[i] << endl;
        }

        for(int i = 0, l, r, k; i < q; i++) {
            cin >> l >> r >> k;
            --l, --r, --k;
            ops.emplace_back(r, 1, lb[k], rb[k], i);
            if(l) ops.emplace_back(l - 1, -1, lb[k], rb[k], i);
        }
        sort(ops.begin(), ops.end());
        fen = new fenwick();
        for(node tt: ops) {
//            cerr << "nodes " << tt.bound << ' ' << tt.type << ' ' << tt.l << ' ' << tt.r << ' ' << tt.id << endl;
            if(tt.type == 0) {
                assert(tt.l == tt.r);
                fen->update(tt.l);
            } else {
                res[tt.id] += tt.type * (fen->query(tt.r) - fen->query(tt.l - 1));
            }
        }
        for(int i = 0; i < q; i++) cout << res[i] << '\n';
    }
};


class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        TaskE *obj = new TaskE();
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
