/*
 * For each r, maintain max(l...r) - min(l...r) + l - r. This has to equal 0
 * Notice that any subsegment will satisfy the condition >= 0, not = 0. So, we only need ot keep track of how many minimums there are
 * We iterate r from left to right, using monotone stack to determine the ranges to add to the value
 * To keep track of the number of minimums, we use something like o61_may02_bread.
 * To keep track of the answer, we need to put in an extra answer tag.
 * When we move the right bound, we want to add one to the answer tag of left positions with mininum value of our maintained expression
 * This of the answer tag as the sum of local answers for each right bound less than the current one
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 18;

struct node {
    long val, lz_val, ans, times, min_count;
    node() {
        val = lz_val = ans = times = min_count = 0;
    }
};

struct SegT { // usual lazy propagation technique that I always use cannot be used here
    node t[2*N];
    void apply_val(int i, long x) {
        t[i].val += x;
        t[i].lz_val += x;
    }
    void apply_times(int i, long x) {
        t[i].times += x;
        t[i].ans += x * t[i].min_count;
    }
    void apply(int i) { // assume t[i] has already been propagated to, attempt propogation to children only
        assert(i < N);
        if(t[i].lz_val) {
            apply_val(i << 1, t[i].lz_val);
            apply_val(i << 1 | 1, t[i].lz_val);
            t[i].lz_val = 0;
        }
        if(t[i].times) { // assume children's val has already been propogated
            if(t[i << 1].val == t[i].val) apply_times(i << 1, t[i].times);
            if(t[i << 1 | 1].val == t[i].val) apply_times(i << 1 | 1, t[i].times);
            t[i].times = 0;
        }
    }
    void merge(int i) {
        assert(i < N);
        t[i].val = min(t[i << 1].val, t[i << 1 | 1].val);
        t[i].min_count = (t[i << 1].val == t[i].val ? t[i << 1].min_count : 0) + (t[i << 1 | 1].val == t[i].val ? t[i << 1 | 1].min_count : 0);
        t[i].ans = t[i << 1].ans + t[i << 1 | 1].ans;
    }
    void build(int i = 1, int l = 0, int r = N-1) {
        t[i].val = l, t[i].min_count = 1;
        if(l == r) return;
        int mid = l+r >> 1;
        build(i << 1, l, mid);
        build(i << 1 | 1, mid+1, r);
    }
    void update(int tl, int tr, long v, int i = 1, int l = 0, int r = N-1) { // only for values
        if(r < tl || l > tr) return;
        if(tl <= l && r <= tr) {
            apply_val(i, v);
            return;
        }
        apply(i);
        int mid = l+r >> 1;
        update(tl, tr, v, i << 1, l, mid);
        update(tl, tr, v, i << 1 | 1, mid+1, r);
        merge(i);
    }
    long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
        if(r < tl || l > tr) return 0;
        if(tl <= l && r <= tr) {
            return t[i].ans;
        }
        apply(i);
        int mid = l+r >> 1;
        return query(tl, tr, i << 1, l, mid) + query(tl, tr, i << 1 | 1, mid + 1, r);
    }
    SegT() { build(); }
};

class TaskE {
    int n, q, a[N], stk_mx[N], stk_mn[N], top_mx, top_mn;
    long res[N];
    vector<pair<int,int> > qq[N];
    SegT *st;
public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0; i < n; i++) {
            cin >> a[i];
        }
        cin >> q;
        for(int i = 0, l, r; i < q; i++) {
            cin >> l >> r;
            --l, --r;
            qq[r].emplace_back(l, i);
        }

        st = new SegT();
        stk_mx[0] = stk_mn[0] = -1;

        for(int r = 0; r < n; r++) {
            st->apply_val(1, -1); // increment r increments all values by -1
            while(top_mx && a[stk_mx[top_mx]] <= a[r]) {
                st->update(stk_mx[top_mx - 1] + 1, stk_mx[top_mx], a[r] - a[stk_mx[top_mx]]);
                --top_mx;
            }
            stk_mx[++top_mx] = r;
            while(top_mn && a[stk_mn[top_mn]] >= a[r]) {
                st->update(stk_mn[top_mn - 1] + 1, stk_mn[top_mn], a[stk_mn[top_mn]] - a[r]);
                --top_mn;
            }
            stk_mn[++top_mn] = r;

            st->apply_times(1, 1); // Surprisingly, updating times in prefix is wrong, but it is because we need to use the global minimum, not that of only one subsegment
            for(auto tt: qq[r]) {
                int l, i;
                tie(l, i) = tt;
                res[i] = st->query(l, r);
            }
        }

        for(int i = 0; i < q; i++) {
            cout << res[i] << '\n';
        }
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
