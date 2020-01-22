#include <bits/stdc++.h>
using namespace std;
const int N = 1 << 17;

int n, m;

int t[2*N];
bool lz[2*N];

void build(int i = 1, int l = 0, int r = N - 1) {
    if(l == r) {
        t[i] = 10;
        return;
    }
    int mid = l+r >> 1;
    build(i << 1, l, mid);
    build(i << 1 | 1, mid + 1, r);
    t[i] = t[i << 1] + t[i << 1 | 1];
}

void apply(int i) {
    if(lz[i]) t[i] = 0;
    else return;
    if(i < N) {
        lz[i << 1] |= lz[i];
        lz[i << 1 | 1] |= lz[i];
    }
    lz[i] = 0;
}

void blank_out(int tl, int tr, int i = 1, int l = 0, int r = N - 1) {
    apply(i);
    if(r < tl || l > tr) return;
    if(tl <= l && r <= tr) {
        lz[i] = 1;
        apply(i);
        return;
    }
    int mid = l+r >> 1;
    blank_out(tl, tr, i << 1, l, mid);
    blank_out(tl, tr, i << 1 | 1, mid + 1, r);
    t[i] = t[i << 1] + t[i << 1 | 1];
}

void update_point(int targ, int v, int i = 1, int l = 0, int r = N - 1) { // increment query, not assignment!
    apply(i);
    if(l == r) {
        t[i] += v;
        return;
    }
    int mid = l+r >> 1;
    if(targ <= mid) update_point(targ, v, i << 1, l, mid);
    else update_point(targ, v, i << 1 | 1, mid+1, r);
    t[i] = t[i << 1] + t[i << 1 | 1];
}

int query_sum(int tl, int tr, int i = 1, int l = 0, int r = N - 1) {
    apply(i);
    if(r < tl || l > tr) return 0 ;
    if(tl <= l && r <= tr) return t[i];
    int mid = l+r >> 1;
    return query_sum(tl, tr, i << 1, l, mid) + query_sum(tl, tr, i << 1 | 1, mid + 1, r);
}

int bsearch_right(int v, int i = 1, int l = 0, int r = N-1){
    apply(i);
    if(l == r) return l;
    int mid = l+r >> 1;
    if(t[i << 1] >= v) {
        return bsearch_right(v, i << 1, l, mid);
    } else {
        return bsearch_right(v - t[i << 1], i << 1 | 1, mid + 1, r);
    }
}
int bsearch_left(int v, int i = 1, int l = 0, int r = N-1) {
    apply(i);
    if(l == r) return i;
    int mid = l+r >> 1;
    if(t[i << 1 | 1] >= v) {
        return bsearch_left(v, i << 1 | 1, mid + 1, r);
    } else {
        return bsearch_left(v - t[i << 1 | 1], i << 1, l, mid);
    }
}

void deduct(int lb, int rb, int targ, int dir) { // decrement by targ, dir == 0 -> target towards left, dir == 1 -> right
    if(dir == 1) {
        int l = bsearch_right(query_sum(0, lb - 1) + targ);
        int org_sum = query_sum(lb, l - 1);
        targ -= org_sum;
        blank_out(lb, l - 1);
        update_point(l, -targ); // important negative sign here
    } else {
        int l = lb, r = rb;
        while(l < r) {
            int mid = l+r+1 >> 1;
            if(query_sum(mid, rb) >= targ) l = mid;
            else r = mid - 1;
        }
//        int l = bsearch_left(query_sum(rb + 1, N - 1) + targ); // should be using this bsearch_left instead of doing stupid binary search, but it bugs for some reason
        int org_sum = query_sum(l + 1, rb);
        targ -= org_sum;
        blank_out(l + 1, rb);
        update_point(l, -targ);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    --n;
    build();
    for(int z = 0, inst, idx; z < m; z++) {
        cin >> inst >> idx;
        --idx;
        if(inst == 1) { // update
            int targ;
            cin >> targ;
            int sum_l = query_sum(0, idx - 1);
            int sum_r = query_sum(idx + 1, n - 1);
            if(sum_l >= targ / 2 && sum_r >= targ / 2) {
                deduct(0, idx - 1, targ / 2, 0);
                deduct(idx + 1, n - 1, targ / 2, 1);
                update_point(idx, targ);
            } else if(sum_l < targ / 2) {
                blank_out(0, idx - 1);
                deduct(idx + 1, n - 1, targ - sum_l, 1);
                update_point(idx, targ);
            } else if(sum_r < targ / 2) {
                blank_out(idx + 1, n - 1);
                deduct(0, idx - 1, targ - sum_r, 0);
                update_point(idx, targ);
            } else {
                assert(false);
            }
        } else { // query
            cout << query_sum(idx, idx) << '\n';
        }
    }
}
//TODO: use bsearch_left
