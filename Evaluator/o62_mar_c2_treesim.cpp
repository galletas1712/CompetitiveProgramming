#include <bits/stdc++.h>
#define long long long
using namespace std;
const long P1 = 31, P2 = 37;
const long MOD = 1e9+7;
const int N = 5e5+1;

long p1[N], p2[N];

/////START
struct node {
    int prior, cnt;
    long key, h1, h2;
    node *l, *r;
    node(long key): key(key), prior(rand()), cnt(1), h1(key), h2(key), l(nullptr), r(nullptr) {}
};

typedef node* pnode;

pnode root;

void update(pnode t) {
    if(!t) return;
    t->cnt = (t->l ? t->l->cnt : 0) + (t->r ? t->r->cnt : 0) + 1;
    int shift = t->r ? t->r->cnt : 0;
    t->h1 = (t->r ? t->r->h1 : 0) + p1[shift] * t->key + (t->l ? p1[shift + 1] * t->l->h1 : 0);
    t->h1 %= MOD;
    t->h2 = (t->r ? t->r->h2 : 0) + p2[shift] * t->key + (t->l ? p2[shift + 1] * t->l->h2 : 0);
    t->h2 %= MOD;
}

void split(pnode t, pnode &l, pnode &r, long v) {
    if(!t) return void(l = r = nullptr);
    if(v < t->key) {
        split(t->l, l, t->l, v);
        r = t;
    } else {
        split(t->r, t->r, r, v);
        l = t;
    }
    update(t);
}

void merge(pnode &t, pnode l, pnode r) {
    if(!l || !r) return void(t = (l ? l : r));
    if(l->key > r->key) swap(l, r);
    if(l->prior > r->prior) {
        merge(l->r, l->r, r);
        t = l;
    } else {
        merge(r->l, l, r->l);
        t = r;
    }
    update(t);
}

//void insert(pnode &t, int v) {
//    if(!t) return void(t = new node(v));
//    if(v <= t->key) insert(t->l, v);
//    else insert(t->r, v);
//}

void insert(pnode& t, int v) {
//    cerr << "insert " << v << endl;
    pnode p1, p2;
    split(t, p1, p2, v);
    merge(p1, p1, new node(v));
    merge(t, p1, p2);
}
//
void remove(pnode& t, int v) {
//    cerr << "remove " << v << endl;
    pnode p1, p2, p3;
    split(t, p1, p2, v-1);
    split(p2, p2, p3, v);
    merge(t, p1, p3);
    delete p2;
}
void clear(pnode t) {
    if(!t) return;
    clear(t->l);
    clear(t->r);
    delete t;
}

void print(pnode t) {
    if(!t) return;
    print(t->l);
    cerr << t->key << ' ' << t->prior << ' ' << t->h1 << ' ' << t->h2 << endl;
    print(t->r);
}
//////END

int n;
vector<int> g[N];
vector<pair<long, long> > h1, h2; // 1, 2, have different definitions here

int tick, sz[N], st[N], en[N], rev[N];
void dfs1(int u, int p) {
    sz[u] = 1;
    if(p) {
        g[u].erase(find(g[u].begin(), g[u].end(), p));
    }
    for(int &v: g[u]) {
        dfs1(v, u);
        sz[u] += sz[v];
        if(sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
    }
}

void dfs_times(int u) {
    st[u] = ++tick;
    rev[st[u]] = u;
    for(int v: g[u]) {
        dfs_times(v);
    }
    en[u] = tick;
//    cerr << u << ' ' << st[u] << ' ' << en[u] << endl;
}

void dfs2(int u, vector<pair<long, long> > &store, bool keep) {
    if(g[u].empty()) {
        print(root);
//        cerr << "node " << u << endl;
        store.emplace_back(u, u);
        if(keep) insert(root, u);
        return;
    }
    for(int v: g[u]) {
        if(v == g[u][0]) continue;
        dfs2(v, store, 0);
    }
    dfs2(g[u][0], store, 1);
    for(int v: g[u]) {
        if(v == g[u][0]) continue;
        for(int i = st[v]; i <= en[v]; i++) {
            if(rev[i] <= n) {
                insert(root, rev[i]);
            }
        }
    }
    store.emplace_back(root->h1, root->h2);
    if(!keep) {
        clear(root);
        root = nullptr;
    }
}

int main() {
    srand(time(NULL));
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;

    p1[0] = p2[0] = 1;
    for(int i = 1; i <= n; i++) {
        p1[i] = p1[i-1] * P1 % MOD;
        p2[i] = p2[i-1] * P2 % MOD;
    }
    for(int i = 0, u, v; i < 2*n - 3; i++) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dfs1(1, 0);
    dfs_times(1);
    dfs2(1, h1, 0);
    for(int i = 1; i <= 2*n - 2; i++) g[i].clear();

    for(int i = 0, u, v; i < 2*n - 3; i++) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    tick = 0;
    dfs1(1, 0);
    dfs_times(1);
    dfs2(1, h2, 0);

    sort(h1.begin(), h1.end());
    sort(h2.begin(), h2.end());
    vector<pair<long, long> > h_int;
    set_intersection(h1.begin(), h1.end(), h2.begin(), h2.end(), back_inserter(h_int));
    cout << h1.size() - h_int.size() << endl;
}
