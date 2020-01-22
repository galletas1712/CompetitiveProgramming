/*
 * Split into cases:
 * a[i] < a[l] and a[i] < a[r] -> diff = 0
 * a[i] < a[l] and a[i] == a[r] -> diff = -1
 * a[i] < a[l] and a[i] > a[r] -> diff = -2
 * a[i] > a[l] and a[i] < a[r] -> diff = 2
 * a[i] > a[l] and a[i] == a[r] -> diff = 1
 * a[i] > a[l] and a[i] > a[r] -> diff = 0
 * a[i] == a[l] and a[i] < a[r] -> diff = 1
 * a[i] == a[l] and a[i] == a[r] -> diff = 0
 * a[i] == a[l] and a[i] > a[r] -> diff = -1
 * From this, we notice that the only possible cases that are the least bit optimal are when a[r] < a[l]
 * So, we reduce the cases down to three:
 * a[r] < a[i] < a[l] -> diff = -2
 * a[i] == a[l] and a[i] > a[r] -> diff = -1
 * a[i] == a[r] and a[i] < a[l] -> diff = -1
 *
 * Visualize this problem on a 2-d plane with indices on the x-axis and their values on the y-axis
 * Notice that the possible left indices form an increasing sequence.
 * This is because i, j such that i < j and a[i] > a[j], then i is always more optimal as a left index than j, no matter which right index is chosen
 * A symmetrical thing applies to the right indices (i > j, a[i] < a[j])
 * Consider any index that is not a candidate left/right index
 * It will contribute to a contiguous subsequence of left indices for a fixed right index, therefore we update range using lazy segment tree
 * We sweep the right index from left to right (using only the candidate ones, so it is always increasing)
 * Since it is always increasing, we only need to consider three operations:
 * Delete an index's contribution, where a[idx] < a[lsr[i]]
 * Adjust an index's contribution, where a[idx] == a[lsr[i]]
 * Add in a new index with the correct contribution (if a[idx] == a[lsr[i]] then use 1, otherwise use 2)
 * See code for how to do this (comments are written in detail)
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
#define pii pair<int,int>
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 17;

struct LazySegTree {
    long t[2*N], lz[2*N];
    void apply(int i) {
        t[i] += lz[i];
        if(i < N) {
            lz[i << 1] += lz[i];
            lz[i << 1 | 1] += lz[i];
        }
        lz[i] = 0;
    }
    void update(int tl, int tr, int v, int i = 1, int l = 0, int r = N-1) {
        apply(i);
        if(r < tl || l > tr) return;
        if(tl <= l && r <= tr) {
            lz[i] += v;
            apply(i);
            return;
        }
        int mid = l+r >> 1;
        update(tl, tr, v, i << 1, l, mid);
        update(tl, tr, v, i << 1 | 1, mid+1, r);
        t[i] = max(t[i << 1], t[i << 1 | 1]);
    }
    long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
        apply(i);
        if(r < tl || l > tr) return 0;
        if(tl <= l && r <= tr) return t[i];
        int mid = l+r >> 1;
        return max(query(tl, tr, i << 1, l, mid), query(tl, tr, i << 1 | 1, mid+1, r));
    }
};

struct Fenwick {
    long f[N];
    void update(int i, int v) { while(i < N) f[i] += v, i += i & -i; }
    long query(int i) {
        long ret = 0;
        while(i > 0) ret += f[i], i -= i & -i;
        return ret;
    }
};

class BubbleSort {
    int n, a[N], lo[N], hi[N];
    vector<int> lsr, lsl;
    priority_queue<pii, vector<pii>, greater<pii> > pq;
    LazySegTree *st;
    long org_inv, ans;

    void compress() {
        vector<int> vals;
        for(int i = 1; i <= n; i++) vals.push_back(a[i]);
        sort(vals.begin(), vals.end());
        vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
        for(int i = 1; i <= n; i++) a[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin() + 1;
    }

    void compute_inversions() {
        Fenwick *f = new Fenwick();
        for(int i = 1; i <= n; i++) {
            org_inv += f->query(N - 1) - f->query(a[i]);
            f->update(a[i], 1);
        }
    }

    void add(int i, int x) {
        // note: hi[i] may have the same value as a[i], but won't be on the same index (refer to RE submissions to see assert that went false)
        if(lo[i] == -1 && hi[i] == -1) return; // happens everytime i is in lsl
        st->update(lo[i] + 1, hi[i] - 1, x);
        st->update(lo[i], lo[i], x - (a[lo[i]] == a[i]));
        if(lo[i] != hi[i]) {
            st->update(hi[i], hi[i], x - (a[hi[i]] == a[i]));
            assert(a[lo[i]] != a[hi[i]]);
        }
        pq.emplace(a[i], i);
    }

    void rem(int i, int x) {
        if(lo[i] == -1 && hi[i] == -1) return;
        st->update(lo[i] + 1, hi[i] - 1, -x);
        st->update(lo[i], lo[i], (a[lo[i]] == a[i]) - x);
        if(lo[i] != hi[i]) {
            st->update(hi[i], hi[i], (a[hi[i]] == a[i]) - x);
            assert(a[lo[i]] != a[hi[i]]);
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1; i <= n; i++) {
            cin >> a[i];
        }
        compress();
        compute_inversions();
        if(!org_inv) {
            map<int, int> mp;
            for(int i = 1; i <= n; i++) {
                ++mp[a[i]];
                if(mp[a[i]] > 1) {
                    cout << 0 << endl;
                    return;
                }
            }
            cout << 1 << endl;
            return;
        }
        for(int i = 1; i <= n; i++) {
            while(!lsr.empty() && a[lsr.back()] >= a[i]) lsr.pop_back(); // must be >= and not >, as we need to guarantee none have the same value
            lsr.push_back(i);
            if(lsl.empty() || a[lsl.back()] < a[i]) lsl.push_back(i); // must be strictly < , as we need to guarantee none have the same value
        }

//        for(int i: lsr) cerr << i << ' ';
//        cerr << endl;
//        for(int i: lsl) cerr << i << ' ';
//        cerr << endl;

        for(int i = 1; i <= n; i++) {
            auto itl = lower_bound(lsl.begin(), lsl.end(), i, [&] (int i, int j) { return a[i] < a[j]; }); // need to include the ones with equal value too
            auto itr = lower_bound(lsl.begin(), lsl.end(), i); // strictly less than i, as we can't use the same __index__ as a bound and as a contributor
            --itr;
            if(itr < itl) lo[i] = hi[i] = -1;
            else lo[i] = *itl, hi[i] = *itr;
            // note: if i is in lsl, then itr < itl
        }

        st = new LazySegTree();
        for(int i = 0; i < lsr.size(); i++) {
//            cerr << "less than a[lsr[i]]" << endl;
            while(!pq.empty() && pq.top().first < a[lsr[i]]) {
                int idx = pq.top().second;
                pq.pop();
                if(!i || a[idx] != a[lsr[i-1]]) rem(idx, 2); // need to check with a[lsr[i-1]] to check which kind of removal we want
                else rem(idx, 1);
            }
//            cerr << "equal to a[lsr[i]]" << endl;
            vector<int> tmp;
            while(!pq.empty() && pq.top().first == a[lsr[i]]) tmp.push_back(pq.top().second), pq.pop();
            for(int idx: tmp) {
                rem(idx, 2); // guaranteed to be > a[lsr[i-1]], a[idx] == a[lsr[i]] > a[lsr[i-1]]
                add(idx, 1); // add it back in (this we in turn add into pq in add function), as we still need it
            }

//            cerr << "insert" << endl;
            for(int j = (i ? lsr[i - 1] + 1 : 1); j < lsr[i]; j++) {
                // must do it here because we need to first handle points below (or equal to) right bound's value before we add in anything new
                // otherwise, we have to separate into which ones came in the prefix before lsr[i-1] and which ones came after
                if(a[j] < a[lsr[i]]) continue;
                if(a[j] == a[lsr[i]]) add(j, 1);
                else add(j, 2);
            }

            // we were clever here to use lo, hi that were already precomputed without having to do a binary search again
            int lb = lo[lsr[i]] + (a[lo[lsr[i]]] == a[lsr[i]]); // only consider a[l], a[r] without the same value
            int rb = hi[lsr[i]];
//            cerr << "query " << lsr[i] << ' ' << lb << ' ' << rb << ' ' << st->query(lb, rb)+1 << endl << endl;
            ans = max(st->query(lb, rb) + 1, ans);
        }
//        cerr << org_inv << ' ' << ans << endl;
        cout << org_inv - ans << endl;
    }
};
// CHECK CASE WHERE DUPLICATES ARE ALLOWED
// HOW SHOULD HI/LO BE INCLUDED IN BOUNDS?
// Observe that if candidate in lsl, it will not update any range (void it from updating)
// If candidate is in lsr, it will also not update any range
// Can a value be candidate in lsl and lsr at the same time?
// Not t[1], need to query valid range
// a[l], a[r] should not have the same value

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        BubbleSort *obj = new BubbleSort();
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
