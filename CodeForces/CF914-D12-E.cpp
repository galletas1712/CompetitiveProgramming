// Compress parity of characters into a bitmask. Since characters only run up to t, we can use an array rather than a map, which reduces complexity to n log n * sigma
// There are some subtleties within the counting functions which need to be handled carefully.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1;

class TaskE {

	int n, mask[N];
	char s[N];
	vector<int> g[N];
	long res[N];

	int sz[N];
	bool blocked[N];
	void find_centroid(int u, int p, int tot, pair<int,int> &ret) {
		int mx = 0;
		sz[u] = 1;
		for(int v: g[u]) if(v != p && !blocked[v]) {
			find_centroid(v, u, tot, ret);
			sz[u] += sz[v];
			mx = max(sz[v], mx);
		}
		mx = max(tot - sz[u], mx);
		ret = min(make_pair(mx, u), ret);
	}

	int cen;
	long paths[N], cnt[1 << 21];

	void dfs(int u, int p, int curr, int mode) {
		for(int v: g[u]) if(v != p && !blocked[v]) {
			dfs(v, u, curr ^ mask[v], mode);
			if(mode == 3) {
				paths[u] += paths[v];
				paths[v] = 0; // important as this also functions as a temporary subtree sum
			}
		}
		if(mode == 0) {
			paths[u] += cnt[curr]; // query
			for(int p = 1; p <= 1 << 20; p <<= 1) paths[u] += cnt[curr ^ p];
		}
		else if(mode == 1) {
			++cnt[curr]; // update
		}
		else if(mode == 2) cnt[curr] = 0; // clear cnt only. clearing temp paths done in mode 3
		else if(mode == 3) res[u] += paths[u]; // collect
		else if(mode == 4) res[cen] += paths[u]; // collect for centroid only
	}

	void dec(int src, int tot) {
		pair<int, int> tmp = {tot, -1};
		find_centroid(src, -1, tot, tmp);
		cen = tmp.second;

		// prefix
		cnt[0] = 1; // for vertical paths to centroid
		for(int v: g[cen]) if(!blocked[v]) {
			dfs(v, cen, mask[cen] ^ mask[v], 0);
			dfs(v, cen, mask[v], 1); // not counting centroid twice
		}
		dfs(cen, -1, 0, 4);
		++res[cen];

		// suffix
		reverse(g[cen].begin(), g[cen].end());
		dfs(cen, -1, 0, 2);
		cnt[0] = 0; // don't double count vertical paths to centroid
		for(int v: g[cen]) if(!blocked[v]) {
			dfs(v, cen, mask[cen] ^ mask[v], 0);
			dfs(v, cen, mask[v], 1);
		}
		for(int v: g[cen]) if(!blocked[v]) dfs(v, cen, 0, 3), paths[v] = 0; // hanging value needs to be removed
		dfs(cen, -1, 0, 2);

		blocked[cen] = true;
		for(int v: g[cen]) if(!blocked[v]) {
			dec(v, (sz[v] > sz[cen] ? tot - sz[cen] : sz[v]));
		}
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		for(int i = 1, u, v; i < n; i++) {
			cin >> u >> v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		cin >> s+1;
		for(int i = 1; i <= n; i++) mask[i] = 1 << int(s[i] - 'a');
		dec(1, n);
		for(int i = 1; i <= n; i++) cout << res[i] << ' ';
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskE *obj = new TaskE();
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
