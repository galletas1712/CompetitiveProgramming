/*
 * # triangles in a graph is bounded by O(m sqrt m), even case of complete graph
 * Notice that every node is visited just once, because edge weights are constant in BFS
 * Once w is visited, it is taken out of the set so it is no longer considered. Only nodes that are left are those that have not been visited
 * The only time where we will visit a node but not remove it is when we encounter a triangle, which only occurs m sqrt m times, as each triangle is only encountered once
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class cen {

    int n, m, k, A, B, d1[N], d2[N];
    vector<int> g[N], active[N];
    unordered_set<int> st[N];

    void bfs1() {
        fill(d1, d1+n+1, INF);
        queue<int> q;
        d1[k] = 0;
        q.push(k);
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: g[u]) {
                if(d1[u] + 1 < d1[v]) {
                    d1[v] = d1[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    void bfs2() {
        fill(d2, d2+n+1, INF);
        for(int i = 0; i < n; i++) active[i] = g[i];
        queue<int> q;
        d2[k] = 0;
        q.push(k);
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: g[u]) {
                vector<int> tmp;
                for(int w: active[v]) {
                    if(d2[u] + 1 >= d2[w]) continue;
                    tmp.push_back(w);
                    if(st[u].find(w) != st[u].end()) continue;
                    d2[w] = d2[u] + 1;
                    q.push(w);
                }
                active[v] = tmp;
            }
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> m >> k >> A >> B;
        --k;
        for(int i = 0, u, v; i < m; i++) {
            cin >> u >> v;
            --u, --v;
            g[u].push_back(v);
            g[v].push_back(u);
            st[u].insert(v);
            st[v].insert(u);
        }
        bfs1();
        bfs2();
        for(int i = 0; i < n; i++) {
            int res = d1[i] * A;
            res = min(d1[i] / 2 * B + (d1[i] % 2 ? A : 0), res);
            if(d2[i] != INF) res = min(d2[i] * B, res);
            cout << res << '\n';
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        cen *obj = new cen();
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
