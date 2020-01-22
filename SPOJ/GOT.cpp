// Easy Mo's on Tree or alternatively persistent segment tree. Wasted nearly three hours on a stupid swapping += with -=
#include <bits/stdc++.h>
using namespace std;
const int BLK = 300;

struct query {
	int l, r, c, i;
	query() {}
	query(int l, int r, int c, int i): l(l), r(r), c(c), i(i) {}
	bool operator<(const query& rhs) const {
		return make_pair(l / BLK, r) < make_pair(rhs.l / BLK, rhs.r);
	}
};

int n, q;
vector<vector<int> > g;
vector<int> color, ord, st, ed, depth, cnt, state, ans;
vector<vector<int> > anc;
vector<query> queries;

void init_dfs(int u, int p) {
	ord.push_back(u);
	st[u] = ord.size() - 1;
	for(int v: g[u]) if(v != p) {
		depth[v] = depth[u] + 1;
		anc[v].push_back(u);
		for(int j = 0; 1 << j+1 <= depth[v]; j++)
			anc[v].push_back(anc[anc[v][j]][j]);
		init_dfs(v, u);
	}
	ord.push_back(u);
	ed[u] = ord.size() - 1;
}

int get_lca(int u, int v) {
	if(depth[v] > depth[u]) swap(u, v);
	for(int j = 0; depth[u] != depth[v]; j++) if(depth[u] - depth[v] >> j & 1) u = anc[u][j];
	if(u == v) return u;
	for(int j = anc[u].size()-1; j >= 0; j--) if(j < anc[u].size() && anc[u][j] != anc[v][j])
		u = anc[u][j], v = anc[v][j];
	return anc[u][0];
}

int main() {
	while(scanf("%d %d", &n, &q) != EOF) {
		g = vector<vector<int> >(n+1);
		ord.clear();
		queries.clear();
		color = vector<int>(n+1);
		st = vector<int>(n+1);
		ed = vector<int>(n+1);
		depth = vector<int>(n+1);
		anc = vector<vector<int> >(n+1);

		for(int i = 1; i <= n; i++) scanf("%d", &color[i]);

		for(int i = 1, u, v; i < n; i++) {
			scanf("%d %d", &u, &v);
			g[u].push_back(v);
			g[v].push_back(u);
		}
		init_dfs(1, -1);
		for(int i = 0, u, v, c; i < q; i++) {
			scanf("%d %d %d", &u, &v, &c);
			int lca = get_lca(u, v);
			if(st[u] > st[v]) swap(u, v);
			if(lca == u) queries.emplace_back(st[u], st[v], c, i);
			else {
				queries.emplace_back(ed[u], st[v], c, i);
				queries.emplace_back(st[lca], st[lca], c, i);
				assert(ed[u] <= st[v] && lca != v);
			}
		}
		sort(queries.begin(), queries.end());
		cnt = vector<int>(n+1);
		state = vector<int>(n+1);
		ans = vector<int>(q);
		int l = 0, r = 0;
		state[ord[0]] = 1;
		++cnt[color[ord[0]]];
		for(query qq: queries) {
			while(l < qq.l) {
				state[ord[l]] ^= 1;
				cnt[color[ord[l]]] += (state[ord[l]] ? 1 : -1);
				++l;
			}
			while(r > qq.r) {
				state[ord[r]] ^= 1;
				cnt[color[ord[r]]] += (state[ord[r]] ? 1 : -1);
				--r;
			}
			while(l > qq.l) {
				--l;
				state[ord[l]] ^= 1;
				cnt[color[ord[l]]] += (state[ord[l]] ? 1 : -1);
			}
			while(r < qq.r) {
				++r;
				state[ord[r]] ^= 1;
				cnt[color[ord[r]]] += (state[ord[r]] ? 1 : -1);
			}
			ans[qq.i] |= cnt[qq.c];
		}
		for(int i = 0; i < q; i++) printf("%s\n", (ans[i] ? "Find" : "NotFind"));
		printf("\n");
	}
}
