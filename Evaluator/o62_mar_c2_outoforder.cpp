#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1;

int n, a[N], d[N];
vector<int> ls[N];
long ans;

int f[N];
void update(int i) {
    while(i <= n) ++f[i], i += i & -i;
}

int query(int i) {
    int res = 0;
    while(i > 0) res += f[i], i -= i & -i;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for(int i = 1; i <= n; i++) {
        cin >> a[i];
        ls[a[i]].push_back(i);
        assert(1 <= a[i] && a[i] <= n);
    }
    for(int i = 1; i <= n; i++) cin >> d[i];
    for(int x = 1; x <= n; x++) {
        for(int i: ls[x]) {
//            cerr << x << ' ' << i << endl;
            ans += query(min(i + d[i], n)) - query(i);
//            cerr << query(min(i + d[i], n)) << ' ' << query(i) << endl;
        }
        for(int i: ls[x]) {
            update(i);
        }
    }
    memset(f, 0, sizeof(f));
    for(int x = n; x >= 1; x--) {
        for(int i: ls[x]) {
            ans += query(i) - query(max(i - d[i], 1) - 1);
        }
        for(int i: ls[x]) {
            update(i);
        }
    }
    cout << ans << endl;
}
