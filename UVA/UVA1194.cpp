/*
Reduce problem to bipartite minimum vertex cover (ignoring 0 vertex)
By Konig's theorem, this is equivalent to maximum bipartite matching
We run our handy maximum bipartite matching algorithm using geeksforgeeks implementation
This is O(VE) because Ford-Fulkerson runs in O(E * max flow), where max flow is V since the maximum possible bipartite matching is V
V = m + n
E = K
So runs in O(K(m + n)) time, which is very fast!
*/
#include <bits/stdc++.h>
using namespace std;

bool rec(int u, vector<bool> &seen, vector<int> &rev_match, vector<vector<int> > g) {
  for(int v: g[u]) if(!seen[v]) {
    seen[v] = true;
    if(rev_match[v] == -1 || rec(rev_match[v], seen, rev_match, g)) {
      rev_match[v] = u;
      return true;
    }
  }
  return false;
}

void solve(int n, int m, int k) {
  vector<int> a(k), b(k);
  vector<vector<int> > g(n);
  for(int i = 0; i < k; i++) {
    cin >> i >> a[i] >> b[i];
    if(a[i] && b[i]) g[a[i]].push_back(b[i]);
  }
  vector<int> rev_match(m, -1);
  int ans = 0;
  for(int i = 0; i < n; i++) {
    vector<bool> seen(m);
    ans += rec(i, seen, rev_match, g);
  }
  cout << ans << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  while(true) {
    int n, m, k;
    cin >> n;
    if(n == 0) break;
    cin >> m >> k;
    solve(n, m, k);
  }
}
