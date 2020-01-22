/*
 * Credits to subthread https://codeforces.com/blog/entry/66240?mobile=false#comment-503639 for hints
 * Notice that if we only consider only the cells that have weight < x,
 * the possible valleys are the components formed.
 * If we only stop at a subset of a component, those were already counted when we had a lower x
 * Therefore, we can keep on incrementing x and finding non-holey components at each point
 *
 * To maintain whether or not something is non-holey, we use v-e+f = 2
 * In a non-holey region, f = # of 2x2 blocks fully covered by the region + 1
 * We simply assume that f has this value, and if it doesn't satisfy v-e+f = 2, then it is holey
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 800;

class valleys {

    int n, a[N][N];
    vector<tuple<int,int,int> > ord;
    bool vis[N][N];
    int dsu[N*N], V[N*N], E[N*N], F[N*N];
    long ans;

    bool valid(int i, int j) {
        return 0 <= i && i < n && 0 <= j && j < n;
    }

    int get_vertex(int i, int j) {
        return i * n + j;
    }

    int root(int v) {
        return dsu[v] < 0 ? v : dsu[v] = root(dsu[v]);
    }

    int root(int i, int j) {
        return root(get_vertex(i, j));
    }

    void merge(int u, int v) {
        if((u = root(u)) == (v = root(v))) return;
        dsu[u] += dsu[v];
        dsu[v] = u;
        V[u] += V[v];
        E[u] += E[v];
        F[u] += F[v] - 1;
    }

    void merge(int x1, int y1, int x2, int y2) {
        merge(get_vertex(x1, y1), get_vertex(x2, y2));
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                cin >> a[i][j];
                ord.emplace_back(a[i][j], i, j);
                int v = get_vertex(i, j);
                dsu[v] = -1;
                V[v] = 1;
                E[v] = 0;
                F[v] = 1;
            }
        }
        sort(ord.begin(), ord.end());
        for(auto tt: ord) {
            int i, j;
            tie(ignore, i, j) = tt;
            vis[i][j] = true;
            for(int k = 0; k < 4; k++) {
                if(valid(i + dx[k], j + dy[k]) && vis[i + dx[k]][j + dy[k]]) {
                    merge(i, j, i + dx[k], j + dy[k]);
                    ++E[root(i, j)];
                }
            }
            if(valid(i - 1, j - 1) && root(i, j) == root(i - 1, j - 1) && root(i, j) == root(i - 1, j) && root(i, j) == root(i, j - 1)) {
                ++F[root(i, j)];
            }
            if(valid(i - 1, j + 1) && root(i, j) == root(i - 1, j + 1) && root(i, j) == root(i - 1, j) && root(i, j) == root(i, j + 1)) {
                ++F[root(i, j)];
            }
            if(valid(i + 1, j - 1) && root(i, j) == root(i + 1, j - 1) && root(i, j) == root(i + 1, j) && root(i, j) == root(i, j - 1)) {
                ++F[root(i, j)];
            }
            if(valid(i + 1, j + 1) && root(i, j) == root(i + 1, j + 1) && root(i, j) == root(i + 1, j) && root(i, j) == root(i, j + 1)) {
                ++F[root(i, j)];
            }
            int v = root(i, j);
            if(V[v] - E[v] + F[v] == 2) {
                ans += V[v];
            }
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        valleys *obj = new valleys();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::ifstream in("valleys.in");
    std::ofstream out("valleys.out");
    solver.solve(in, out);
    return 0;
}
