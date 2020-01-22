#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+10;

class Hiking {

    int n, m, L;
    vector<int> g[N];
    int dist[N], trace[N];
    vector<int> s;
    bool on_path[N];

    void bfs() {
        queue<int> q;
        fill(dist, dist+N, INF);
        q.push(1);
        dist[1] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: g[u]) {
                if(dist[u] + 1 < dist[v]) {
                    dist[v] = dist[u] + 1;
                    trace[v] = u;
                    q.push(v);
                }
            }
        }
        for(int v = n; v >= 1; v = trace[v]) on_path[v] = true;
    }

    int pre[N], low[N];
    stack<int> stk;
    void dfs1(int u, int p) {
        static int tick = 0;
        pre[u] = low[u] = ++tick;
        stk.push(u);
        stable_partition(g[u].begin(), g[u].end(), [&] (int i) { return on_path[i]; }); // need to do this or else not using BFS order will get condition low[v] >= pre[trace[v]] mixed up in traceback
        for(int v: g[u]) {
            if(v == p) continue;
            if(!pre[v]) {
                dfs1(v, u);
                low[u] = min(low[v], low[u]);
            } else {
                low[u] = min(pre[v], low[u]);
            }
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m >> L;
        for(int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        bfs();
        dfs1(1, -1);
        for(int v = n; v != 1; v = trace[v]) {
            if(trace[v] != 1 && trace[v] != n) {
                if(low[v] >= pre[trace[v]]) {
                    s.push_back(dist[trace[v]]);
                }
            }
        }
        reverse(s.begin(), s.end());

        int ans = 0, curr = 0;
        while(true) {
            auto it = lower_bound(s.begin(), s.end(), curr);
            if(it == s.end()) break;
            ++ans;
            curr = *it + 2*L + 1;
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Hiking *obj = new Hiking();
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
