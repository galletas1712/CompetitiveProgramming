// Count # of rectangles that don't intersect the current rectangle (iterate from back to front, to also preserve order)
// We do this using a fenwick tree to maintain the prefix sum of each coordinate (x1, y1, x2, y2)
// We double count the ones where the rectangle lies in a corner so we have to also count and subtract those using a merge sort tree
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

struct Fenwick {
	vector<int> f;
	Fenwick() {}
	Fenwick(int n) {
		f.resize(n + 2);
	}
	void update(int i, int v) {
		++i;
		while(i < f.size()) f[i] += v, i += i & -i;
	}
	int query(int i) {
		++i;
		int ret = 0;
		while(i > 0) ret += f[i], i -= i & -i;
		return ret;
	}
};

struct MST {
	int n;
	vector<vector<int> > ls;
	vector<Fenwick> t;
	MST() {}
	MST(int n): n(n) {
		ls.resize(2*n);
		t.resize(2*n);
	}
	void clear() {
		for(int i = 1; i < 2*n; i++) fill(t[i].f.begin(), t[i].f.end(), 0);
	}
	void add_value(int x, int y) {
		ls[x + n].push_back(y);
	}
	void build() {
		for(int i = n; i < 2*n; i++) {
			sort(ls[i].begin(), ls[i].end());
			ls[i].resize(unique(ls[i].begin(), ls[i].end()) - ls[i].begin());
			t[i] = Fenwick(ls[i].size());
		}
		for(int i = n-1; i >= 1; i--) {
			merge(ls[i << 1].begin(), ls[i << 1].end(), ls[i << 1 | 1].begin(), ls[i << 1 | 1].end(), back_inserter(ls[i]));
			ls[i].resize(unique(ls[i].begin(), ls[i].end()) - ls[i].begin());
			t[i] = Fenwick(ls[i].size());
		}
	}
	void update(int x, int y) {
		for(x += n; x >= 1; x >>= 1) {
			auto it = lower_bound(ls[x].begin(), ls[x].end(), y);
			t[x].update(it - ls[x].begin(), 1);
		}
	}
	int query(int x1, int y1, int x2, int y2) {
		int ret = 0;
		for(x1 += n, x2 += n+1; x1 < x2; x1 >>= 1, x2 >>= 1) {
			if(x1 & 1) {
				auto ity1 = lower_bound(ls[x1].begin(), ls[x1].end(), y1);
				auto ity2 = upper_bound(ls[x1].begin(), ls[x1].end(), y2);
				if(ity1 != ls[x1].end()) {
					ret += t[x1].query(ity2 - ls[x1].begin() - 1) - t[x1].query(ity1 - ls[x1].begin() - 1);
				}
				++x1;
			}
			if(x2 & 1) {
				--x2;
				auto ity1 = lower_bound(ls[x2].begin(), ls[x2].end(), y1);
				auto ity2 = upper_bound(ls[x2].begin(), ls[x2].end(), y2);
				if(ity1 != ls[x2].end()) {
					ret += t[x2].query(ity2 - ls[x2].begin() - 1) - t[x2].query(ity1 - ls[x2].begin() - 1);
				}
			}
		}
		return ret;
	}
};

class Suncanje {

	int n;
	vector<tuple<int,int,int,int> > a;
	vector<int> res;
	vector<string> ans;
	MST mst1, mst2;
	Fenwick fen[4];

	vector<int> xs, ys;
	void compress() {
		for(int i = 0, x1, y1, x2, y2; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			xs.push_back(x1);
			xs.push_back(x2);
			ys.push_back(y1);
			ys.push_back(y2);
		}
		sort(xs.begin(), xs.end());
		xs.resize(unique(xs.begin(), xs.end()) - xs.begin());
		sort(ys.begin(), ys.end());
		ys.resize(unique(ys.begin(), ys.end()) - ys.begin());
		for(auto &tt: a) {
			int x1, y1, x2, y2;
			tie(x1, y1, x2, y2) = tt;
			auto it = lower_bound(xs.begin(), xs.end(), x1);
			get<0>(tt) = it - xs.begin();
			it = lower_bound(xs.begin(), xs.end(), x2);
			get<2>(tt) = it - xs.begin();
			it = lower_bound(ys.begin(), ys.end(), y1);
			get<1>(tt) = it - ys.begin();
			it = lower_bound(ys.begin(), ys.end(), y2);
			get<3>(tt) = it - ys.begin();
		}
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		for(int i = 0, x1, y1, A, B; i < n; i++) {
			cin >> x1 >> y1 >> A >> B;
			a.emplace_back(x1, y1, x1 + A, y1 + B);
		}
		res.resize(n);
		compress();
		reverse(a.begin(), a.end());
		int sz = max(xs.size(), ys.size());
		for(int i = 0; i < 4; i++) fen[i] = Fenwick(sz);
		mst1 = MST(xs.size());
		mst2 = MST(xs.size());
		for(auto tt: a) {
			int x1, y1, x2, y2;
			tie(x1, y1, x2, y2) = tt;
			mst1.add_value(x1, y1);
			mst1.add_value(x1, y2);
			mst2.add_value(x2, y1);
			mst2.add_value(x2, y2);
		}
		mst1.build();
		mst2.build();
		int x1, y1, x2, y2;
		for(int i = 0; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			res[i] -= mst1.query(x2, y2, xs.size() - 1, ys.size() - 1);
			mst1.update(x1, y1);
		}
		mst1.clear();
		for(int i = 0; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			res[i] -= mst1.query(x2, 0, xs.size() - 1, y1);
			mst1.update(x1, y2);
		}
		for(int i = 0; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			res[i] -= mst2.query(0, y2, x1, ys.size() - 1);
			mst2.update(x2, y1);
		}
		mst2.clear();
		for(int i = 0; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			res[i] -= mst2.query(0, 0, x1, y1);
			mst2.update(x2, y2);
		}

		for(int i = 0; i < n; i++) {
			tie(x1, y1, x2, y2) = a[i];
			res[i] += fen[2].query(x1) + i - fen[0].query(x2 - 1);
			res[i] += fen[3].query(y1) + i - fen[1].query(y2 - 1);
			fen[0].update(x1, 1);
			fen[1].update(y1, 1);
			fen[2].update(x2, 1);
			fen[3].update(y2, 1);
		}
		reverse(res.begin(), res.end());
		for(int i = 0; i < n; i++) cout << (res[i] == n - i - 1 ? "DA\n" : "NE\n");

	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    Suncanje *obj = new Suncanje();
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
