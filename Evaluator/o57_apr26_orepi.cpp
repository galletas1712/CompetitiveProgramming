/*
Divide and conquer optimization
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 40100;

class Orepi {
    int n, m;
    long a[N], s[N], dp[110][N];

    void rec(int i, int l, int r, int optL, int optR) {
        if(l > r) return;
        int mid = l + r >> 1;
        int opt = optL;
        for(int j = optL; j <= min(optR, mid-1); j++) {
            if(dp[i-1][j] + (s[mid] - s[j]) * (mid - j) < dp[i][mid]) {
                dp[i][mid] = dp[i-1][j] + (s[mid] - s[j]) * (mid - j);
                opt = j;
            }
        }
        rec(i, l, mid-1, optL, opt);
        rec(i, mid+1, r, opt, optR);
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m;
        ++m;
        for(int i = 1; i <= n; i++) {
            cin >> a[i];
            s[i] = s[i-1] + a[i];
        }
        if(m >= n) {
            cout << s[n] << endl;
            return;
        }
        for(int i = 0; i <= m; i++) for(int j = 0; j <= n; j++)
            dp[i][j] = LINF;
        dp[0][0] = 0;
        for(int i = 1; i <= m; i++) {
            rec(i, 1, n, 0, n-1);
        }
        cout << dp[m][n] << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Orepi *obj = new Orepi();
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
