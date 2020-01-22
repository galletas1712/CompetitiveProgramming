#include <bits/stdc++.h>
using namespace std;
const int N = 5010, INF = 1e9+1;

int dp[2][2*N][3][2][2];
// 1st parameter: prefix considered so far
// 2nd parameter: number of adjacent cat/dog pairs that have not been compensated for by placing a lion in the middle
// 3rd parameter: last animal type in configuration so far
// 4th parameter: whether or not we have kept a block of cats
// 5th parameter: whether or not we have kept a block of dogs

void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  vector<pair<int,int> > b;
  int last = 0;
  int tot_cat_cnt = 0, tot_dog_cnt = 0, tot_lion_cnt = 0;

  for(int i = 0; i < n; i++) {
    cin >> a[i];
    tot_cat_cnt += a[i] == 0;
    tot_dog_cnt += a[i] == 1;
    tot_lion_cnt += a[i] == 2;
    if(i > 0 && (a[i] != a[i-1] || a[i-1] == 2)) {
      b.emplace_back(i - last, a[i-1]);
      last = i;
    }
  }
  b.emplace_back(n - last, a[n-1]);

  if(tot_cat_cnt && tot_dog_cnt && !tot_lion_cnt) {
    cout << -1 << endl;
    return;
  }

  if(tot_cat_cnt == 0 || tot_dog_cnt == 0) {
    cout << 0 << endl;
    return;
  }

  for(int j = 0; j <= 2*n+2; j++)
    for(int k = 0; k < 3; k++)
      for(int c = 0; c < 2; c++)
        for(int d = 0; d < 2; d++)
          dp[0][j][k][c][d] = INF;

  if(b[0].second == 0)
    dp[0][n+1][b[0].second][1][0] = 0;
  else
    dp[0][n+1][b[0].second][0][1] = 0;

  int sum = 0;
  int cat_cnt = 0, dog_cnt = 0, lion_cnt = 0;
  for(int i = 1; i < (int) b.size(); i++) {
    sum += b[i-1].first;
    cat_cnt += b[i-1].second == 0;
    dog_cnt += b[i-1].second == 1;
    lion_cnt += b[i-1].second == 2;

    int now = i & 1, pre = !now;
    for(int j = 0; j <= 2*n+2; j++)
      for(int k = 0; k < 3; k++)
        for(int c = 0; c < 2; c++)
          for(int d = 0; d < 2; d++)
            dp[now][j][k][c][d] = INF;

    // remove everthing before block i
    if(b[i].second == 0)
      dp[now][n+1 - lion_cnt][b[i].second][1][0] = sum;
    else
      dp[now][n+1 - lion_cnt][b[i].second][0][1] = sum;

    for(int j = 1; j <= 2*n+1; j++) { // j = number of adj CDs, to be not yet compensated for with L
      if(b[i].second == 2) {
        // mark for removal
        for(int k = 0; k < 3; k++) {
          for(int c = 0; c < 2; c++) {
            for(int d = 0; d < 2; d++) {
              dp[now][j][k][c][d] = min(min(dp[pre][j+1][k][c][d], dp[pre][j+1][k][c][d]) + b[i].first, dp[now][j][k][c][d]);
            }
          }
        }
        // don't mark for removal
        for(int c = 0; c < 2; c++) for(int d = 0; d < 2; d++) {
          dp[now][j][2][c][d] = min(min(min(dp[pre][j][0][c][d], dp[pre][j][1][c][d]), dp[pre][j][2][c][d]), dp[now][j][2][c][d]);
        }
        continue;
      }
      // Assumes b[i].second == 0 or b[i].second == 1
      // Keep block in place
      if(b[i].second == 0) {
        for(int k = 0; k < 3; k++) {
          for(int d = 0; d < 2; d++) {
            if(k == b[i].second || k == 2) {
              dp[now][j][b[i].second][1][d] = min(min(dp[pre][j][k][0][d], dp[pre][j][k][1][d]), dp[now][j][b[i].second][1][d]);
            } else {
              dp[now][j][b[i].second][1][d] = min(min(dp[pre][j-1][k][0][d], dp[pre][j-1][k][1][d]), dp[now][j][b[i].second][1][d]);
            }
          }
        }
      } else {
        for(int k = 0; k < 3; k++) {
          for(int c = 0; c < 2; c++) {
            if(k == b[i].second || k == 2) {
              dp[now][j][b[i].second][c][1] = min(min(dp[pre][j][k][c][0], dp[pre][j][k][c][1]), dp[now][j][b[i].second][c][1]);
            } else {
              dp[now][j][b[i].second][c][1] = min(min(dp[pre][j-1][k][c][0], dp[pre][j-1][k][c][1]), dp[now][j][b[i].second][c][1]);
            }
          }
        }
      }
      // Mark for removal
      for(int k = 0; k < 3; k++) {
        for(int c = 0; c < 2; c++) {
          for(int d = 0; d < 2; d++) {
            dp[now][j][k][c][d] = min(dp[pre][j][k][c][d] + b[i].first, dp[now][j][k][c][d]);
          }
        }
      }
    }
  }
  int ans = INF;
  for(int j = 1; j <= n+1; j++) for(int k = 0; k < 3; k++) {
    ans = min(dp[(b.size() - 1) & 1][j][k][1][1], ans); // at least one block kept for each C, D
  }

  //handle corner case where we can move all C or all D
  bool found = false;
  for(int i = 1; i < b.size(); i++) {
    if(b[i].second == b[i-1].second && b[i].second == 2) {
      found = true;
    }
  }
  if(b[0].second == 2 || b.back().second == 2) found = true;
  ans = min(min(tot_cat_cnt, tot_dog_cnt) + !found, ans);

  cout << ans << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while(t--) solve();
}
