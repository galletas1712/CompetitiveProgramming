#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 101;

class Citrus {

    int n, k, S[N], A[N][N], B[N];
    int dp[N][N][N];

    inline void setmin(int &a, int b) {
        a = min(a, b);
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> k;
        for(int i = 0; i < n; i++) {
            cin >> S[i];
            --S[i];
        }
        for(int i = 0; i < k; i++) {
            for(int j = 0; j < k; j++) {
                cin >> A[i][j];
                if(i == j) A[i][j] = 0;
            }
        }
        for(int x = 0; x < k; x++) {
            for(int i = 0; i < k; i++) {
                for(int j = 0; j < k; j++) {
                    A[i][j] = min(A[i][x] + A[x][j], A[i][j]);
                }
            }
        }
        for(int i = 0; i < k; i++) cin >> B[i];

        memset(dp, -1, sizeof(dp));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < k; j++) {
                dp[i][i][j] = A[S[i]][j];
            }
        }
        for(int len = 2; len <= n; len++) {
            for(int l = 0; l < n - len + 1; l++) {
                int r = l + len - 1;
                for(int x = 0; x < k; x++) {
                    dp[l][r][(x + 1) % k] = INF;
                    for(int i = l; i < r; i++) {
//                        cerr << "compute_nxt " << l << ' ' << r << ' ' << x << ' ' << dp[l][i][x] << ' ' << dp[i+1][r][x] << ' ' << B[x] << ' ' << dp[l][i][x] + dp[i+1][r][x] + B[x] << endl;
                        setmin(dp[l][r][(x+1) % k], dp[l][i][x] + dp[i+1][r][x] + B[x]);
                    }
                }
                for(int x = 0; x < k; x++) {
                    for(int y = 0; y < k; y++) {
                        setmin(dp[l][r][y], dp[l][r][x] + A[x][y]);
                    }
                }
//                for(int x = 0; x < k; x++) {
//                    cerr << "Actual " << l << ' ' << r << ' ' << x << ' ' << dp[l][r][x] << endl;
//                }
            }
        }
        int ans = *min_element(dp[0][n-1], dp[0][n-1] + k);
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Citrus *obj = new Citrus();
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
