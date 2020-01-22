#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 2e5+10;
const long LINF = 1e17;

int n, m, A, B;
vector<pair<int,int> > g[N];

vector<int> tmp, scaffold;
long S[N];
bool in_path[N];
int rev[N];
void dfs_scaffold(int u, int par) {
    tmp.push_back(u);
    if(u == B) {
        for(int i = 0; i < tmp.size(); i++) {
           rev[tmp[i]] = i;
           scaffold.push_back(tmp[i]);
           in_path[tmp[i]] = true;
        }
    }
    for(auto p: g[u]) {
        int v, w;
        tie(v, w) = p;
        if(v == par) continue;
        S[v] = S[u] + w;
        dfs_scaffold(v, u);
    }
    tmp.pop_back();
}

long sub[N];
long dfs_subtree(int u, int par, int src) {
    long ret = 0;
    for(auto p: g[u]) {
        int v, w;
        tie(v, w) = p;
        if(v == par || in_path[v]) continue;
        long tmp = dfs_subtree(v, u, src);
        ret = max(tmp + w, ret);
    }
    return ret;
}

long t[2*N], vl[2*N], vr[2*N];

void build() {
    for(int i = 0; i < scaffold.size(); i++) {
        t[scaffold.size() + i] = -LINF;
        vl[scaffold.size() + i] = sub[scaffold[i]] - S[scaffold[i]];
        vr[scaffold.size() + i] = sub[scaffold[i]] + S[scaffold[i]];
    }
    for(int i = scaffold.size() - 1; i >= 1; i--) {
        t[i] = max(t[i << 1], t[i << 1 | 1]);
        t[i] = max(vl[i << 1] + vr[i << 1 | 1], t[i]);
        vl[i] = max(vl[i << 1], vl[i << 1 | 1]);
        vr[i] = max(vr[i << 1], vr[i << 1 | 1]);
    }
}

long query(int l, int r) {
    long ret = -LINF;
    vector<int> nodes_l, nodes_r, nodes;
    for(l += scaffold.size(), r += scaffold.size() + 1; l < r; l >>= 1, r >>= 1) {
        if(l & 1) nodes_l.push_back(l++);
        if(r & 1) nodes_r.push_back(--r);
    }
    nodes = nodes_l;
    reverse(nodes_r.begin(), nodes_r.end());
    for(int i: nodes_r) nodes.push_back(i);
    long mxl = -LINF;
    for(int i: nodes) {
        ret = max(t[i], ret);
        ret = max(vr[i] + mxl, ret);
        mxl = max(vl[i], mxl);
    }
    return ret;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m >> A >> B;
    for(int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    dfs_scaffold(A, -1);
    for(int v: scaffold) {
        sub[v] = dfs_subtree(v, -1, v);
    }
    build();
    for(int i = 0, C, D; i < m; i++) {
        cin >> C >> D;
        if(rev[C] > rev[D]) swap(C, D);
        cout << query(rev[C], rev[D]) << '\n';
    }
}
