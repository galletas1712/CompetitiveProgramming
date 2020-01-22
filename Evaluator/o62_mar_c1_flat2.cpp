#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1, INF = 1e9;

int n, w, a[N], t[2*N], lb[N], rb[N], ans[N];

int query(int l, int r) {
    int res = INF;
    for(l += n, r += n+1; l < r; l >>= 1, r >>= 1) {
        if(l & 1) res = min(t[l++], res);
        if(r & 1) res = min(t[--r], res);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> w;
    memset(ans, -1, sizeof(ans));
    for(int i = 0; i < n; i++) {
        cin >> a[i];
        t[n+i] = a[i];
    }
    for(int i = n-1; i >= 1; i--) t[i] = min(t[i << 1], t[i << 1 | 1]);
    for(int i = 0; i < n; i++) {
        int l = 0, r = i;
        while(l < r) {
            int mid = l+r >> 1;
            if(query(mid, i) >= a[i]) r = mid;
            else l = mid + 1;
        }
        lb[i] = l;
        l = i, r = n-1;
        while(l < r) {
            int mid = l+r+1 >> 1;
            if(query(i, mid) >= a[i]) l = mid;
            else r = mid-1;
        }
        rb[i] = l;
        ans[rb[i] - lb[i]] = max(a[i], ans[rb[i] - lb[i]]);
//        cout << i << ' ' << lb[i] << ' ' << rb[i] << endl;
    }
    if(w > 0) cout << ans[w] << endl;
    else {
        for(int i = 1; i <= n; i++) cout << ans[i] << ' ';
        cout << endl;
    }
}
