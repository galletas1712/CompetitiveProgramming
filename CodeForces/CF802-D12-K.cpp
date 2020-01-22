// dp[v][0] = maximum cost of edges of a connected subgraph within the subtree rooted at v, such that we return back to v afterwards
// dp[v][1] = the same thing but we end up at a vertex in the subtree
// Since we visit a node no more than k times, we go to at most k-1 children (coming back), and one child to not come back (if we choose to)
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class TaskK {

	int n, k;
	vector<pair<int,int> > g[N];
	long dp[N][2];

	void dfs(int u, int par, int pw) {
		vector<pair<long, int> > ls0;
		for(auto p: g[u]) {
			int v, w; tie(v, w) = p;
			if(v == par) continue;
			dfs(v, u, w);
			ls0.emplace_back(dp[v][0], v);
		}
		sort(ls0.begin(), ls0.end(), greater<pair<long, int> >());
		for(int i = 0; i < min(k-1, (int) ls0.size()); i++) dp[u][0] += ls0[i].first;
		dp[u][1] = dp[u][0];
		for(int i = 0; i < min(k-1, (int) ls0.size()); i++)
			dp[u][1] = max(dp[u][0] - ls0[i].first + dp[ls0[i].second][1] + (ls0.size() > k-1 ? ls0[k-1].first : 0), dp[u][1]);
		for(int i = k-1; i < ls0.size(); i++)
			dp[u][1] = max(dp[u][0] + dp[ls0[i].second][1], dp[u][1]);

		dp[u][0] += pw, dp[u][1] += pw;
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> k;
		for(int i = 1, u, v, w; i < n; i++) {
			cin >> u >> v >> w;
			g[u].emplace_back(v, w);
			g[v].emplace_back(u, w);
		}
		dfs(0, -1, 0);
		cout << dp[0][1];
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskK *obj = new TaskK();
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
