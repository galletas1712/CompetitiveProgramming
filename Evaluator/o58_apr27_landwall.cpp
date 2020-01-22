#include <bits/stdc++.h>
#define long int64_t
using namespace std;
const int N = 1001, INF = 1e9+1;

int n, k, ans, a[N], dp[N][N], mx[N];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> k;
    for(int i = 1; i <= n; i++) cin >> a[i];

    for(int i = 0; i <= n; i++) for(int j = 0; j <= k; j++) dp[i][j] = -INF;
    for(int j = 0; j <= k; j++) mx[j] = -INF;

    mx[0] = 0;
    dp[1][0] = a[1];
    ans = max(a[1], ans);

    for(int i = 2; i <= n; i++) {
        dp[i][0] = mx[0] + a[i];
        for(int j = 1; j <= k; j++) {
            dp[i][j] = max(dp[i-1][j-1], mx[j]) + a[i];
        }
        for(int j = 0; j <= k; j++) {
            if(dp[i][j] < 0) dp[i][j] = -INF;
            ans = max(dp[i][j], ans);
//            cerr << i << ' ' << j << ' ' << dp[i][j] << endl;
        }
        for(int j = 0; j <= k; j++) {
            mx[j] = max(dp[i-1][j], mx[j]);
        }
    }
    cout << ans << endl;
}
