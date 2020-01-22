#include <bits/stdc++.h>
#define long long long
using namespace std;
const int MOD = 1e9+7;

struct matrix {
  vector<vector<long> > a;
  matrix() { a = vector<vector<long> >(6, vector<long>(6)); }
  matrix operator*(matrix rhs) {
    matrix ret = matrix();
    for(int i = 0; i < 6; i++) for(int j = 0; j < 6; j++) for(int k = 0; k < 6; k++) {
      ret.a[i][j] += a[i][k] * rhs.a[k][j] % MOD;
      ret.a[i][j] %= MOD;
    }
    return ret;
  }
} bases[31];

struct vec {
  vector<long> a;
  vec() { a = vector<long>(6); }
  vec operator*(matrix rhs) {
    vec ret = vec();
    for(int i = 0; i < 6; i++) for(int j = 0; j < 6; j++) {
      ret.a[i] += rhs.a[i][j] * a[j] % MOD;
      ret.a[i] %= MOD;
    }
    return ret;
  }
};

int main() {
  int n;
  cin >> n;
  matrix A = matrix();
  A.a[0] = {2, 1, 0, -1, -1, 1};
  A.a[1] = {1, 2, 1, -1, -1, -1};
  A.a[2] = {0, 1, 2, 1, -1, -1};
  for(int i = 0; i < 3; i++) A.a[i+3][i] = 1;

  bases[0] = A;
  for(int j = 1; j <= 30; j++) bases[j] = bases[j-1] * bases[j-1];

  vec ans = vec();
  for(int i = 0; i < 3; i++) ans.a[i] = 1;
  for(int j = 30; j >= 0; j--) if(n >> j & 1) {
    ans = ans * bases[j];
  }
  cout << ((ans.a[0] + ans.a[1] + ans.a[2] - ans.a[3] - ans.a[4] - ans.a[5]) % MOD + MOD) % MOD;
}
