#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 2010;
const long INF = 1e7+1;

struct vec {
   long x, y;
   vec() { x = y = INF; }
   vec(long x, long y): x(x), y(y) {}
   long operator^(const vec& rhs) const {
       return x * rhs.y - y * rhs.x;
   }
   vec operator-(const vec& rhs) const {
       return vec(x - rhs.x, y - rhs.y);
   }
   bool operator==(const vec& rhs) const {
       return x == rhs.x && y == rhs.y;
   }
};

int n, ans;
vec pt[N];

vector<int> g[N];
bool adj[N][N];

bool valid[N][N];
int pre[N][N];

void dfs1(int u, int src, int second, vec last) {
    for(int v: g[u]) {
        if(v == pre[src][u]) continue;
        pre[src][v] = u;

        if(last == vec(INF, INF)) {
            valid[src][v] = true;
            second = v;
        } else {
            valid[src][v] = valid[src][u];
            valid[src][v] &= (last ^ (pt[v] - pt[u])) < 0;
            valid[src][v] &= ((pt[v] - pt[pre[src][v]]) ^ (pt[src] - pt[v])) < 0;
            valid[src][v] &= ((pt[src] - pt[v]) ^ (pt[second] - pt[src])) < 0;
//            cerr << src << ' ' << u << ' ' <<v << ' ' << second << ' ' << valid[src][u] << ' ' << valid[src][v] << endl;
        }

        dfs1(v, src, second, pt[v] - pt[u]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for(int i = 0, x, y; i < n; i++) {
        cin >> x >> y;
        pt[i] = vec(x, y);
        valid[i][i] = true;
    }
    for(int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
        adj[u][v] = adj[v][u] = true;
    }
    for(int i = 0; i < n; i++) {
        pre[i][i] = -1;
        dfs1(i, i, -1, vec(INF, INF));
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if(i == j || adj[i][j]) continue;
            ans += valid[i][j];
//            if(valid[i][j]) cerr << i << ' ' << j << endl;
        }
    }
    cout << ans << endl;
}
