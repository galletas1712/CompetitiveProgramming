// HLD with a deque of pairs for each heavy path. When we update the deque, we can see that we cancel out the work done by previous queries, reducing the number of elements on the deque
// Therefore, the amortized number of elements on the deque that we iterate is n log n, since we handle each heavy path separately.
// Complexity is n log^2 n amortized
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class Construction {

	int n;
	vector<vector<int> > g;
	vector<pair<int,int> > edges;
	vector<int> C, vals;

	vector<int> sz, nxt, par, depth;
	deque<pair<int,int> > dq[N];

	vector<int> fen;
	vector<long> ans;

	void init_dfs(int u) {
		sz[u] = 1;
		for(int &v: g[u]) {
			par[v] = u;
			depth[v] = depth[u] + 1;
			init_dfs(v);
			sz[u] += sz[v];
			if (sz[v] > sz[g[u][0]]) swap(v, g[u][0]);
		}
	}

	void dec(int u) {
		for(int &v: g[u]) {
			nxt[v] = (v == g[u][0] ? nxt[u] : v);
			dec(v);
		}
	}

	void compress() {
		for(int i = 1; i <= n; i++) vals.push_back(C[i]);
		sort(vals.begin(), vals.end());
		vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
		unordered_map<int, int> mp;
		for(int i = 0; i < vals.size(); i++) mp[vals[i]] = i;
		for(int i = 1; i <= n; i++) C[i] = mp[C[i]];
	}

	void update(int i, int v) {
		++i;
		while(i <= n) fen[i] += v, i += i & -i;
	}

	int query(int i) {
		++i;
		int ret = 0;
		while(i > 0) ret += fen[i], i -= i & -i;
		return ret;
	}

public:
	void solve(istream& cin, ostream& cout) {
		cin >> n;
		g = vector<vector<int> >(n+1);
		C = vector<int>(n+1);
		sz = vector<int>(n+1);
		nxt = vector<int>(n+1);
		par = vector<int>(n+1);
		depth = vector<int>(n+1);
		fen = vector<int>(n+1);

		for(int i = 1; i <= n; i++) cin >> C[i];
		compress();

		for(int i = 1, u, v; i < n; i++) {
			cin >> u >> v;
			g[u].push_back(v);
			edges.emplace_back(u, v);
		}
		nxt[1] = 1;
		par[1] = -1;
		init_dfs(1);
		dec(1);

		dq[1].emplace_front(C[1], 1);
		for(auto e: edges) {
			int a, b;
			tie(a, b) = e;
			vector<pair<int,int> > segs;
			for(int v = a; v != -1; v = par[nxt[v]]) {
				vector<pair<int,int> > loc;
				int sum = 0, targ = depth[v] - depth[nxt[v]] + 1;
				while(!dq[nxt[v]].empty() && sum < targ) {
					if(targ - sum < dq[nxt[v]].front().second) {
						int val, cnt;
						tie(val, cnt) = dq[nxt[v]].front();
						loc.emplace_back(val, targ - sum);
						dq[nxt[v]].pop_front();
						dq[nxt[v]].emplace_front(val, cnt - (targ - sum));
						sum = targ;
					} else {
						sum += dq[nxt[v]].front().second;
						loc.push_back(dq[nxt[v]].front());
						dq[nxt[v]].pop_front();
					}
				}
				reverse(loc.begin(), loc.end());
				segs.insert(segs.end(), loc.begin(), loc.end());
			}
			// compute answer using segs
			long res = 0;
			for(auto p: segs) {
				res += 1ll * query(p.first-1) * p.second;
				update(p.first, p.second);
			}
			for(auto p: segs) {
				update(p.first, -p.second);
			}
			ans.push_back(res);
			// update
			for(int v = b; v != -1; v = par[nxt[v]]) {
				dq[nxt[v]].emplace_front(C[b], depth[v] - depth[nxt[v]] + 1);
			}
		}
		for(long x: ans) cout << x << '\n';
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Construction solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
