#include <bits/stdc++.h>
#define long int64_t
using namespace std;

int n, T, M1, M2;

struct node {
    int cnt, prior;
    node *l, *r;
    long A, B, sum, mx;
    node(long A, long B): cnt(1), prior(rand()), l(nullptr), r(nullptr), sum(A), mx(A + B), A(A), B(B) {}
};

typedef node* pnode;
pnode root;

void update(pnode t) {
    if(!t) return;
    t->cnt = (t->l ? t->l->cnt : 0) + (t->r ? t->r->cnt : 0) + 1;
    t->sum = (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0) + t->A;
    t->mx = (t->l ? t->l->sum : 0) + t->A + t->B;
    t->mx = max((t->l ? t->l->mx : 0), t->mx);
    t->mx = max((t->l ? t->l->sum : 0) + t->A + t->B, t->mx);
    t->mx = max((t->l ? t->l->sum : 0) + t->A + (t->r ? t->r->mx : 0), t->mx);
}

void split(pnode t, pnode &l, pnode &r, int pos, int add = 0) {
    if(!t) return void(l = r = nullptr);
    int curr_key = add + 1 + (t->l ? t->l->cnt : 0);
    if(pos < curr_key) {
        split(t->l, l, t->l, pos, add);
        r = t;
    } else {
        split(t->r, t->r, r, pos, curr_key);
        l = t;
    }
    update(t);
}

void merge(pnode &t, pnode l, pnode r) {
    if (!l || !r) return void(t = (l ? l : r));
    if (l->prior > r->prior) {
        merge(l->r, l->r, r);
        t = l;
    } else {
        merge(r->l, l, r->l);
        t = r;
    }
    update(t);
}

void insert(pnode &t, int pos, int A, int B) {
    pnode p1, p2;
    split(t, p1, p2, pos);
    merge(p1, p1, new node(A, B));
    merge(t, p1, p2);
}

long query_prefix(pnode t, int pos, int add = 0) {
    if(!t) return 0;
    int curr_key = add + 1 + (t->l ? t->l->cnt : 0);
    long ret = 0;
    if(curr_key <= pos) {
        ret += (t->l ? t->l->sum : 0) + t->A;
        ret += query_prefix(t->r, pos, curr_key);
    } else {
        ret += query_prefix(t->l, pos, add);
    }
    return ret;
}

long query_suffix(pnode t, int pos, int add = 0, long sum = 0) {
    if(!t) return 0;
    int curr_key = add + 1 + (t->l ? t->l->cnt : 0);
    long ret = 0;
    long sum2 = sum + (t->l ? t->l->sum : 0) + t->A;
    if(curr_key >= pos) {
        ret = max(max(sum2 + (t->r ? t->r->mx : 0), sum + (t->l ? t->l->sum : 0) + t->A + t->B), ret);
        ret = max(query_suffix(t->l, pos, add, sum), ret);
    } else {
        ret = max(query_suffix(t->r, pos, curr_key, sum2), ret);
    }
    return ret;
}

void print_treap(pnode t) {
    if(!t) return;
    print_treap(t->l);
    cerr << "print " << t->A << ' ' << t->B << endl;
    print_treap(t->r);
}

int main() {
    srand(100);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> T >> M1 >> M2;

    long S = 0;
    for(int i = 1; i <= n; i++) {
        long x, y, A, B;
        cin >> x >> y;
        if(T == 1) {
            A = x;
            B = y;
        } else {
            A = x + (S % M1);
            B = y + (S % M2);
        }

        int l = 0, r = i;
        int pos = -1;
        while(l < r - 1) {
            int mid = l+r >> 1;
            long res_prefix = query_prefix(root, mid) + A + B;
            long res_suffix = query_suffix(root, mid + 1) + A;
            if(res_prefix < res_suffix) l = mid;
            else if(res_prefix > res_suffix) r = mid;
            else {
                pos = mid;
                break;
            }
        }
        if(pos == -1) {
            if(max(query_prefix(root, l) + A + B, query_suffix(root, l + 1) + A) <= max(query_prefix(root, r) + A + B, query_suffix(root, r + 1) + A)) {
                pos = l;
            } else {
                pos = r;
            }
        }
        insert(root, pos, A, B); // inserts after pos
        S = query_suffix(root, 1);
        cout << S << '\n';
    }
}
