#include <bits/stdc++.h>
using namespace std;
const int N = 5e4+1;

int n, m, res[N];
vector<int> g[N];
vector<pair<int,int> > edges;
vector<tuple<int,int,int,int> > queries;

int t[2*N];
void update(int i, int v) {
	i += N;
	for(t[i] = min(v, t[i]); i > 1; i >>= 1) t[i>>1] = min(t[i], t[i^1]); 
}
int query(int l, int r) {
	int res = INT_MAX;
	for(l += N, r += N+1; l < r; l >>= 1, r >>= 1) {
		if(l & 1) res = min(t[l++], res);
		if(r & 1) res = min(t[--r], res);
	}
	return res;
}

int curr_time, par[N], in[N], out[N];
void dfs(int u) {
	in[u] = ++curr_time;
	for(int v: g[u]) if(v != par[u]) {
		par[v] = u;
		dfs(v);
	}
	out[u] = curr_time;
}

int main() {
	freopen("disrupt.in", "r", stdin);
	freopen("disrupt.out", "w", stdout);
	// ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> m;
	for(int i = 0, u, v; i < n-1; i++) {
		cin >> u >> v; --u; --v;
		g[u].push_back(v);
		g[v].push_back(u);
		edges.emplace_back(u, v);
	}
	par[0] = -1;
	dfs(0);
	for(int i = 0, u, v, w; i < m; i++) {
		cin >> u >> v >> w; --u; --v;
		if(in[u] > in[v]) swap(u, v);
		queries.emplace_back(in[u], 1, in[v], w); // only want less l
	}
	for(int i = 0; i < n; i++) queries.emplace_back(in[i], 0, out[i], i);

	sort(queries.begin(), queries.end());
	fill(res, res+n, INT_MAX);
	fill(t, t+2*N, INT_MAX);

	for(auto qq: queries) {
		int l, tp, r, wi;
		tie(l, tp, r, wi) = qq;
		if(tp) update(r, wi);
		else res[wi] = min(query(l, r), res[wi]);
	}

	for(auto& qq: queries) {
		swap(get<0>(qq), get<2>(qq));
		get<1>(qq) ^= 1;
	}
	sort(queries.begin(), queries.end(), greater<tuple<int,int,int,int>>());
	fill(t, t+2*N, INT_MAX);

	for(auto qq: queries) {
		int r, tp, l, wi;
		tie(r, tp, l, wi) = qq;
		if(!tp) update(l, wi);
		else res[wi] = min(query(l, r), res[wi]);
	}

	for(auto e: edges) {
		int u, v; tie(u, v) = e;
		if(in[u] > in[v]) swap(u, v);
		cout << (res[v] == INT_MAX ? -1 : res[v]) << '\n';
	}
}
