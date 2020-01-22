// dp[c][s] = number of ways to get a prefix of c columns with connectivity state s
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class Islands {

    int R, C, ans;
    bool blocked[1001][10], valid[1 << 9][1001];
    int initH[1 << 9];
    unordered_map<int, int> dp[2];

    void add(int &a, int b) {
        a += b;
        if(a >= MOD) a -= MOD;
    }

    bool is_one_component(int h) {
        int config[R + 1];
        decode(h, config);
        int mn = INF, mx = -INF;
        for(int i = 1; i <= R; i++) {
            if(!config[i]) continue;
            mn = min(config[i], mn);
            mx = max(config[i], mx);
        }
        return h && mn == mx; // h cannot be zero, othewise we double count
    }

    int encode(int arr[]) {
        int h = 0;
        for(int i = 1; i <= R; i++) {
            h = h * 10 + arr[i];
        }
        return h;
    }

    void decode(int h, int store[]) {
        for(int i = R; i >= 1; i--) {
            store[i] = h % 10;
            h /= 10;
        }
    }

    int root(int v, int par[]) {
        assert(par[v]);
        if(par[v] == v) return v;
        return par[v] = root(par[v], par);
    }

    void unite(int u, int v, int par[]) {
        assert(u && v);
        u = root(u, par), v = root(v, par);
        if(u > v) swap(u, v);
        par[v] = u;
    }

    vector<int> universe;
    int curr[10], idx = 0;
    void gen_universe(int depth) {
        if(depth == R) {
            for(int i = 1; i <= R; i++) {
                if(i && curr[i] && curr[i-1] && curr[i] != curr[i-1]) return;
            }
            vector<int> mn_pos(R + 1, INF), mx_pos(R + 1, -INF);
            for(int i = 1; i <= R; i++) {
                mn_pos[curr[i]] = min(i, mn_pos[curr[i]]);
                mx_pos[curr[i]] = max(i, mx_pos[curr[i]]);
            }
            for(int i = 1; i <= R; i++) {
                for(int j = 1; j <= R; j++) {
                    if(i == j) continue;
                    if(mn_pos[i] < mn_pos[j]) {
                        if(mn_pos[j] < mx_pos[i] && mx_pos[j] > mx_pos[i]) return;
                    } else {
                        if(mn_pos[i] < mx_pos[j] && mx_pos[i] > mx_pos[j]) return;
                    }
                }
            }
            universe.push_back(encode(curr));
            return;
        }
        for(int i = 0; i <= depth + 1; i++) {
            curr[++idx] = i;
            gen_universe(depth + 1);
            --idx;
        }
    }


public:
    void solve(istream &cin, ostream &cout) {
        cin >> R >> C;
        for(int i = 1; i <= R; i++) {
            string s;
            cin >> s;
            for(int j = 1; j <= C; j++) blocked[j][i] = (s[j-1] == '#' ? 1 : 0);
        }
        // precompute dsu for only that column
        for(int mask = 0; mask < 1 << R; mask++) {
            vector<int> alive;
            for(int i = 0; i < R; i++) {
                if(mask >> i & 1) {
                    alive.push_back(i + 1);
                }
            }
            int scaffold[R + 1];
            fill(scaffold, scaffold + R + 1, 0);
            for(int i = 0; i < alive.size(); i++) {
                if(i && alive[i] == alive[i-1] + 1) {
                    scaffold[alive[i]] = scaffold[alive[i]- 1];
                } else {
                    scaffold[alive[i]] = alive[i];
                }
            }
            initH[mask] = encode(scaffold);
        }
        // precompute transitions
        int pre_config[R + 1], nxt_config[R + 1], lowest_adj[R + 1];
        // precompute validity of mask with respect to the column
        for(int mask = 0; mask < 1 << R; mask++) {
            for(int i = 1; i <= C; i++) {
                valid[mask][i] = true;
                for(int j = 1; j <= R; j++) if(mask >> j-1 & 1) {
                        if(blocked[i][j]) {
                            valid[mask][i] = false;
                        }
                    }
            }
        }
        // fill in first column
        for(int mask = 0; mask < 1 << R; mask++) {
            if(valid[mask][1]) {
                ++dp[1][initH[mask]];
            }
        }
        for(int c = 1; c < C; c++) {
            int p = c & 1;
            dp[p ^ 1].clear();
            for(auto tt: dp[p]) {
                int h, res;
                tie(h, res) = tt;
                if(is_one_component(h)) add(ans, res);
                decode(h, pre_config);
                for(int mask = 0; mask < 1 << R; mask++) {
                    if(!valid[mask][c+1]) continue;
                    decode(initH[mask], nxt_config);
                    fill(lowest_adj, lowest_adj + R + 1, INF);

                    for(int i = R; i >= 1; i--) {
                        if(pre_config[i] && nxt_config[i]) {
                            lowest_adj[pre_config[i]] = i;
                        }
                    }
                    bool can_use_mask = true;
                    for(int i = 1; i <= R; i++) if(pre_config[i] && lowest_adj[pre_config[i]] == INF) {
                            can_use_mask = false;
                            break;
                        }
                    if(!can_use_mask) continue;

                    for(int i = 1; i <= R; i++) {
                        if(pre_config[i] && nxt_config[i]) {
                            unite(i, lowest_adj[pre_config[i]], nxt_config);
                        }
                    }
                    for(int i = 1; i <= R; i++) {
                        if(nxt_config[i]) {
                            nxt_config[i] = root(i, nxt_config);
                        }
                    }
                    int h_new = encode(nxt_config);
                    add(dp[p ^ 1][h_new], res);
                }
            }
        }
        for(auto tt: dp[C & 1]) {
            int h, res;
            tie(h, res) = tt;
            if(is_one_component(h)) add(ans, res);
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Islands *obj = new Islands();
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
