#include <bits/stdc++.h>
using namespace std;
const int N = 50;

class Solver {

  long double mat[N][N], E[N], comb[N][N];

public:
  double solve() {
    int c, n;
    cin >> c >> n;
    comb[0][0] = 1;
    for(int i = 1; i <= c; i++) {
      comb[i][0] = comb[i][i] = 1;
      for(int j = 1; j < i; j++) comb[i][j] = comb[i-1][j-1] + comb[i-1][j];
    }
    for(int x = 0; x < c; x++) {
      for(int y = x; y <= min(x + n, c); y++) {
        mat[x][y] = comb[c-x][y-x] / comb[c][n] * comb[x][n-(y-x)];
      }
    }
    for(int i = c-1; i >= 0; i--) {
      E[i] = -1;
      for(int j = i+1; j <= c; j++) E[i] -= E[j] * mat[i][j];
      E[i] /= (mat[i][i] - 1);
    }
    return E[0];
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  for(int i = 1; i <= t; i++) {
    Solver *solver = new Solver();
    cout << "Case #" << i << ": " << solver->solve() << endl;
  }
}
