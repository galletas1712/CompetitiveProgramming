// Repeatedly invalidate vertices (ones that don't have both options for A and B adjacent to it).
// Magnificently, we can do this with a queue, which resembles BFS
// We simply have to check if there is still a valid connected component. We don't even need DSU for this.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1;

class TaskC {
	int n, m, cnt[N][2];
	bool removed[N];
	string s;
	vector<int> g[N];

public:
	TaskC() {
		memset(cnt, 0, sizeof(cnt));
		memset(removed, 0, sizeof(removed));
	}
	void solve(istream& cin, ostream& cout) {
		cin >> n >> m >> s;
		for(int i = 0, u, v; i < m; i++) {
			cin >> u >> v;
			--u, --v;
			g[u].push_back(v);
			g[v].push_back(u);
			++cnt[u][s[v] == 'B'];
			++cnt[v][s[u] == 'B'];
		}
		queue<int> q;
		for(int i = 0; i < n; i++) if(!cnt[i][0] || !cnt[i][1]) q.push(i), removed[i] = true;
		while(!q.empty()) {
			int u = q.front();
			q.pop();
			for(int v: g[u]) {
				--cnt[v][s[u] == 'B'];
				if(!removed[v] && (!cnt[v][0] || !cnt[v][1])) {
					q.push(v);
					removed[v] = true;
				}
			}
		}
		int ans = 0;
		for(int i = 0; i < n; i++) ans += !removed[i];
		assert(ans || ans != 1);
		cout << (ans ? "Yes" : "No") << endl;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TaskC solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
