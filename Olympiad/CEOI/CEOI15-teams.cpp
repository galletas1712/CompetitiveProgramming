/*
 * dp[j] = # of possible sequences with length i and maximum = j
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e6+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 10001;

class Calvinball {
    int n, a[N], mx[N];
    long dp[N];
public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1; i <= n; i++) {
            cin >> a[i];
            mx[i] = max(mx[i-1], a[i]);
        }
        for(int i = 1; i <= n; i++) {
            for(int j = i; j >= 1; j--) {
                dp[j] = (dp[j - 1] + 1ll * j * dp[j]) % MOD;
            }
            dp[mx[i-1]] += a[i] - 1; // need to mx[i-1] because we don't want to increase the maximum
        }
        long ans = 1;
        for(int i = 1; i <= n; i++) {
            ans = (ans + dp[i]) % MOD;
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Calvinball *obj = new Calvinball();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::istream& in(std::cin);
    std::ostream& out(std::cout);
    solver.solve(in, out);
    return 0;
}
