// Do case analysis with sqrt. Look at both the panel size if it is less than sqrt, but quotients if it is more than sqrt, as the number of quotients is always <= sqrt
// We can use the quotient of one dimension to check with the other dimension as we only use right bounds.
#include <bits/stdc++.h>
using namespace std;

int n;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n;
  while(n--) {
    int a1, a2, b1, b2;
    cin >> a1 >> a2 >> b1 >> b2;
    auto can = [&] (int x) -> bool {
      return (a1+x-1) / x <= a2 / x && (b1+x-1) / x <= b2 / x;
    };
    int ans = 1, lim = max(sqrt(a2), sqrt(b2)) + 1;
    for(int x = 1; x <= lim; x++) {
      if(can(x)) ans = max(x, ans);
      if((a1+x-1) / x <= a2 / x && can(a2 / x)) ans = max(a2 / x, ans);
      if((b1+x-1) / x <= b2 / x && can(b2 / x)) ans = max(b2 / x, ans);
    }
    cout << ans << '\n';
  }
}
