#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 4e5+1;

class TreeRotations {

	int n, a[N], lc[N], rc[N], f[N];
	long dp[N];

	void update(int i, int v) { while(i <= n) f[i] += v, i += i & -i; }
	int query(int i) {
		int ret = 0;
		while(i > 0) ret += f[i], i -= i & -i;
		return ret;
	}

	int sz[N], st[N], en[N], rev[N];
	void dfs1(int u) {
		if(!a[u]) {
			dfs1(lc[u]);
			dfs1(rc[u]);
			if(sz[rc[u]] > sz[lc[u]]) swap(lc[u], rc[u]);
		}
		sz[u] = sz[lc[u]] + sz[rc[u]] + 1;
	}

	void dfs2(int u) {
		static int tick = 0;
		st[u] = ++tick;
		rev[st[u]] = u;
		if(!a[u]) {
			dfs2(lc[u]);
			dfs2(rc[u]);
		}
		en[u] = tick;
	}

	void dfs3(int u, bool keep) {
		if(a[u]) {
			if(keep) update(a[u], 1);
			return;
		}
		dfs3(rc[u], 0);
		dfs3(lc[u], 1);
		long a0 = 0, a1 = 0;
		for(int i = st[rc[u]]; i <= en[rc[u]]; i++) if(a[rev[i]]) {
			a0 += query(a[rev[i]]);
			a1 += query(n) - query(a[rev[i]]);
		}
		dp[u] = dp[lc[u]] + dp[rc[u]] + min(a0, a1);
		for(int i = st[rc[u]]; i <= en[rc[u]]; i++) if(a[rev[i]]) update(a[rev[i]], 1);
		if(!keep) {
			for(int i = st[u]; i <= en[u]; i++) if(a[rev[i]]) update(a[rev[i]], -1);
		}
	}

	int node;
	int input(istream& cin) {
		int ret = ++node;
		cin >> a[ret];
		if(a[ret] == 0) {
			lc[ret] = input(cin);
			rc[ret] = input(cin);
		}
		return ret;

	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		input(cin);
		dfs1(1);
		dfs2(1);
		dfs3(1, 0);
		cout << dp[1] << endl;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TreeRotations *obj = new TreeRotations();
		obj->solve(in, out);
		delete obj;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Solver solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
