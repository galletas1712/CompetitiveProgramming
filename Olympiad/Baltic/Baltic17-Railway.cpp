#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1;

int n, m, k;
vector<pair<int,int> > g[N];
vector<int> ls;

int d[N];
vector<int> anc[N];
void init_dfs(int u, int par) {
	for(auto p: g[u]) {
		int v, i;
		tie(v, i) = p;
		if(v == par) continue;

		d[v] = d[u] + 1;
		anc[v].push_back(u);
		for(int j = 0; 1 << j+1 <= d[v]; j++)
			anc[v].push_back(anc[anc[v][j]][j]);		
		init_dfs(v, u);
	}
}

int get_lca(int u, int v) {
	if(d[u] < d[v]) swap(u, v);
	for(int j = 0; d[u] - d[v]; j++)
		if(d[u] - d[v] >> j & 1) u = anc[u][j];
	if(u == v) return u;
	for(int j = 17; j >= 0; j--)
		if(j < anc[u].size() && anc[u][j] != anc[v][j])
			u = anc[u][j], v = anc[v][j];
	return anc[u][0];
}

void add(int val, int num, map<int, int> &f, int &cnt) {
	if(!f.count(val)) {
		assert(f[val] == 0);
		++cnt;
	}
	f[val] += num;
}

void sub(int val, int num, map<int, int> &f, int &cnt) {
	assert(f.count(val));
	f[val] -= num;
	assert(f[val] >= 0);
	if(!f[val]) {
		--cnt;
		f.erase(val);
	}
}

vector<pair<int,int> > qq[N];
map<int, int> f[N];
int cnt[N];
vector<int> ans;
void dfs(int u, int par) {
	for(auto p: g[u]) {
		int v, i;
		tie(v, i) = p;
		if(v == par) continue;
		dfs(v, u);
		if(cnt[v] >= k) ans.push_back(i);
		if(f[v].size() > f[u].size()) {
			swap(cnt[u], cnt[v]);
			swap(f[u], f[v]);
		}
		for(auto pp: f[v]) {
			add(pp.first, pp.second, f[u], cnt[u]);
			assert(pp.second > 0);
		}
	}
	for(auto p: qq[u]) {
		if(p.second >= 1) add(p.first, p.second, f[u], cnt[u]);
		else {
			sub(p.first, -p.second, f[u], cnt[u]);
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> m >> k;
	for(int i = 1, u, v; i < n; i++) {
		cin >> u >> v;
		g[u].emplace_back(v, i);
		g[v].emplace_back(u, i);
	}
	init_dfs(1, -1);
	for(int i = 0; i < m; i++) {
		int sz;
		vector<int> req;	
		cin >> sz;
		req.resize(sz);
		for(int j = 0; j < sz; j++) {
			cin >> req[j];
			qq[req[j]].emplace_back(i, 1);
		}
		int lca = req[0];
		for(int j = 1; j < sz; j++) {
			lca = get_lca(req[j], lca);
		}
		qq[lca].emplace_back(i, -sz);
	}
	dfs(1, -1);
	sort(ans.begin(), ans.end());
	cout << ans.size() << endl;
	for(int i: ans) cout << i << ' ';
	cout << endl;
}
