#include <bits/stdc++.h>
using namespace std;
const int N = 5e4+1, RAND_LIM = 95, INF = 1e9+1;

int n, S, T, m, Q, dist[RAND_LIM][N];
vector<pair<int,int> > g[N];
vector<int> ss, tt;
bool iss[N], ist[N];

void dijkstra(int s, int dist[]) {
    fill(dist, dist + n + 1, INF);
    priority_queue<pair<int, int> > pq;
    dist[s] = 0;
    pq.emplace(-dist[s], s);
    while(!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for(auto p: g[u]) {
            int v, w;
            tie(v, w) = p;
            if(dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.emplace(-dist[v], v);
            }
        }
    }
}

int main() {
    srand(1000);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> S >> T >> m >> Q;
    for(int i = 0, u, v, w; i < m; i++) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    for(int i = 0, v; i < S; i++) {
        cin >> v;
        ss.push_back(v);
        iss[v] = true;
    }
    for(int i = 0, v; i < T; i++) {
        cin >> v;
        tt.push_back(v);
        ist[v] = true;
    }
    vector<int> candidates;
    for(int i = 1; i <= n; i++) {
        if(iss[i] || ist[i]) continue;
        candidates.push_back(i);
    }
    random_shuffle(candidates.begin(), candidates.end());
    for(int i = 0; i < min((int) candidates.size(), RAND_LIM); i++) {
        dijkstra(candidates[i], dist[i]);
    }
    for(int i = 0, u, v; i < Q; i++) {
        cin >> u >> v;
        int res = INF;
        for(int w = 0; w < min((int) candidates.size(), RAND_LIM); w++) {
            res = min(dist[w][u] + dist[w][v], res);
        }
        cout << res << '\n';
    }
}
