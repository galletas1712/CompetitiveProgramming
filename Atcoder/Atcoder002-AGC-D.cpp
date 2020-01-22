// parallel binary search with DSU
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class TaskD {
	int n, m, q, x[N], y[N], z[N], l[N], r[N], dsu[N];
	vector<pair<int,int> > edges;
	vector<int> qq[N];

	int root(int v) { return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v])); }

	void merge(int u, int v) {
		if((u = root(u)) == (v = root(v))) return;
		dsu[u] += dsu[v];
		dsu[v] = u;
	}

public:
	void run(istream &cin, ostream &cout) {
		fill_n(dsu+1, n, -1);
		cin >> n >> m;
		for(int i = 0, u, v; i < m; i++) {
			cin >> u >> v;
			edges.emplace_back(u, v);
		}
		cin >> q;
		for(int i = 0; i < q; i++) {
			cin >> x[i] >> y[i] >> z[i];
			l[i] = 0, r[i] = m-1;
		}
		for(int t = 0; t < 20; t++) {
			fill_n(dsu+1, n, -1);
			for(int i = 0; i < m; i++) qq[i].clear();
			for(int i = 0; i < q; i++) if(l[i] != r[i]) {
				qq[l[i] + r[i] >> 1].push_back(i);
			}
			for(int i = 0; i < m; i++) {
				merge(edges[i].first, edges[i].second);
				for(int qi: qq[i]) {
					int rtx = root(x[qi]), rty = root(y[qi]);
					int cnt = (rtx == rty ? -dsu[rtx] : -dsu[rtx] -dsu[rty]);
					if(cnt >= z[qi]) r[qi] = i;
					else l[qi] = i+1;
				}
			}
		}
		for(int i = 0; i < q; i++) cout << l[i]+1 << '\n';
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskD *obj = new TaskD();
		obj->run(in, out);
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
