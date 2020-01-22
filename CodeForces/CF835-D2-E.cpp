//Very interesting involving bitmasks of indices. Idea was to check on which bits the indices differ, which can be done with xor queries as it can be observed that at least one bit will differ, from the requirements in the problem. From there, we can find the indices themselves.
#include <bits/stdc++.h>
using namespace std;

int n, x, y;
vector<int> odd, even;

int query(vector<int> S) {
  if(S.empty()) return 0;
  cout << "? " << S.size() << ' ';
  for(int i: S) cout << i << ' ';
  cout << endl;
  cout.flush();
  int ret;
  cin >> ret;
  if(ret == -1) exit(0);
  return ret;
}

int query_sup(int i) {
    vector<int> S;
    for(int mask = 0; mask <= n; mask++) if((mask & i) == i) S.push_back(mask);
    int ret = query(S);
    return ret == y || ret == (x ^ y);
}

int main() {
  cin >> n >> x >> y;
  for(int i = 0; i < 32 - __builtin_clz(n); i++) {
    if(query_sup(1 << i)) odd.push_back(i);
    else even.push_back(i);
  }
  int p1 = 1 << odd.back(), p2 = 0;
  odd.pop_back();
  for(unsigned int i: odd) {
    if(query_sup(p1 | 1 << i)) p1 |= 1 << i;
    else p2 |= 1 << i;
  }
  for(unsigned int i: even) if(query_sup(p1 | 1 << i)) p1 |= 1 << i, p2 |= 1 << i;
  if(p1 > p2) swap(p1, p2);
  cout << "! " << p1 << ' ' << p2 << endl;
  cout.flush();
}
