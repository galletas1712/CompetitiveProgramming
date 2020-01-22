// Fix the right bound. This gives us exactly what to do for suffix sums. Then use segment tree to determine the lowest possible prefix sum value.
// This simplifies the problem, as if we only consider one dimension (either prefix or suffix sums), the problem is much easier. This offline sorting does just that for us.

#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 19;

class Elections {

	int n, q, ans[N];
	string s;
	vector<pair<int,int> > qq[N];

	int mn[2*N], lz[2*N];
	void apply(int i, int l, int r) {
		if(!lz[i]) return;
		mn[i] += lz[i];
		if(i < N) {
			lz[i << 1] += lz[i];
			lz[i << 1 | 1] += lz[i];
		}
		lz[i] = 0;
	}
	void update_mn(int tl, int tr, int v, int i = 1, int l = 0, int r = N-1) {
		apply(i, l, r);
		if(r < tl || l > tr) return;
		if(tl <= l && r <= tr) {
			lz[i] += v;
			apply(i, l, r);
			return;
		}
		int mid = l+r >> 1;
		update_mn(tl, tr, v, i << 1, l, mid);
		update_mn(tl, tr, v, i << 1 | 1, mid+1, r);
		mn[i] = min(mn[i << 1], mn[i << 1 | 1]);
	}
	int query_mn(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
		apply(i, l, r);
		if(r < tl || l > tr) return INF;
		if(tl <= l && r <= tr) return mn[i];
		int mid = l+r >> 1;
		return min(query_mn(tl, tr, i << 1, l, mid), query_mn(tl, tr, i << 1 | 1, mid+1, r));
	}

public:
	void run(istream &cin, ostream &cout) {
		cin >> n >> s >> q;
		for(int i = 0, l, r; i < q; i++) {
			cin >> l >> r;
			--l, --r;
			qq[r].emplace_back(l, i);
		}
		vector<int> ts;
		for(int i = 0; i < n; i++) {
			if(s[i] == 'T') ts.push_back(i);
			else {
				if(!ts.empty()) {
					update_mn(ts.back(), n-1, -1);
					ts.pop_back();
				}
				update_mn(i, n-1, 1);
			}
			for(auto p: qq[i]) {
				int j, qidx;
				tie(j, qidx) = p;
				int res = query_mn(j, i) - (j > 0 ? query_mn(j-1, j-1) : 0);
				auto it = lower_bound(ts.begin(), ts.end(), j);
				int cnt = (int) ts.size() - (it - ts.begin());
				ans[qidx] = cnt - min(res, 0);
			}
		}
		for(int i = 0; i < q; i++) cout << ans[i] << '\n';
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    Elections *obj = new Elections();
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
