/*
 * Problem basically reduces to APIO07-backup
 * At first, we take all positive values
 * If the number of segments is too much, we either erase a positive segment or add in a non-positive segment
 * This is equivalent to subtracting segment sums
 * Use the same proof of APIO07-backup for greedy (including consideration for undo operations)
 * One thing to be careful of is non-positive prefixes/suffixes. These need to be taken out first, as they don't contribute to lowering segment count and are suboptimal anyways
Key test:
17 1
11 4 5 -13 0 0 11 -9 9 12 -7 1 -2 7 -14 5 -15
 */

#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 3e5+1;

class Feast {

    int n, k, pre[N], nxt[N];
    long ans, a[N], s[N], res[N];
    priority_queue<tuple<long, int, int> > pq;
    bool mark[N];

public:

    void solve(istream &cin, ostream &cout) {
        auto input = [&] () {
            cin >> n >> k;
            vector<int> org(n);
            for(int i = 0; i < n; i++) cin >> org[i];
            int st = 0, en = n-1;
            while(st < n && org[st] <= 0) ++st;
            while(en >= 0 && org[en] <= 0) --en;
            if(st > en) {
                return false;
            }
            n = 0;
            for(int i = st; i <= en; i++) {
                a[++n] = org[i];
            }
            for(int i = 1; i <= n; i++) {
                s[i] = s[i-1] + a[i];
            }
            return true;
        };
        if(!input()) {
            cout << 0 << endl;
            return;
        }

        pre[0] = nxt[n] = INF;
        int last = 0;
        for(int i = 1; i <= n; i++) {
            if(i > 1 && (a[i] > 0) != (a[i-1] > 0)) {
                if(a[i-1] > 0) --k;
                res[i-1] = -abs(s[i-1] - s[last]);
                pre[i-1] = last;
                nxt[last] = i-1;
                pq.emplace(res[i-1], last, i-1);
                last = i-1;
            }
        }
        --k;
        res[n] = -abs(s[n] - s[last]);
        pre[n] = last;
        nxt[last] = n;
        pq.emplace(res[n], last, n);

        while(!pq.empty() && k < 0) {
            long cost;
            int l, r;
            tie(cost, l, r) = pq.top();
            pq.pop();

            if(mark[r] || mark[l] || cost != res[r]) continue;
            ++k;
            ans += res[r];
            mark[r] = mark[l] = true; // need to set mark for both l, r, as we need to prevent segments from being adjacent to each other

            int ll = pre[l], rr = nxt[r];
            if(ll < rr && ll >= 0 && rr <= n) {
                pre[rr] = ll, nxt[ll] = rr;
                res[rr] = res[rr] + res[l] - res[r];
                res[r] = -LINF;
            }

            if(ll < rr && ll >= 0 && rr <= n && !mark[ll] && !mark[rr]) {
                pq.emplace(res[rr], ll, rr);
            }
        }

        for(int i = 1; i <= n; i++) if(a[i] > 0) ans += a[i];

        cout << ans << endl;
    }
};


class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Feast *obj = new Feast();
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
