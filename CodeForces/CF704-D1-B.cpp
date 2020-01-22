/*
dp[i][p] = minimum cost with prefix i and p disjoint paths
Don't count paths containing s and e
If s > e, we can swap them and swap a[i], c[i] and b[i], d[i]
Four cases to consider:
- Create new path
- Append to start of path
- Append to end of path
- Merge paths
Final answer is not dp[n][0] even though it might look like so at first
This is because we never actually merge the path containing s and the path containing e
*/
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 5010;
const long LINF = 1e17;

inline void setmin(long &a, long b) {
  a = min(a, b);
}

int n, s, e;
long x[N], a[N], b[N], c[N], d[N];
long dp[N][N];
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n >> s >> e;
  for(int i = 1; i <= n; i++) cin >> x[i];
  for(int i = 1; i <= n; i++) cin >> a[i], a[i] += x[i];
  for(int i = 1; i <= n; i++) cin >> b[i], b[i] -= x[i];
  for(int i = 1; i <= n; i++) cin >> c[i], c[i] += x[i];
  for(int i = 1; i <= n; i++) cin >> d[i], d[i] -= x[i];
  for(int i = 0; i <= n; i++) for(int j = 0; j <= n; j++) dp[i][j] = LINF;
  if(s > e) {
    for(int i = 1; i <= n; i++) swap(a[i], c[i]), swap(b[i], d[i]);
    swap(s, e);
  }
  dp[0][0] = 0;
  for(int i = 0; i < n; i++) {
    for(int p = 0; p <= n; p++) {
      if(i+1 == s) {
        setmin(dp[i+1][p], dp[i][p] + d[i+1]); // new
        if(p > 0) setmin(dp[i+1][p-1], dp[i][p] + c[i+1]); // attach
      } else if(i+1 == e) {
        setmin(dp[i+1][p], dp[i][p] + b[i+1]); // new
        if(p > 0) setmin(dp[i+1][p-1], dp[i][p] + a[i+1]); // attach
      } else {
        setmin(dp[i+1][p+1], dp[i][p] + b[i+1] + d[i+1]); // new
        if(i+1 > e || p > 0) setmin(dp[i+1][p], dp[i][p] + c[i+1] + b[i+1]); // append to start
        if(i+1 > s || p > 0) setmin(dp[i+1][p], dp[i][p] + a[i+1] + d[i+1]); // append to end
        if(p + (i+1 > s) >= 2) setmin(dp[i+1][p-1], dp[i][p] + a[i+1] + c[i+1]); // merge
      }
    }
  }
  cout << dp[n-1][0] + a[n] + (e < n ? c[n] : 0);
}
