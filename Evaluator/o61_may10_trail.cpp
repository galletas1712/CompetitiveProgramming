#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 210;

struct edge {
    int u, v, flow, cap;
    edge(int u, int v, int flow, int cap): u(u), v(v), flow(flow), cap(cap) {}
};

class Trail {

    int n, m, p, q, ans, D[N][N], task[N][N];

    int S, T, d[N], ptr[N];
    vector<edge> edges;
    vector<vector<int> > g;

    void add_edge(int u, int v, int cap) {
        edges.emplace_back(u, v, 0, cap);
        g[u].push_back(edges.size() - 1);
        edges.emplace_back(v, u, 0, 0);
        g[v].push_back(edges.size() - 1);
    }

    bool bfs(int src) {
        queue<int> q;
        fill(d, d + N, INF);
        q.push(src);
        d[src] = 0;
        while(!q.empty() && d[T] == INF) {
            int u = q.front();
            q.pop();
            for(int eidx: g[u]) {
                int v = edges[eidx].v;
                if(d[v] == INF && edges[eidx].flow < edges[eidx].cap) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[T] != INF;
    }

    int dfs(int u, int flow) {
        if(!flow) return 0;
        if(u == T) return flow;
        for(; ptr[u] < g[u].size(); ++ptr[u]) {
            int eidx = g[u][ptr[u]], v = edges[eidx].v;
            if(d[v] != d[u] + 1) continue;
            int routed = dfs(v, min(flow, edges[eidx].cap - edges[eidx].flow));
            if(routed) {
                edges[eidx].flow += routed;
                edges[eidx ^ 1].flow -= routed;
                return routed;
            }
        }
        return 0;
    }


    bool check(int iter, int mid) {
        edges.clear();
        g = vector<vector<int> >(N);
        S = N - 1;
        T = N - 2;

        for(int i = 0; i < p; i++) add_edge(S, i, 1);
        for(int i = 0; i < p; i++) add_edge(i + p, T, 1);
        for(int i = 0; i < p; i++) {
            for(int j = 0; j < p; j++) {
                if(D[task[iter-1][i]][task[iter][j]] <= mid) {
                    add_edge(i, j + p, INF);
                }
            }
        }
        int flow = 0;
        while(bfs(S)) {
            memset(ptr, 0, sizeof(ptr));
            while(int routed = dfs(S, INF)) {
                flow += routed;
            }
        }
        return flow == p;
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m >> p >> q;
        for(int i = 0; i < q; i++) {
            for(int j = 0; j < p; j++) {
                cin >> task[i][j];
            }
        }
        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++) if(i != j) D[i][j] = INF;
        for(int i = 0, u, v, w; i < m; i++) {
            cin >> u >> v >> w;
            D[u][v] = min(w, D[u][v]);
            D[v][u] = min(w, D[v][u]);
        }
        for(int k = 0; k < n; k++)
            for(int i = 0; i < n; i++)
                for(int j = 0; j < n; j++)
                    D[i][j] = min(D[i][k] + D[k][j], D[i][j]);
        for(int i = 1; i < q; i++) {
            int l = 0, r = INF;
            while(l < r) {
                int mid = l+r >> 1;
                if(check(i, mid)) r = mid;
                else l = mid + 1;
            }
            ans += l;
        }
        cout << ans << endl;
    }
};


class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Trail *obj = new Trail();
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
