#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 1 << 18;
const long LINF = 1e16;
const int INF = 1e9+2;

int n;
vector<tuple<int,int,int> > org;
vector<int> vals;
vector<pair<int,int> > ls_pref[N], ls_suff[N];
long pref_one[N], suff_one[N], pref[N], suff[N];
long sum, ans = LINF;

struct LazySegTree {

    long t[2*N], lz[2*N];
    long f[N];

    void apply(int i) {
        t[i] += lz[i];
        if(i < N) {
            lz[i << 1] += lz[i];
            lz[i << 1 | 1] += lz[i];
        }
        lz[i] = 0;
    }

    void build() {
        fill(t,t + 2*N, 0);
        fill(lz, lz + 2*N, 0);
    }

    void update(int tl, int tr, int v, int i = 1, int l = 0, int r = N - 1) {
        apply(i);
        if(r < tl || l > tr) return;
        if(tl <= l && r <= tr) {
            lz[i] += v;
            apply(i);
            return;
        }
        int mid = l+r >> 1;
        update(tl, tr, v, i << 1, l, mid);
        update(tl, tr, v, i << 1 | 1, mid + 1, r);
        t[i] = min(t[i << 1], t[i << 1 | 1]);
    }

    long query(int tl, int tr, int i = 1, int l = 0, int r = N - 1) {
        apply(i);
        if(r < tl || l > tr) return LINF;
        if(tl <= l && r <= tr) return t[i];
        int mid = l+r >> 1;
        return min(query(tl, tr, i << 1, l, mid), query(tl, tr, i << 1 | 1, mid+1, r));
    }

} seg;

void compress() {
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
    for(auto tt: org) {
        int a, b, c;
        tie(a, b, c) = tt;
        a = lower_bound(vals.begin(), vals.end(), a) - vals.begin();
        b = lower_bound(vals.begin(), vals.end(), b) - vals.begin();
        ls_pref[b].emplace_back(a, c);
        ls_suff[a].emplace_back(b, c);
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for(int i = 0, a, b, c; i < n; i++) {
        cin >> a >> b >> c;
        org.emplace_back(a, b, c);
        vals.push_back(a);
        vals.push_back(b);
        sum += c;
    }
    compress();
    for(int r = 0; r < vals.size(); r++) { // off by one error is likely to happen here
        if(!r) continue;
        pref_one[r] = pref_one[r - 1];
        for(auto p: ls_pref[r - 1]) {
            pref_one[r] += p.second;
        }
    }
    for(int l = vals.size() - 1; l >= 0; l--) {
        if(l == vals.size() - 1) continue;
        suff_one[l] = suff_one[l + 1];
        for(auto p: ls_suff[l + 1]) {
            suff_one[l] += p.second;
        }
    }
    seg.build();
    for(int r = 0; r < vals.size(); r++) {
        if(!r) pref[r] = 0;
        else pref[r] = min(seg.query(0, r), pref_one[r]);
        for(auto p: ls_pref[r]) { // always update after
            seg.update(0, p.first - 1, p.second);
            seg.update(r + 1, N - 1, p.second);
        }
    }
    seg.build();
    for(int l = vals.size() - 1; l >= 0; l--) {
        if(l == vals.size() - 1) suff[l] = 0;
        else suff[l] = min(seg.query(l, N - 1), suff_one[l]);
        for(auto p: ls_suff[l]) {
            seg.update(p.first + 1, N - 1, p.second);
            seg.update(0, l - 1, p.second);
        }
    }
    for(int i = 0; i < vals.size(); i++) {
        ans = min(pref[i] + suff[i], ans);
    }
    cout << sum - ans << endl;
}
