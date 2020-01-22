//http://www.boi2011.ro/resurse/tasks/timeismoney-sol.pdf
#include <bits/stdc++.h>
using namespace std;
#define all(a) begin(a), end(a)
#define csz(a) (int) a.size()
#define load(a, v) fill(begin(a), end(a), v)
#define load_mem(a, v) memset(a, v, sizeof(a));
#define iostream_optimize() ios::sync_with_stdio(false); cin.tie(0);
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

struct edge {
	int u, v;
	long t, c;
};

class TimeIsMoney {
public:
	int n, m;
	vector<edge> edges;
	pair<long, pair<long, long> > res = {LINF, {LINF, LINF}};
	istream* cin_ptr;
	ostream* cout_ptr;

	void tracer(long T, long C, vector<edge> trace_ls) {
		ostream& cout = *cout_ptr;
		cout << T << ' ' << C << '\n';
		for(edge e: trace_ls) cout << e.u << ' ' << e.v << '\n';
	}

	pair<long, long> gen_mst(long a, long b, bool trace) {
		vector<edge> ls = edges, trace_ls;
		sort(ls.begin(), ls.end(), [a, b] (auto x, auto y) { return x.t * a + x.c * b < y.t * a + y.c * b; });
		vector<int> dsu(n, -1);
		function<int(int)> root;
		root = [&root, &dsu] (int v) { return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v])); };
		pair<long, long> ret = {0, 0};

		for(edge e: ls) {
			int u = root(e.u), v = root(e.v);
			if (u == v) continue;
			dsu[u] += dsu[v];
			dsu[v] = u;
			ret.first += e.t;
			ret.second += e.c;
			if(trace) trace_ls.push_back(e);
		}

		res = min(make_pair(ret.first * ret.second, make_pair(a, b)), res);
		if(trace) tracer(ret.first, ret.second, trace_ls);

		return ret;
	}

	bool ccw(pair<long, long> p1, pair<long, long> p2, pair<long, long> p3) {
		pair<long, long> v1 = {p2.first - p1.first, p2.second - p1.second};
		pair<long, long> v2 = {p3.first - p2.first, p3.second - p2.second};
		long cross = v1.first * v2.second - v1.second * v2.first;
		return cross > 0;
	}

	void rec(pair<long, long> p1, pair<long, long> p2) {
		res.first = min(p1.first * p1.second, res.first);
		res.first = min(p2.first * p2.second, res.first);
		long b = abs(p2.first - p1.first);
		long a = abs(p2.second - p1.second);
		auto opt = gen_mst(a, b, 0);
		if(ccw(p1, opt, p2)) rec(p1, opt), rec(opt, p2);
	}

	void solve(istream& in, ostream& out) {
		cin_ptr = &in;
		cout_ptr = &out;
		istream& cin = *cin_ptr;
		ostream& cout = *cout_ptr;
		cin >> n >> m;
		for(int i = 0; i < m; i++) {
			edge e;
			cin >> e.u >> e.v >> e.t >> e.c;
			edges.push_back(e);
		}
		auto leftmost = gen_mst(1, 0, 0);
		auto bottommost = gen_mst(0, 1, 0);
		rec(leftmost, bottommost);
		gen_mst(res.second.first, res.second.second, 1);
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TimeIsMoney solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
