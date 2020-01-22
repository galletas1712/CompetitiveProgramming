#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 6e5+1;

int n, orgl[N], orgr[N], orgw[N];
long ans, dp[N][2];
vector<pair<int,int> > ls[N];

vector<int> xs;
void preprocess() {
    sort(xs.begin(), xs.end());
    xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
    assert(xs.size() == n * 2);
    for(int i = 0; i < n; i++) {
        orgr[i] = lower_bound(xs.begin(), xs.end(), orgr[i]) - xs.begin();
        orgl[i] = lower_bound(xs.begin(), xs.end(), orgl[i]) - xs.begin();
        ls[orgr[i]].emplace_back(orgl[i], orgw[i]);
    }
}

struct SegmentTree {
    long t[2*N];
    void update(int i, long v) {
        for(t[i += N] = v; i > 1; i >>= 1) t[i>>1] = max(t[i], t[i^1]);
    }

    long query(int l, int r) {
        long res = 0;
        for(l += N, r += N+1; l < r; l >>= 1, r >>= 1) {
            if(l & 1) res = max(t[l++], res);
            if(r & 1) res = max(t[--r], res);
        }
        return res;
    }
} st1, st2;

void solve_large() {
    for(int r = 0; r < xs.size(); r++) {
        if(r) {
            dp[r][0] = dp[r-1][0];
            dp[r][1] = dp[r-1][1];
        }
        long mxw = 0;
        for(auto tt: ls[r]) {
            long l, w;
            tie(l, w) = tt;
            mxw = max(w, mxw);
            dp[r][0] = max((l ? dp[l][0] : 0) + w, dp[r][0]);
            dp[r][1] = max((l ? dp[l][1] : 0) + w, dp[r][1]);
            dp[r][1] = max(st1.query(l, r) + w, dp[r][1]);
            dp[r][1] = max((l ? dp[l][0] : 0) + st2.query(l, r) + w, dp[r][1]);
        }
        st1.update(r, dp[r][0]);
        st2.update(r, mxw);
        ans = max(max(dp[r][0], dp[r][1]), ans);
    }
}

int main() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> orgl[i] >> orgr[i] >> orgw[i];
        xs.push_back(orgr[i]);
        xs.push_back(orgl[i]);
    }
    preprocess();
    solve_large();
    cout << ans << endl;
}
