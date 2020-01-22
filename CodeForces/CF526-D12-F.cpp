/*
Do divide and conquer (usual partitioning into two roughly equally sized segments)
Consider suffix minimums and maximums from mid down to l and prefix mins/maxs from mid+1 up to r
Split into four cases of combinations of mins and maxs to use (use left one or right one)
For each index > mid, we count the number of possible left bounds by keeping a running sum and processing queries offline
*/
#include <bits/stdc++.h>
#define long long long
#define pii pair<int,int>
using namespace std;
const int N = 3e5+10;

int n, a[N], mnl[N], mxl[N], mnr[N], mxr[N];
vector<tuple<int,int,int> > qq[N];
long ans;

int sum[4][4*N];

pair<int,int> get_intersection(int l1, int r1, int l2, int r2) {
  return make_pair(max(l1, l2), min(r1, r2));
}

void solve(int l, int r) {
  if(l == r) {
    ++ans;
    return;
  }
  int mid = (l+r) >> 1;

  auto valid = [&] (int i) -> bool { return l <= i && i <= r; };

  fill(mnl+l, mnl+mid+1, N);
  fill(mnr+mid+1, mnr+r+1, -1);
  fill(mxl+l, mxl+mid+1, N);
  fill(mxr+mid+1, mxr+r+1, -1);

  mnl[mid] = mxl[mid] = a[mid];
  mnr[mid+1] = mxr[mid+1] = a[mid+1];
  for(int i = mid-1; i >= l; i--) mnl[i] = min(mnl[i+1], a[i]), mxl[i] = max(mxl[i+1], a[i]);
  for(int i = mid+2; i <= r; i++) mnr[i] = min(mnr[i-1], a[i]), mxr[i] = max(mxr[i-1], a[i]);

  for(int i = mid+1; i <= r; i++) {
    // jmin and jmax are not denoting that the indices are minimum, but simply correspond to a min/max parameter
    int s = l, e = mid;
    while(s < e) {
      int m = s+e+1 >> 1;
      if(mnl[m] < mnr[i]) s = m;
      else e = m - 1;
    }
    int jmin = (mnl[s] < mnr[i] ? s : l-1) + 1;
    s = l, e = mid;
    while(s < e) {
      int m = s+e+1 >> 1;
      if(mxl[m] > mxr[i]) s = m;
      else e = m - 1;
    }
    int jmax = (mxl[s] > mxr[i] ? s : l-1) + 1;

    // Case 1: mxl < mxr and mnl < mnr
    pii seg1 = get_intersection(jmax, mid, l, jmin-1);
    if(seg1.first <= seg1.second && valid(seg1.second)) {
      if(valid(seg1.first-1)) qq[seg1.first-1].emplace_back(i - mxr[i], -1, 0);
      qq[seg1.second].emplace_back(i - mxr[i], 1, 0);
    }
    // Case 2: mxl > mxr and mnl < mnr
    pii seg2 = get_intersection(l, jmax-1, l, jmin-1);
    if(seg2.first <= seg2.second && valid(seg2.second)) {
      if(valid(seg2.first-1)) qq[seg2.first-1].emplace_back(i, -1, 1);
      qq[seg2.second].emplace_back(i, 1, 1);
    }
    // Case 3: mxl < mxr and mnl > mnr
    pii seg3 = get_intersection(jmax, mid, jmin, mid);
    if(seg3.first <= seg3.second && valid(seg3.second)) {
      if(valid(seg3.first-1)) qq[seg3.first-1].emplace_back(i - mxr[i] + mnr[i], -1, 2);
      qq[seg3.second].emplace_back(i - mxr[i] + mnr[i], 1, 2);
    }
    // Case 4: mxl > mxr and mnl < mnr
    pii seg4 = get_intersection(l, jmax-1, jmin, mid);
    if(seg4.first <= seg4.second && valid(seg4.second)) {
      if(valid(seg4.first-1)) qq[seg4.first-1].emplace_back(i + mnr[i], -1, 3);
      qq[seg4.second].emplace_back(i + mnr[i], 1, 3);
    }
  }

  for(int i = l; i <= mid; i++) {
    ++sum[0][2*N + i - mnl[i]];
    ++sum[1][2*N + mxl[i] - mnl[i] + i];
    ++sum[2][2*N + i];
    ++sum[3][2*N + mxl[i] + i];
    for(auto tt: qq[i]) {
      int targ, coeff, tp;
      tie(targ, coeff, tp) = tt;
      ans += coeff * sum[tp][2*N + targ];
    }
  }

  for(int i = l; i <= mid; i++) {
    qq[i].clear();
    --sum[0][2*N + i - mnl[i]];
    --sum[1][2*N + mxl[i] - mnl[i] + i];
    --sum[2][2*N + i];
    --sum[3][2*N + mxl[i] + i];
  }

  solve(l, mid);
  solve(mid+1, r);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n;
  for(int i = 1, x, y; i <= n; i++) {
    cin >> x >> y;
    a[x] = y;
  }
  solve(1, n);
  cout << ans;
}
