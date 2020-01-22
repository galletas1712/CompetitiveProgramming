#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 1 << 18;
const long LINF = 1e16;

int n, m;
long ans;
vector<pair<int,int> > lsl[N], lsr[N];
long t[2*N], lz[2*N], dp[N], isect[N];

long f[N];
void update_fen(int i, long v) {
  while(i < N) f[i] += v, i += i & -i;
}

long query_fen(int i) {
  long ret = 0;
  while(i > 0) ret += f[i], i -= i & -i;
  return ret;
}

void apply(int i) {
  if(!lz[i]) return;
  t[i] += lz[i];
  if(i < N) {
    lz[i << 1] += lz[i];
    lz[i << 1 | 1] += lz[i];
  }
  lz[i] = 0;
}

void update(int tl, int tr, long v, int i = 1, int l = 0, int r = N-1) {
  apply(i);
  if(r < tl || l > tr) return;
  if(tl <= l && r <= tr) {
    lz[i] += v;
    apply(i);
    return;
  }
  int mid = l+r >> 1;
  update(tl, tr, v, i << 1, l, mid);
  update(tl, tr, v, i << 1 | 1, mid+1, r);
  t[i] = max(t[i << 1], t[i << 1 | 1]);
}

long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
  apply(i);
  if(r < tl || l > tr) return -LINF;
  if(tl <= l && r <= tr) {
    return t[i];
  }
  int mid = l+r >> 1;
  return max(query(tl, tr, i << 1, l, mid), query(tl, tr, i << 1 | 1, mid+1, r));
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n >> m;
  for(int i = 0, l, r, a; i < m; i++) {
    cin >> l >> r >> a;
    lsr[r].emplace_back(l, a);
    lsl[l].emplace_back(r, a);
    update_fen(l, a);
    update_fen(r+1, -a);
  }
  for(int i = 1; i <= n; i++) {
    isect[i] = query_fen(i);
    dp[i] = query(0, i-1) + isect[i];
    ans = max(dp[i], ans);
    update(i, i, dp[i]);
    for(auto p: lsl[i]) {
      int r, a;
      tie(r, a) = p;
      update(i, r, -a);
    }
    for(auto p: lsr[i]) {
      int l, a;
      tie(l, a) = p;
      update(l, i, a);
    }
  }
  cout << ans;
}
