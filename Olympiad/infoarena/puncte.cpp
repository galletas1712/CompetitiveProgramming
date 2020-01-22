// Expand formula and do CHT. Either Dynamic LiChao or we can sort the points and queries and use O(n) CHT.
#include <bits/stdc++.h>
#define long long long
using namespace std;
const long LINF = LLONG_MAX, INF = 1e9+1;

struct Line {
    long m, c;
    Line(long m, long c): m(m), c(c) {}
    long get(long x) { return m*x + c; }
};

struct Node {
    Line v;
    Node *l, *r;
    Node(Line v): v(v), l(NULL), r(NULL) {}
};

void update(Node* &t, long l, long r, Line v) {
    if(!t) {
        t = new Node(v);
        return;
    }
    if(t->v.get(l) < v.get(l) && t->v.get(r) < v.get(r)) return;
    if(l == r || t->v.get(l) > v.get(l) && t->v.get(r) > v.get(r)) {
        t->v = v;
        return;
    }
    long mid = l+r >> 1;
    if(t->v.get(mid) > v.get(mid)) swap(t->v, v);
    if((t->v.get(l) > v.get(l)) == (t->v.get(mid) > v.get(mid))) {
        update(t->r, mid+1, r, v);
    } else {
        update(t->l, l, mid, v);
    }
}

long query(Node *t, long l, long r, long x) {
    if(!t) return LINF;
    if(l == r) return t->v.get(x);
    long ret = t->v.get(x);
    long mid = l+r >> 1;
    if(x <= mid) {
        ret = min(query(t->l, l, mid, x), ret);
    } else {
        ret = min(query(t->r, mid+1, r, x), ret);
    }
    return ret;
}

long n, m;
Node *root;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    ifstream cin("puncte.in");
    ofstream cout("puncte.out");
    cin >> n >> m;
    vector<pair<long, long> > lines;
    for(long i = 0; i < n; i++) {
        long x, y;
        cin >> x >> y;
        Line add = Line(-2ll * x, 1ll * x * x + 1ll * y * y);
        update(root, -INF, INF, add);
    }
    for(long i = 0; i < m; i++) {
        long x;
        cin >> x;
        cout << query(root, -INF, INF, x) + x * x << '\n';
    }
}
