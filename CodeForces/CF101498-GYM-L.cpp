//Interesting/Important problem. Got hint that keeping dp[u] is enough. Had to look at another person's code to find bug in Bellman-ford. Turns out that in this version of Bellman-Ford, the condition for negative cycles is a bit different, which cost me a lot of time. Probably better to attack this from an SPFA angle (I don't know it at all). Also, there was a bug with long long, which the problem shouldn't require. Not sure if the test data is wrong or if the problem actually requires long long.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const long INF = 1e14;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2001;

class TaskL {
	int n, m;
	vector<int> cnt;
	vector<long> dp;
	vector<tuple<int,int,long> > edges;
public:
	void solve(istream& cin, ostream& cout) {
		int test;
		cin >> test;
		while(test--) {
			cin >> n >> m;

			dp = vector<long>(n+1, 0);
			cnt = vector<int>(n+1, 0);
			edges.clear();

			long ans = INF;
			for(int i = 0, u, v, w; i < m; i++) {
				cin >> u >> v >> w;
				edges.emplace_back(u, v, w);
				ans = min((long) w, ans);
			}
			if(ans >= 0) {
				cout << ans << '\n';
				continue;
			}

			while(ans != -INF && true) {
				bool term = false;
				for(auto e: edges) {
					int u, v, w;
					tie(u, v, w) = e;
					if(dp[u] + w < dp[v]) {
						dp[v] = dp[u] + w;
						++cnt[v];
						term = true;
					}
					if(cnt[v] > n+1 || dp[v] < -2e9) {
						ans = -INF;
						term = false;
						break;
					}
				}
				if(!term) break;
			}

			for(int i = 1; i <= n; i++) ans = min(dp[i], ans);

			if(ans == -INF) cout << "-inf\n";
			else cout << ans << '\n';
		}
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TaskL solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
