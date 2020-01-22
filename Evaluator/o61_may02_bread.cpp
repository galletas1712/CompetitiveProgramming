// hard part is counting # times minimum appears in range -> just augment lazy segment tree
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 17;

class Bread {

    int n, a[N];
    vector<int> ls[N];
    int t[2*N], cnt[2*N], lz[2*N];
    long ans;

    void apply(int i) {
        if(!lz[i]) return;
        t[i] += lz[i];
        if(i < N) {
            lz[i << 1] += lz[i];
            lz[i << 1 | 1] += lz[i];
        }
        lz[i] = 0;
    }

    void build(int i = 1, int l = 0, int r = N-1) {
        cnt[i] = r-l+1;
        t[i] = 0;
        if(l == r) return;
        int mid = l+r >> 1;
        build(i << 1, l, mid);
        build(i << 1 | 1, mid+1, r);
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
        t[i] = min(t[i << 1], t[i << 1 | 1]);
        cnt[i] = 0;
        if(t[i] == t[i << 1]) cnt[i] += cnt[i << 1];
        if(t[i] == t[i << 1 | 1]) cnt[i] += cnt[i << 1 | 1];
    }

    pair<int,int> query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
        apply(i);
        if(r < tl || l > tr) return {INF, INF};
        if(tl <= l && r <= tr) return {t[i], cnt[i]};
        int mid = l+r >> 1;
        auto retL = query(tl, tr, i << 1, l, mid);
        auto retR = query(tl, tr, i << 1 | 1, mid+1, r);
        pair<int,int> ret = (retL.first == retR.first ? make_pair(retL.first, retL.second + retR.second) : min(retL, retR));
        return ret;
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0; i < n; i++) cin >> a[i];
        build();
        for(int i = 0; i < n; i++) ls[i].push_back(-1);
        for(int i = 0; i < n; i++) {
            if(ls[a[i]].size() > 5) {
                int len = ls[a[i]].size();
                update(ls[a[i]][len - 6] + 1, ls[a[i]][len - 5], -1);
            }
            ls[a[i]].push_back(i);
            if(ls[a[i]].size() > 5) {
                int len = ls[a[i]].size();
                update(ls[a[i]][len - 6] + 1, ls[a[i]][len - 5], 1);
            }
            pair<int,int> tmp = query(0, i-1);
            if(tmp.first == INF) continue;
            ans += i - tmp.second;
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Bread *obj = new Bread();
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
