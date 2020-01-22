/*
 * Cost of a subproject is (sz - 1) * (sum of distances to and from headquarters)
 * Claim: groups are made of contiguous segments in the *sorted* array of distances
 * Proof: Let c_x be the size of group x - 1. Let c_1 be to the left of c_2
 * Assume that there exists some x in group 1 and y in group 2 such that it is more optimal to swap their order
 * This means that c_1(y - x) + c2(x - y) < 0, which is equivalent to c_1 < c_2
 * However, this is never optimal, as we can move elements from group 2 to group 1 and make it more optimal
 * So, our initial assumption of the existence of x, y is false.
 * Therefore, group must form a contiguous segment in sorted distance array
 *
 * This gives rise to contiguous segment form of DP
 * dp[i][j] = min(dp[i-1][k] + (j-k-1) * (sum[j] - sum[k]))),
 * where i is the number of groups so far and j is the position in sorted distance array.
 * Intuitively, opt[i][j-1] <= opt[i][j] and opt[i][j] <= opt[i+1][j] (as we have the opportunity to create a new group)
 * So, we can use Knuth optimization!
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
#define pii pair<int,int>
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 5e3+10;

class LIVEARCHIVE7577 {
    int n, B, S, m, fd[N], bd[N], d[N];
    vector<pii> g[N], gt[N];

    long sum[N], dp[N][N], opt[N][N];

    void dijkstra(vector<pii> g[N], int d[]) {
        priority_queue<pii, vector<pii>, greater<pii> > pq;
        fill(d, d + N, INF);
        d[B + 1] = 0;
        pq.emplace(0, B + 1);
        while(!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            for(auto p: g[u]) {
                int v, w;
                tie(v, w) = p;
                if(d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    pq.emplace(d[v], v);
                }
            }
        }
    }
public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> B >> S >> m;
        for(int i = 0, u, v, w; i < m; i++) {
            cin >> u >> v >> w;
            g[u].emplace_back(v, w);
            gt[v].emplace_back(u, w);
        }
        dijkstra(g, fd);
        dijkstra(gt, bd);
        for(int i = 1; i <= B; i++) d[i] = fd[i] + bd[i];
        sort(d + 1, d + B + 1);
        for(int i = 1; i <= B; i++) sum[i] = sum[i-1] + d[i];
        for(int i = 0; i <= B; i++) for(int j = 0; j <= B; j++) dp[i][j] = LINF;
        for(int i = 0; i <= B; i++) {
            dp[i][i] = 0;
            opt[i][i] = i-1;
        }
        for(int delta = 1; delta < B; delta++) { // B >= S, so we can just use B
            for(int i = 1; i <= B - delta; i++) {
                int j = i + delta;
                for(int k = opt[i][j-1]; k <= opt[i+1][j]; k++) {
                    long res = dp[i-1][k] + 1ll * (j-k-1) * (sum[j] - sum[k]);
                    if(res < dp[i][j]) {
                        dp[i][j] = res;
                        opt[i][j] = k;
                    }
                }
            }
        }
        cout << dp[S][B];
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        LIVEARCHIVE7577 *obj = new LIVEARCHIVE7577();
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
