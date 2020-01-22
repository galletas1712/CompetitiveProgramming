#include <bits/stdc++.h>
using namespace std;
const int N = 5010;

int f[2*N][2*N];
void update(int x, int y) {
    assert(x >= 0 && y >= 0);
    ++x, ++y;
    for(int i = x; i < 2*N; i += i & -i)
        for(int j = y; j < 2*N; j += j & -j)
            ++f[i][j];
}
int query(int x, int y) {
    ++x, ++y;
    int ret = 0;
    for(int i = x; i > 0; i -= i & -i)
        for(int j = y; j > 0; j -= j & -j)
            ret += f[i][j];
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for(int i = 0, x, h; i < n; i++) {
        cin >> x >> h;
        cout << query(2*N-2, N+x-h) - query(x+h-1, N+x-h) << '\n';
        update(x + h, N + x - h);
    }
}
