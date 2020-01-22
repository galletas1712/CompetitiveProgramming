// Main observation is that when we go down the tree, we can cut out one whole subtree.
// This is because the maximum potential when a leaf in the subtree is cut, where the only components are that inside the subtree are considered,
// is bounded by the subtree sum. So, for one of the subtrees, we know the potential of all leaves within the subtree.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class TaskD {

	int h, q;
	map<int, long> f;

	long query(int v, long mx) {
		int lg = 32 - __builtin_clz(v);
		if(31 - __builtin_clz(v) == h) return max(mx, f[v]);
		long ret = 0;
		if(f[v << 1] >= f[v << 1 | 1]) {
			ret += (1 << h-lg) * max(mx, f[v] - f[v << 1 | 1]);
			ret += query(v << 1, max(mx, f[v] - f[v << 1]));
		} else {
			ret += (1 << h-lg) * max(mx, f[v] - f[v << 1]);
			ret += query(v << 1 | 1, max(mx, f[v] - f[v << 1 | 1]));
		}
		return ret;
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> h >> q;
		while(q--) {
			string inst;
			cin >> inst;
			if(inst == "add") {
				int v, e;
				cin >> v >> e;
				while(v >= 1) f[v] += e, v /= 2;
			} else {
				cout << 1.0 * query(1, 0) / (1 << h)  << '\n';
			}
		}
	}

};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskD *obj = new TaskD();
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
