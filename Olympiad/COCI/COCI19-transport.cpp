/*
 * Centroid decomposition
 * current sum up [v] >= minimum prefix sum down [u] where minimum prefix sum up [v] >= 0
 * use offline query sorting and use a fenwick to maintain subtree sums
 */
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 1e5+1;

void update(int i, vector<long> &fen) {
    while(i < fen.size()) ++fen[i], i += i & -i;
}

long query(int i, vector<long> &fen) {
    long ret = 0;
    while(i > 0) ret += fen[i], i -= i & -i;
    return ret;
}

int n, a[N];
vector<pair<int,int> > g[N];
long ans;

int cen, sz[N];
bool blocked[N];
void find_centroid(int u, int par, int tot, pair<int,int> &tmp) {
    sz[u] = 1;
    int mx = 0;
    for(auto p: g[u]) {
        int v, w;
        tie(v, w) = p;
        if(v == par || blocked[v]) continue;
        find_centroid(v, u, tot, tmp);
        sz[u] += sz[v];
        mx = max(sz[v], mx);
    }
    mx = max(tot - sz[u], mx);
    if(mx < tmp.first) tmp = {mx, u};
}

long fup[N], fdown[N], cup[N];
int tick, st[N], en[N];
vector<pair<long, int> > ls;
void dfs1(int u, int par, int child_subtree, long cdown = 0) {
    st[u] = ++tick;
    for(auto p: g[u]) {
        int v, w;
        tie(v, w) = p;
        if(v == par || blocked[v]) continue;
        fup[v] = min(fup[u], 0ll) + a[v] - w;
        cup[v] = cup[u] + a[v] - w;
        fdown[v] = min(cdown + a[u] - w, fdown[u]);
        dfs1(v, u, (st[u] == 1 ? v : child_subtree), cdown + a[u] - w);
    }
    en[u] = tick;
    if(st[u] != 1) {
        if(fup[u] >= 0) {
            ++ans;
            ls.emplace_back(-cup[u], -u);
        }
        if(fdown[u] >= 0) {
            ++ans;
        }
        assert(child_subtree != -1);
        ls.emplace_back(fdown[u], child_subtree);
    }
}


void dec(int src, int tot) {
    pair<int,int> tmp = {tot, -1};
    find_centroid(src, -1, tot, tmp);
    cen = tmp.second;

    tick = 0;
    ls.clear();
    fup[cen] = fdown[cen] = cup[cen] = 0;

    dfs1(cen, -1, -1);

    sort(ls.begin(), ls.end());
    vector<long> fen(tick+1);
    long tmp_ans = 0;

    for(auto p: ls) {
        int v;
        tie(ignore, v) = p;
        if(v < 0) {
            update(st[-v], fen);
        } else { // don't consider centroid for this section
            tmp_ans += query(st[v]-1, fen) + query(tick, fen) - query(en[v], fen);
        }
    }
    ans += tmp_ans;

    blocked[cen] = true;
    for(auto p: g[cen]) {
        int v, w;
        tie(v, w) = p;
        if(blocked[v]) continue;
        dec(v, (sz[v] > sz[cen] ? tot - sz[cen] : sz[v]));
    }
}

void solve(istream& cin) {
    cin >> n;
    for(int i = 1; i <= n; i++) cin >> a[i];
    for(int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    dec(1, n);
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve(cin);
}
