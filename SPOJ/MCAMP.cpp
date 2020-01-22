#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class MCAMP {
	int n;
	vector<long> x, es, gs;
public:
	void solve(istream& cin, ostream& cout) {
		cin >> n;
		x = vector<long>(n+1);
		es = vector<long>(n+1);
		gs = vector<long>(n+1);
		vector<tuple<long,long,long> > ord;
		for(int i = 1; i <= n; i++) {
			cin >> x[i] >> gs[i] >> es[i];
			es[i] += es[i-1];
			gs[i] += gs[i-1];
			ord.emplace_back(-(x[i] - es[i-1]), i, 0);
			ord.emplace_back(-(x[i] - es[i]), i, 1);
		}
		sort(ord.begin(), ord.end());
		int j = INF;
		long ans = 0;
		for(auto tt: ord) {
			int i, type;
			tie(ignore, i, type) = tt;
			if(type == 0) j = min(i, j);
			else if(j <= i) ans = max(gs[i] - gs[j-1], ans);
		}
		cout << ans;
	}
};



int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	MCAMP solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
