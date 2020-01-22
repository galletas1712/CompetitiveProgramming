#include <bits/stdc++.h>
using namespace std;
const int N = 3001, M = 1001, INF = 1e9+1;

int n, m, A, B, cnt_between, f[M][M], dp[M][M];
vector<tuple<int,int,int> > edges;

vector<int> xs;
void compress() {
    sort(xs.begin(), xs.end());
    xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
    for(int i = 0, u, v, w; i < m; i++) {
        tie(u, v, w) = edges[i];
        get<0>(edges[i]) = lower_bound(xs.begin(), xs.end(), u) - xs.begin();
        get<1>(edges[i]) = lower_bound(xs.begin(), xs.end(), v) - xs.begin();
    }
    assert(xs.size() < M);
}

int main() {
    cin >> n >> m >> A >> B;
    if(A > B) swap(A, B);
    for(int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w;
        if(u > v) swap(u, v);
        if(w <= A) {
            cout << -1 << endl;
            return 0;
        }
        edges.emplace_back(u, v, w);
        xs.push_back(u);
        xs.push_back(v);
        cnt_between += w <= B;
    }
    if(cnt_between == 0) {
        cout << 1ll * B * (n - 1) << endl;
        return 0;
    }
    compress();
    for(int i = 0, u, v, w; i < m; i++) {
        tie(u, v, w) = edges[i];
        for(int x = u; x >= 0; x--) {
            for(int y = v; y < xs.size(); y++) {
                ++f[x][y];
            }
        }
    }

    for(int k = 0; k <= m; k++) for(int r = 0; r < xs.size(); r++) {
        dp[k][r] = INF;
    }

    int res = INF;
    dp[0][0] = 0;

    for(int k = 0; k <= m; k++) {
        for(int r = 1; r < xs.size(); r++) {
            dp[k][r] = min(dp[k][r-1], dp[k][r]);
            for(int l = r-1; l >= 0; l--) {
                if(f[l][r] > k) continue;
                dp[k][r] = min(dp[k - f[l][r]][l] + xs[r] - xs[l], dp[k][r]);
            }
            if(k >= cnt_between) res = min(dp[k][r], res);
        }
    }
    cout << 1ll * A * res + 1ll * B * (n - 1 - res) << endl;
}
