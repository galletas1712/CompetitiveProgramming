#include <bits/stdc++.h>
#define long long long
using namespace std;

int n, m;
long ans;
map<int, int> ls[64][64];
void solve(istream& cin) {
    cin >> n >> m;
    for(int i = 0; i < n; i++) {
        string s;
        cin >> s;
        int h = 0, mask1 = 0;
        for(int j = 0; j < m; j++) if(s[j] != '?') mask1 |= 1 << j;
        int tmp[64];
        fill(tmp, tmp+64, -1);
        for(int mask2 = 0; mask2 < 1 << m; mask2++) if((mask2 & mask1) == mask2) {
                int h = 0;
                for(int j = 0, p = 1; j < m; j++, p *= 26) {
                    if(mask2 >> j & 1) {
                        h += p * (s[j] - 'a');
                    }
                }
                tmp[mask2] = h;
            }
        // different mask2
        for(int mask2 = 0; mask2 < 1 << m; mask2++) {
            int and_mask = mask1 & mask2;
            int h = tmp[and_mask];
            if(ls[mask2][and_mask].count(h))
                ans += ls[mask2][and_mask][h];
        }
        for(int mask2 = 0; mask2 < 1 << m; mask2++) if(tmp[mask2] >= 0) ++ls[mask1][mask2][tmp[mask2]];
    }
    cout << ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve(cin);
}
