/*
 * Use segment tree on compressed segments. Segment is accessed via right bound
 * Use the "most recent" property of lazy segment tree to maintain the sum and maximum prefix sum in interval
 * We also need to know what the current slope at a segment is. To do this, use the RMQ range updates and point query method.
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 1 << 18;
const int INF = 1e9+1;

int X;
vector<tuple<int,int,int> > queries;
vector<int> vals;

int sum[2*N], pref[2*N], lz[2*N];
pair<int,int> curr[2*N];

void apply(int i, int l, int r) {
    if(lz[i] == -INF) return;
    sum[i] = pref[i] = lz[i] * (vals[r] - vals[l - 1]);
    if(i < N) {
        lz[i << 1] = lz[i];
        lz[i << 1 | 1] = lz[i];
    }
    lz[i] = -INF;
}

void update(int tl, int tr, int d, int tick, int i = 1, int l = 0, int r = N-1) {
    apply(i, l, r);
    if(l > tr || r < tl) return;
    if(tl <= l && r <= tr) {
        lz[i] = d;
        curr[i] = {tick, d};
        apply(i, l, r);
        return;
    }
    int mid = l+r >> 1;
    update(tl, tr, d, tick, i << 1, l, mid);
    update(tl, tr, d, tick, i << 1 | 1, mid+1, r);
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
    pref[i] = max(pref[i << 1], sum[i << 1] + pref[i << 1 | 1]);
}

pair<int,int> query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
    apply(i, l, r);
    if(l > tr || r < tl) return {0, 0};
    if(tl <= l && r <= tr) return {pref[i], sum[i]};
    int mid = l+r >> 1;
    auto retL = query(tl, tr, i << 1, l, mid);
    auto retR = query(tl, tr, i << 1 | 1, mid+1, r);
    return {max(retL.first, retL.second + retR.first), retL.second + retR.second};
}

pair<int,int> get_curr(int targ, int i = 1, int l = 0, int r = N-1) {
    apply(i, l, r);
    if(l == r) return curr[i];
    int mid = l+r >> 1;
    if(targ <= mid)
        return max(get_curr(targ, i << 1, l, mid), curr[i]);
    else
        return max(get_curr(targ, i << 1 | 1, mid+1, r), curr[i]);
}

void solve(istream& cin) {
    cin >> X;
    while(true) {
        char inst;
        cin >> inst;
        if(inst == 'E') break;
        else if(inst == 'I') {
            int l, r, d;
            cin >> l >> r >> d;
            queries.emplace_back(l-1, r, d);
            vals.push_back(l-1);
            vals.push_back(r);
        } else {
            int h;
            cin >> h;
            queries.emplace_back(-INF, -INF, h);
        }
    }

    vals.push_back(0);
    vals.push_back(X);
    sort(vals.begin(), vals.end());
    vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
    fill(lz, lz+2*N, -INF);

    int tick = 0;
    for(auto &tt: queries) {
        if(get<0>(tt) != -INF) {
            get<0>(tt) = lower_bound(vals.begin(), vals.end(), get<0>(tt)) - vals.begin() + 1;
            get<1>(tt) = lower_bound(vals.begin(), vals.end(), get<1>(tt)) - vals.begin();
            int l, r, d;
            tie(l, r, d) = tt;
            update(l, r, d, ++tick);
        } else {
            int h = get<2>(tt);
            int l = 0, r = vals.size() - 1;
            while(l < r) {
                int mid = l+r+1 >> 1;
                if(query(0, mid).first <= h) l = mid;
                else r = mid-1;
            }
            pair<int,int> curr = get_curr(l + 1);
            int res = (curr.second ? (h - query(0, l).second) / curr.second : 0) + vals[l]; // first?
            cout << res << '\n';
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve(cin);
}
