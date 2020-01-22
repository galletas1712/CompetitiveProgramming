/*
 * dp[x][y][z] = answer with dimensions (x, y) and we want to cut in direction of z (0 means cutting horizontally, 1 means cutting vertically)
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 7919, LINF = 1e18 + 1e16;
const int INF = 1e9+7;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class PrettyCut {

    int W, L, K, dp[400][400][2];
    bool mark[400][400][2];

    int rec(int x, int y, int z) {
        assert(x >= 1 && y >= 1 && (z == 0 || z == 1));
        if(mark[x][y][z]) return dp[x][y][z];
        mark[x][y][z] = true;
        if(x == 1 && y == 1) return dp[x][y][z] = 1;
        else if(x == 1) {
            if(!z) return dp[x][y][z] = 0;
//            for(int i = 0; i <= K; i++) {
//                for(int j = 0; j <= K && i + j <= y - 2; j++) {
//                    ++dp[x][y][z];
//                    dp[x][y][z] %= MOD;
//                }
//            }
            for(int sum = 0; sum <= y - 2; sum++) { // loop above reduces to this
                if(2*K < sum) continue;
                dp[x][y][z] = (dp[x][y][z] + min(2*K - sum, sum) + 1) % MOD;
            }
            if(K >= y) dp[x][y][z] = (dp[x][y][z] + 1) % MOD; // case: cut at every cell (y - 2 ignores this)
        } else if(y == 1) {
            if(z) return dp[x][y][z] = 0;
//            for(int i = 0; i <= K; i++) {
//                for(int j = 0; j <= K && i + j <= x - 2; j++) {
//                    ++dp[x][y][z];
//                    dp[x][y][z] %= MOD;
//                }
//            }
            for(int sum = 0; sum <= x - 2; sum++) { // loop above reduces to this
                if(2*K < sum) continue;
                dp[x][y][z] = (dp[x][y][z] + min(2*K - sum, sum) + 1) % MOD;
            }
            if(K >= x) dp[x][y][z] = (dp[x][y][z] + 1) % MOD; // case: cut at every cell (x - 2 ignores this)
        } else {
            if(!z) {
//                for(int i = 0; i <= K; i++) {
//                    for(int j = 0; j <= K && i + j <= x - 1; j++) {
//                        if(i + j == 0) continue;
//                        dp[x][y][z] = (dp[x][y][z] + rec(x - i - j, y, z ^ 1)) % MOD;
//                    }
//                }
                for(int sum = 1; sum <= x - 1; sum++) { // loop above reduces to this
                    if(2*K < sum) continue;
                    dp[x][y][z] = (dp[x][y][z] + rec(x - sum, y, z ^ 1) * (min(2*K - sum, sum) + 1)) % MOD;
                }
                if(2*K >= x - 1) { // if we actually reached sum == x-1, we need to handle case where we don't cut anything in the last cell remaining (double count)
                    for(int i = 0; i <= min(K, x - 1); i++) {
                        int j = x - 1 - i;
                        if(j > K) continue;
                        dp[x][y][z] += MOD - 1;
                        dp[x][y][z] %= MOD;
                    }
                }
                if(K >= x) dp[x][y][z] = (dp[x][y][z] + 1) % MOD; // if statement above discards case where we chop everything into strips (no more recursion). If we allow this, we need to add it back in.
            }

            if(z) {
//                for(int i = 0; i <= K; i++) {
//                    for(int j = 0; j <= K && i + j <= y - 1; j++) {
//                        if(i + j == 0) continue;
//                        dp[x][y][z] = (dp[x][y][z] + rec(x, y - i - j, z ^ 1)) % MOD;
//                    }
//                }

                for(int sum = 1; sum <= y - 1; sum++) { // loop above reduces to this
                    if(2*K < sum) continue;
                    dp[x][y][z] = (dp[x][y][z] + rec(x, y - sum, z ^ 1) * (min(2*K - sum, sum) + 1)) % MOD;
                }

                if(2*K >= y - 1) { // same idea as above
                    for(int i = 0; i <= min(K, y - 1); i++) {
                        int j = y - 1 - i;
                        if(j > K) continue;
                        dp[x][y][z] += MOD - 1;
                        dp[x][y][z] %= MOD;
                    }
                }
                if(K >= y) dp[x][y][z] = (dp[x][y][z] + 1) % MOD; // same ideas as above
            }
        }

        return dp[x][y][z];
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> W >> L >> K;
        if(W == 1 && L == 1) {
            cout << rec(W, L, 0) << endl;
        } else {
            cout << (rec(W, L, 0) + rec(W, L, 1)) % MOD << endl;
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        PrettyCut *obj = new PrettyCut();
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
