/*
Notice that what we're doing here is kind of like an LIS
So use the same algorithm as LIS
This gives at most n*c queries
To reduce to n*c/2, construct the sequence from both the front and the back
*/
#include <bits/stdc++.h>
using namespace std;

int n, m, c, cnt_f, cnt_b
vector<int> f, b;
int main() {
  cin >> n >> m >> c;
  while(m--) {
    int x;
    cin >> x;
    if(x <= c/2) {
      auto it = upper_bound(f.begin(), f.end(), x);
      cout << it - f.begin() + 1 << endl;
      if(it == f.end()) f.push_back(x), ++cnt_f;
      else *it = x;
    } else {
      auto it = upper_bound(b.begin(), b.end(), -x);
      cout << n - (it - b.begin()) << endl;
      if(it == b.end()) b.push_back(-x), ++cnt_b;
      else *it = -x;
    }
    if(cnt_f + cnt_b == n) return 0;
  }
}
