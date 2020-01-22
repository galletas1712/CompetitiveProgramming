#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class gathering {

	int n, m;
	vector<int> g[N], ls[N], rev_ls[N];
	bool res[N];

	int mark[N];
	bool has_cycle = false;
	void find_cycle(int u) {
		mark[u] = 1;
		for(int v: ls[u]) {
			if(!mark[v]) {
				find_cycle(v);
			} else if(mark[v] == 1) {
				has_cycle = true;
			}
		}
		mark[u] = 2;
	}

	int d[N], st[N], en[N], up_edges;
	vector<int> anc[N];

	int get_lca(int u, int v) {
		if(d[u] < d[v]) swap(u, v);
		for(int j = 17; j >= 0; j--) if(d[u] - d[v] >> j & 1) u = anc[u][j];
		if(u == v) return u;
		for(int j = 17; j >= 0; j--) if(j < anc[u].size() && anc[u][j] != anc[v][j])
				u = anc[u][j], v = anc[v][j];
		return anc[u][0];
	}

	void init_dfs(int u, int p) {
		static int tick = 0;
		st[u] = ++tick;
		for(int v: g[u]) if(v != p) {
				d[v] = d[u] + 1;
				anc[v].push_back(u);
				for(int j = 0; 1 << j+1 <= d[v]; j++)
					anc[v].push_back(anc[anc[v][j]][j]);
				init_dfs(v, u);
			}
		en[u] = tick;
	}

	void count_ups(int u, int p) {
		for(int v: g[u]) if(v != p) {
				count_ups(v, u);
			}
		for(int v: ls[u]) {
			int lca = get_lca(u, v);
			up_edges += lca == v;
		}
	}

	void dfs2(int u, int p) {
		if(!up_edges) res[u] = 1;
//		assert(up_edges >= 0);
		for(int v: g[u]) if(v != p) {
				int org = up_edges;
				for(int w: rev_ls[u]) {
					up_edges -= st[v] <= st[w] && st[w] <= en[v];
				}
				for(int w: rev_ls[v]) {
					up_edges += !(st[v] <= st[w] && st[w] <= en[v]);
				}
				dfs2(v, u);
				up_edges = org;
			}
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> m;
		for(int i = 1, u, v; i < n; i++) {
			cin >> u >> v;
			--u, --v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		for(int i = 0, u, v; i < m; i++) {
			cin >> u >> v;
			--u, --v;
			ls[v].push_back(u);
			rev_ls[u].push_back(v);
		}
		find_cycle(0);
		if(has_cycle) {
			for(int i = 0; i < n; i++) cout << 0 << '\n';
			return;
		}
		init_dfs(0, -1);
		count_ups(0, -1);
		dfs2(0, -1);
		for(int i = 0; i < n; i++) cout << res[i] << '\n';
	}
};


class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    gathering *obj = new gathering();
		obj->solve(in, out);
		delete obj;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Solver solver;
	std::ifstream in("gathering.in");
	std::ofstream out("gathering.out");
	solver.solve(in, out);
	return 0;
}
