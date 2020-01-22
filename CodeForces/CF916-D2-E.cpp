// When we reroot, we can simply maintain which node has the root without changing the aggregates based on the euler tour in any way
// Notice that subtree sum after rerooting only changes for nodes that are parents of the new root in the original tree. For those cases, we just subtract sum aggregate of that child of the subtree root from the whole tree.
// To find lca, we use distances in the original tree to find LCA in the rerooted tree
// d(root, u) + d(root, v) - 2*d(root, lca) = d(u, v)
// if LHS > RHS too shallow
// if LHS < RHS too deep
// rearrange equation and binary search for lca
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 17;

class TaskE {
	int n, q, root, a[N];
	vector<int> g[N], anc[N];

	int d[N], st[N], ed[N], rev[N];
	void init_dfs(int u, int p) {
		static int tick = 0;
		rev[tick] = u;
		st[u] = tick++;
		for(int v: g[u]) if(v != p) {
			d[v] = d[u] + 1;
			anc[v].push_back(u);
			for(int j = 0; 1 << j+1 <= d[v]; j++)
				anc[v].push_back(anc[anc[v][j]][j]);
			init_dfs(v, u);
		}
		ed[u] = tick-1;
	}

	int get_lca(int u, int v) {
		if(d[u] < d[v]) swap(u, v);
		for(int j = 17; j >= 0; j--) if(d[u] - d[v] >> j & 1) u = anc[u][j];
		if(u == v) return u;
		for(int j = 17; j >= 0; j--) if(j < anc[u].size() && anc[u][j] != anc[v][j])
			u = anc[u][j], v = anc[v][j];
		return anc[u][0];
	}
	int get_par(int u, int k) {
		assert(k >= 0);
		for(int j = anc[u].size() - 1; j >= 0; j--) if(j < anc[u].size() && (k >> j & 1)) u = anc[u][j];
		return u;
	}
	int get_vertex_with_dist(int u, int v, int k) {
		int lca = get_lca(u, v);
		if(k <= d[u] - d[lca]) return get_par(u, k);
		else return get_par(v, d[v] - (k - d[u] + 2*d[lca]));
	}

	long t[2*N], lz[2*N];
	void build(int i = 1, int l = 0, int r = N-1) {
		if(l == r) {
			t[i] = a[rev[l]];
			return;
		}
		int mid = l+r >> 1;
		build(i << 1, l, mid);
		build(i << 1 | 1, mid+1, r);
		t[i] = t[i << 1] + t[i << 1 | 1];
	}
	void apply(int i, int l, int r) {
		t[i] += lz[i] * (r - l + 1);
		if(l != r) {
			lz[i << 1] += lz[i];
			lz[i << 1 | 1] += lz[i];
		}
		lz[i] = 0;
	}
	void update(int tl, int tr, int v, int i = 1, int l = 0, int r = N-1) {
		apply(i, l, r);
		if(r < tl || l > tr) return;
		if(tl <= l && r <= tr) {
			lz[i] += v;
			apply(i, l, r);
			return;
		}
		int mid = l+r >> 1;
		update(tl, tr, v, i << 1, l, mid);
		update(tl, tr, v, i << 1 | 1, mid+1, r);
		t[i] = t[i << 1] + t[i << 1 | 1];
	}
	long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
		apply(i, l, r);
		if(r < tl || l > tr) return 0;
		if(tl <= l && r <= tr) return t[i];
		int mid = l+r >> 1;
		return query(tl, tr, i << 1, l, mid) + query(tl, tr, i << 1 | 1, mid+1, r);
	}

public:
	TaskE() {
		memset(a, 0, sizeof(a));
		memset(d, 0, sizeof(d));
		memset(t, 0, sizeof(t));
		memset(lz, 0, sizeof(lz));
	}
	void solve(istream& cin, ostream& cout) {
		cin >> n >> q;
		root = 1;
		for(int i = 1; i <= n; i++) cin >> a[i];
		for(int i = 1, u, v; i < n; i++) {
			cin >> u >> v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		init_dfs(1, -1);
		build();
		while(q--) {
			int inst, u, v, x;
			cin >> inst;
			if(inst == 1) {
				cin >> v;
				root = v;
			} else if(inst == 2) {
				cin >> u >> v >> x;
				int duv = d[u] + d[v] - 2*d[get_lca(u, v)];
				int dru = d[root] + d[u] - 2*d[get_lca(root, u)];
				int drv = d[root] + d[v] - 2*d[get_lca(root, v)];
				if(dru < drv) swap(u, v), swap(dru, drv);
				int targ = (dru + drv - duv) / 2;
				int l = 0, r = dru;
				while(l < r) {
					int mid = l+r >> 1;
					int tmp_lca = get_vertex_with_dist(root, u, mid);
					if(d[root] + d[tmp_lca] - 2*d[get_lca(root, tmp_lca)] >= targ) r = mid;
					else l = mid+1;
				}
				int subtree_root = get_vertex_with_dist(root, u, l);
				if(get_lca(root, subtree_root) == subtree_root && root != 1) {
					update(st[1], ed[1], x);
					if(root != subtree_root) {
						int child = get_par(root, d[root] - d[subtree_root] - 1);
						update(st[child], ed[child], -x);
					}
				} else {
					update(st[subtree_root], ed[subtree_root], x);
				}
			} else {
				cin >> v;
				long res;
				if(get_lca(root, v) == v && root != 1) {
					res = query(st[1], ed[1]);
					if(root != v) {
						int child = get_par(root, d[root] - d[v] - 1);
						res -= query(st[child], ed[child]);
					}
				}
				else res = query(st[v], ed[v]);
				cout << res << '\n';
			}
		}
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TaskE solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
