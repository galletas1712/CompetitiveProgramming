/*
 * For every pair (s, t), store a neighbor of s on the path from s to t
 * Also, make this the smallest index in the adjacency list
 * Denote this as nxt[s][t]
 * When invalidating an edge, just set a tag and do nothing more
 * When querying, we do a DFS, but using nxt[s][t] to guide us. This way, we don't need to iterate through edges and thus removes the |E| factor
 * Since nxt[s][t] always has the minimal index, if the edge(s, g[nxt[s][t]]) is invalid, we just increment nxt[s][t] and recursively check whether the new one is valid
 * Keep on doing this until either a recursively valid one is reached (and thus t is reachable) or we reach the end of the adjacency list (in which we return false)
 * Notice that each nxt[s][t] can only be incremented at most deg[s] times. Therefore, complexity is n * sum of degrees of all vertices = O(mn)
 * Since we only use nxt[s][t] to guide us, the queries take at most O(qn)
 * Therefore we get complexity O(qn + mn)
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 301;

class Unfolllow {

    int n, m, q, cnt_so_far;
    vector<int> g[N];
    bool invalid[N][N];

    bool vis[N];
    int nxt[N][N];
    void dfs1(int u) {
        vis[u] = true;
        for(int i = 0; i < g[u].size(); i++) {
            int v = g[u][i];
            if(!vis[v]) dfs1(v);
            for(int t = 1; t <= n; t++) {
                if(nxt[u][t] == -1 && nxt[v][t] != -1)
                    nxt[u][t] = i;
            }
        }
        nxt[u][u] = INF; // make different from -1, but still should be invalid
    }

    bool dfs2(int u, int t) {
        if(nxt[u][t] == -1) return false;
        if(u == t) return true;
        while(true) {
            if(nxt[u][t] >= g[u].size()) break;
            int v = g[u][nxt[u][t]];
            if(!invalid[u][v]) {
                bool ret = dfs2(v, t);
                if(ret) return true;
            }
            ++nxt[u][t];
        }
        return false;
    }

    bool dfs3(int u, int t) {
        vis[u] = true;
        if(u == t) return true;
        bool ret = false;
        for(int v: g[u]) {
            if(vis[v] || invalid[u][v]) continue;
            ret |= dfs3(v, t);
        }
        return ret;
    }

    bool brute(int s, int t) {
        memset(vis, 0, sizeof(vis));
        return dfs3(s, t);
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m >> q;
        for(int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            swap(u, v);
            g[u].push_back(v);
//            cerr << "g " << u << ' ' << v << endl;
        }
        memset(nxt, -1, sizeof(nxt));
        for(int i = 1; i <= n; i++) if(!vis[i]) dfs1(i);

//        for(int i = 1; i <= n; i++)
//            for(int j = 1; j <= n; j++)
//                cerr << "nxt " << i << ' ' << j << ' ' << nxt[i][j] << endl;

        while(q--) {
            int inst;
            cin >> inst;
            if(inst == 1) {
                int x, y;
                cin >> x >> y;
                swap(x, y);
                invalid[x][y] = true;
            } else {
                int y, w;
                cin >> y >> w;
                int x = 1 + (cnt_so_far + w) % n;
                swap(x, y);
//                cerr << "Q2 " << x << ' ' << y << endl;
                bool ret = dfs2(x, y);
                cnt_so_far += ret;
                cout << (ret ? "y" : "n") << '\n';
            }
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Unfolllow *obj = new Unfolllow();
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
