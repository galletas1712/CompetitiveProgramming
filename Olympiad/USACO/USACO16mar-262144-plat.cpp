// It can be proven that we can always merge from right to left.
// dp[i][k] = rightmost index we need to keep merging from the right to such that we get value k after merge
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 262144 + 10;

class Problem1 {

    int n, ans, a[N], dp[N][100];

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 1; i <= n; i++) cin >> a[i];
        memset(dp, -1, sizeof(dp));
        for(int i = 1; i <= n; i++) {
            dp[i][a[i]] = i-1;
            for(int k = a[i]+1; k < 100; k++) {
                if(dp[i][k-1] >= 0) {
                    dp[i][k] = dp[dp[i][k-1]][k-1];
                }
                if(dp[i][k] != -1) {
                    ans = max(k, ans);
                }
//                cerr << i << ' ' << k << ' ' << dp[i][k] << endl;
            }
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Problem1 *obj = new Problem1();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::ifstream in("262144.in");
    std::ofstream out("262144.out");
    solver.solve(in, out);
    return 0;
}
