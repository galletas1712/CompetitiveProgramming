#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
const int N = 1e5+1, INF = 1e9+1;

struct node {
    int key, prior;
    int cnt;
    node *l, *r;
    node(int key): key(key), cnt(1), prior(rand()), l(nullptr), r(nullptr) {}
};

typedef node* pnode;

void update_cnt(pnode t) {
    if(!t) return;
    t->cnt = (t->l ? t->l->cnt : 0) + (t->r ? t->r->cnt : 0) + 1;
}

void split(pnode t, pnode &l, pnode &r, int targ) {
    if(!t) return void(l = r = nullptr);
    if(targ >= t->key) {
        split(t->r, t->r, r, targ);
        l = t;
    } else {
        split(t->l, l, t->l, targ);
        r = t;
    }
    update_cnt(t);
}

void merge(pnode &t, pnode l, pnode r) {
    if(!l || !r) return void(t = l ? l : r);
    if(l->prior > r->prior) {
        merge(l->r, l->r, r);
        t = l;
    } else {
        merge(r->l, l, r->l);
        t = r;
    }
    update_cnt(t);
}

void insert(pnode &t, pnode v) {
    if(!t) t = v;
    else if(v->prior > t->prior) {
        split(t, v->l, v->r, v->key);
        t = v;
    } else {
        insert(v->key < t->key ? t->l : t->r, v);
    }
    update_cnt(t);
}

void insert(pnode &t, int val) {
    insert(t, new node(val));
}

int order_of_key(pnode t, int v) {
    if(!t) return 0;
    if(v > t->key) {
        return (t->l ? t->l->cnt : 0) + 1 + order_of_key(t->r, v);
    } else {
        return order_of_key(t->l, v);
    }
}

int n, q;
vector<int> g[N], gc[N];
pnode f1[N], f2[N];
//ordered_set<pair<int,int> > f1[N], f2[N];


int sz[N], par[N], depth_gc[N], dist[N][20];
bool blocked[N];
void find_centroid(int u, int par, int tot, pair<int,int> &ret) {
    sz[u] = 1;
    int mx = -INF;
    for(int v: g[u]) {
        if(v == par || blocked[v]) continue;
        find_centroid(v, u, tot, ret);
        sz[u] += sz[v];
        mx = max(sz[v], mx);
    }
    mx = max(tot - sz[u], mx);
    ret = min(make_pair(mx, u), ret);
}

void fill_dist(int u, int par, int cen, int curr) {
    dist[u][depth_gc[cen]] = curr;
    for(int v: g[u]) {
        if(v == par || blocked[v]) continue;
        fill_dist(v, u, cen, curr + 1);
    }
}

void dec(int src, int pre, int tot) {
    pair<int,int> tmp = {INF, INF};
    find_centroid(src, -1, tot, tmp);
    int cen = tmp.second;
    assert(cen != INF);

    blocked[cen] = true;
    par[cen] = pre;
    if(pre != -1) {
        gc[pre].push_back(cen);
//        cerr << pre << ' ' << cen << endl;
        depth_gc[cen] = depth_gc[pre] + 1;
    }
    fill_dist(cen, -1, cen, 0);

    for(int v: g[cen]) if(!blocked[v]) {
            dec(v, cen, (sz[v] < sz[cen] ? sz[v] : tot - sz[cen]));
        }
}

int main() {
    srand(time(NULL));
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> q;
    for(int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dec(1, -1, n);
    for(int i = 0, inst, u; i < q; i++) {
        cin >> inst;
        if (inst == 1) {
            int k;
            cin >> u >> k;
            for (int v = u; par[v] != -1; v = par[v]) {
                insert(f1[par[v]], k - dist[u][depth_gc[par[v]]]);
                insert(f2[v], k - dist[u][depth_gc[par[v]]]);
            }
            insert(f1[u], k);
        } else {
            cin >> u;
            int res = 0;
            for (int v = u; par[v] != -1; v = par[v]) {
                int top = order_of_key(f1[par[v]], dist[u][depth_gc[par[v]]]);
                int bottom = order_of_key(f2[v], dist[u][depth_gc[par[v]]]);
                res += (f1[par[v]] ? f1[par[v]]->cnt : 0) - top;
                res -= (f2[v] ? f2[v]->cnt : 0) - bottom;
            }
            res += (f1[u] ? f1[u]->cnt : 0) - order_of_key(f1[u], 0);
            cout << res << '\n';
        }
    }
//    for(int i = 0, inst, u; i < q; i++) {
//        cin >> inst;
//        if(inst == 1) {
//            int k;
//            cin >> u >> k;
//            for(int v = u; par[v] != -1; v = par[v]) {
//                f1[par[v]].insert(make_pair(k - dist[u][depth_gc[par[v]]], i));
//                f2[v].insert(make_pair(k - dist[u][depth_gc[par[v]]], i));
//            }
//            f1[u].insert(make_pair(k, i));
//        } else {
//            cin >> u;
//            int res = 0;
//            for(int v = u; par[v] != -1; v = par[v]) {
//                int top = f1[par[v]].order_of_key(make_pair(dist[u][depth_gc[par[v]]], -INF));
//                int bottom = f2[v].order_of_key(make_pair(dist[u][depth_gc[par[v]]], -INF));
//                res += f1[par[v]].size() - top;
//                res -= f2[v].size() - bottom;
//            }
//            res += f1[u].size() - f1[u].order_of_key(make_pair(0, -INF));
//            cout << res << '\n';
//        }
//    }
}
