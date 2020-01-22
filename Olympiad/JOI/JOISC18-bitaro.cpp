/*
 * if Y[j] >= MAGIC, then we will run a naive DP, since the number of times this case occurs is few
 * Otherwise, we notice that if we consider the kth furthest vertices, the solution, if exists, will be contained there by PHP
 * Therefore, we can preprocess kth furthest vertices in O(K(N+M)) time with a merge sort
 * Optimal K in this problem should be around sqrt(N)
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int MAGIC = 320, N = 1e5+1;

class Bitaro {
	int n, m, q;
	vector<int> adj[N];
	pair<int,int> dp[N][MAGIC], dp2[N];
	bool blocked[N], mark[N];

	void naive() {
		fill(dp2, dp2+n+1, make_pair(-INF, -1));
		for(int u = 1; u <= n; u++) {
			if(!blocked[u]) dp2[u] = {0, u};
			for(int v: adj[u]) if(dp2[v].first + 1 > dp2[u].first) {
				dp2[u] = {dp2[v].first + 1, dp2[v].second};
			}
		}
	}

	void preprocess() {
		for(int i = 1; i <= n; i++) for(int j = 0; j < MAGIC; j++) dp[i][j] = {-INF, -1};
		for(int u = 1; u <= n; u++) {
			dp[u][0] = {0, u};
			for(int v: adj[u]) {
				pair<int,int> tmp[2*MAGIC], tmp2[MAGIC];
				int i = 0, j = 0, idx = 0;
				while(idx < 2*MAGIC) {
					if(j == MAGIC || dp[u][i].first > dp[v][j].first + 1) tmp[idx++] = dp[u][i++];
					else {
						tmp[idx++] = make_pair(dp[v][j].first + 1, dp[v][j].second);
						++j;
					}
				}
				idx = 0;
				for(int i = 0; i < 2*MAGIC; i++) {
					if(tmp[i].second != -1 && mark[tmp[i].second]) continue;
					if(tmp[i].second != -1) mark[tmp[i].second] = true;
					if(idx < MAGIC) tmp2[idx++] = tmp[i];
				}
				for(int i = 0; i < 2*MAGIC; i++) mark[tmp[i].second] = false;
				copy(tmp2, tmp2+MAGIC, dp[u]);
			}
		}
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> m >> q;
		for(int i = 0, s, e; i < m; i++) {
			cin >> s >> e;
			adj[e].push_back(s);
		}
		preprocess();
		for(int i = 0, T, Y; i < q; i++) {
			cin >> T >> Y;
			vector<int> ls(Y);
			for(int j = 0, v; j < Y; j++) {
				cin >> ls[j];
				blocked[ls[j]] = true;
			}
			if(Y >= MAGIC) {
				naive();
				cout << (dp2[T].first < 0 ? -1 : dp2[T].first) << '\n';
			} else {
				bool found = false;
				for(int i = 0; i < MAGIC; i++) if(dp[T][i].second != -1 && !blocked[dp[T][i].second]) {
					cout << dp[T][i].first << '\n';
					found = true;
					break;
				}
				if(!found) cout << -1 << '\n';
			}
			for(int v: ls) blocked[v] = false;
		}
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    Bitaro *obj = new Bitaro();
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
