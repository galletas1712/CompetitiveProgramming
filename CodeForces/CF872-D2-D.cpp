// Notice that we can find all values of b[0] ^ p[i], p[0] ^ b[i] in 2*n queries
// We also know b[0] ^ p[0], so we can iterate all values of p[0] (or b[0] if you want) and check if the resulting permutations are valid
// The criteria are that p[b[i]] == b[p[i]] == i, all values p[i], b[i] are unique and < n
#include <bits/stdc++.h>
using namespace std;

int n, p[5001], b[5001];

int query(int i, int j) {
  cout << "? " << i << ' ' << j << endl;
  int ret;
  cin >> ret;
  return ret;
}

int cnt[5001];
bool eval() {
  for(int i = 0; i < n; i++) if(!(b[i] < n && p[i] < n && b[p[i]] == i && p[b[i]] == i)) return false;
  fill(cnt, cnt+n, 0);
  for(int i = 0; i < n; i++) ++cnt[p[i]];
  for(int i = 0; i < n; i++) if(cnt[i] > 1) return false;
  fill(cnt, cnt+n, 0);
  for(int i = 0; i < n; i++) ++cnt[b[i]];
  for(int i = 0; i < n; i++) if(cnt[i] > 1) return false;
  return true;
}

int main() {
  cin >> n;
  for(int i = 0; i < n; i++) {
    p[i] = query(i, 0);
    b[i] = query(0, i);
  }
  for(int i = 0; i < n; i++) p[i] ^= b[0];
  int tot = 0;
  vector<int> res;
  while(p[0] < n) {
    if(eval()) {
      ++tot;
      if(tot == 1) {
        for(int i = 0; i < n; i++) res.push_back(p[i]);
      }
    }
    int org_b0 = b[0];
    ++p[0];
    for(int i = 0; i < n; i++) b[i] ^= (p[0] - 1) ^ p[0];
    for(int i = 1; i < n; i++) p[i] ^= org_b0 ^ b[0];
  }
  cout << "!\n" << tot << endl;
  for(int x: res) cout << x << ' ';
  cout << endl;
}
