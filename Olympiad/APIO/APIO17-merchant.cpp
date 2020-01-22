// binary search on efficiency
// for every pair of vertices, we buy/sell the good with the maximum efficiency
// when checking, create new adjacency matrix from rearranged inequality with fixed efficiency and check >= 0
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 101;
const long LINF = 1e16;

int n, m, K, buy[N][1001], sell[N][1001];
long dist[N][N], dp[N][N], S[N][N];

bool check(long x) {
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) {
    dp[i][j] = -LINF;
    if(i != j && dist[i][j] != LINF && S[i][j] != -LINF) dp[i][j] = S[i][j] - x * dist[i][j];
  }

  for(int k = 0; k < n; k++) {
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) if(i != k && j != k) {
      dp[i][j] = max(dp[i][k] + dp[k][j], dp[i][j]);
    }
  }
  
  for(int i = 0; i < n; i++) if(dp[i][i] >= 0) return true;
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n >> m >> K;
  for(int i = 0; i < n; i++) {
    for(int k = 0; k < K; k++) cin >> buy[i][k] >> sell[i][k];
  }
  for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) if(i != j) dist[i][j] = LINF;
  for(int i = 0, u, v, w; i < m; i++) {
    cin >> u >> v >> w;
    --u, --v;
    dist[u][v] = min((long) w, dist[u][v]);
  }
  for(int k = 0; k < n; k++) {
    for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) if(i != j) {
      dist[i][j] = min(dist[i][k] + dist[k][j], dist[i][j]);
    }
  }
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      S[i][j] = -LINF;
      if(dist[i][j] == LINF) continue;
      S[i][j] = 0;
      for(int k = 0; k < K; k++) if(sell[j][k] != -1 && buy[i][k] != -1) {
        S[i][j] = max((long) sell[j][k] - buy[i][k], S[i][j]);
      }
    }
  }
  long l = 0, r = 1e9;
  while(l < r) {
    long mid = l+r+1 >> 1;
    if(check(mid)) l = mid;
    else r = mid-1;
  }
  cout << l;
}
