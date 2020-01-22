#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class Slicice {

    int n, m, k;
    vector<int> B, P;
    vector<int> dp[2];

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m >> k;
        P = vector<int>(n);
        B = vector<int>(m + 1);
        for(int i = 0; i < n; i++) cin >> P[i];
        for(int i = 0; i <= m; i++) cin >> B[i];
        int pre = 0, now = 1;
        dp[0] = vector<int>(k + 1, -INF);
        dp[1] = vector<int>(k + 1, -INF);
        dp[0][0] = 0;
        for(int i = 0; i < n; i++, swap(pre, now)) {
            dp[now].assign(k + 1, -INF);
            for(int x = 0; x <= k; x++) {
                for(int y = 0; y <= x; y++) {
                    if(P[i] + x - y > m) continue;
                    int tmp = dp[pre][y] + B[P[i] + x - y];
                    dp[now][x] = max(tmp, dp[now][x]);
                }
                if(dp[now][x] < 0) dp[now][x] = -INF;
            }
        }
        cout << dp[pre][k] << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Slicice *obj = new Slicice();
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
