// Segment tree and model the problem somewhat like matrix multplication
#include <bits/stdc++.h>
using namespace std;
const int N = 5e4+1, INF = 1e9+1;

int K, n, m, O;
vector<vector<int> > I, adj[N], t[4*N];

vector<vector<int> > merge(vector<vector<int> > &a, vector<vector<int> > &b) {
    vector<vector<int> > res(K, vector<int>(K, INF));
    for(int x = 0; x < K; x++) for(int y = 0; y < K; y++) for(int z = 0; z < K; z++) {
        res[x][y] = min(a[x][z] + b[z][y], res[x][y]);
    }
    return res;
}

void build() {
    for(int i = 0; i < n; i++) t[n+i] = adj[i];
    for(int i = n-1; i >= 1; i--) {
        t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }
}

vector<vector<int> > query(int l, int r) {
    vector<vector<int> > resl = I, resr = I;
    for(l += n, r += n+1; l < r; l >>= 1, r >>= 1){
        if(l & 1) resl = merge(resl, t[l++]);
        if(r & 1) resr = merge(t[--r], resr);
    }
    return merge(resl, resr);
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> K >> n >> m >> O;
    for(int i = 0; i < n; i++) adj[i] = vector<vector<int> >(K, vector<int>(K, INF));
    I = vector<vector<int> >(K, vector<int>(K, INF));
    for(int i = 0; i < K; i++) I[i][i] = 0;
    for(int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w;
        adj[u / K][u % K][v % K] = w;
    }
    build();
    for(int i = 0, a, b; i < O; i++) {
        cin >> a >> b;
        vector<vector<int> > res = query(a / K, b / K - 1);
        if(a / K == b / K || res[a % K][b % K] == INF) cout << -1 << '\n';
        else cout << res[a % K][b % K] << '\n';
    }
}
