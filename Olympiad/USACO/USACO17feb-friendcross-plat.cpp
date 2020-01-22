#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class WhyDidTheCowCrossTheRoad3 {
	int n, K;
	long ans;
	vector<int> a, b, revb;
	vector<vector<int> > ls, t;
	vector<pair<int,int> > ord;
	void allocate() {
		ans = 0;
		a = vector<int>(n+1);
		b = vector<int>(n+1);
		revb = vector<int>(n+1);
		ls = vector<vector<int> >(2*(n+1));
		t = vector<vector<int> >(2*(n+1));
	}
	void build() {
		for(int i = 0; i < n; i++) ls[n+i].push_back(b[i]);
		for(int i = n-1; i >= 1; i--) {
			ls[i].resize(ls[i << 1].size() + ls[i << 1 | 1].size());
			merge(ls[i << 1].begin(), ls[i << 1].end(), ls[i << 1 | 1].begin(), ls[i << 1 | 1].end(), ls[i].begin());
			ls[i].resize(unique(ls[i].begin(), ls[i].end()) - ls[i].begin());
		}
		for(int i = 0; i < 2*(n+1); i++) t[i].resize(ls[i].size() + 1);
	}
	int get_rb(vector<int> &arr, int v) {
		auto it = upper_bound(arr.begin(), arr.end(), v);
		if(arr.empty() || it == arr.begin()) return -1;
		return it - arr.begin() - 1;
	}
	int get_lb(vector<int> &arr, int v) {
		auto it = lower_bound(arr.begin(), arr.end(), v);
		if(arr.empty() || it == arr.end()) return -1;
		return it - arr.begin();
	}
	void fen_update(vector<int> &f, int i) {
		++i;
		assert(0 <= i && i < f.size());
		while(i < f.size()) ++f[i], i += i & -i;
	}
	int fen_query(vector<int> &f, int i) {
		++i;
		assert(0 <= i && i < f.size());
		int ret = 0;
		while(i > 0) ret += f[i], i -= i & -i;
		return ret;
	}
	void update(int i, int v) {
		i += n;
		fen_update(t[i], get_lb(ls[i], v));
		while(i >>= 1) fen_update(t[i], get_lb(ls[i], v));
	}
	int query(int l, int r, int lv, int rv) {
		int ret = 0;
		for(l += n, r += n+1; l < r; l >>= 1, r >>= 1) {
			if(l & 1) {
				int lb = get_lb(ls[l], lv), rb = get_rb(ls[l], rv);
				if(lb != -1 && rb != -1) ret += fen_query(t[l], rb) - fen_query(t[l], lb-1);
				l++;
			}
			if(r & 1) {
				--r;
				int lb = get_lb(ls[r], lv), rb = get_rb(ls[r], rv);
				if(lb != -1 && rb != -1) ret += fen_query(t[r], rb) - fen_query(t[r], lb-1);
			}
		}
		return ret;
	}
public:
	void solve() {
		freopen("friendcross.in", "r", stdin);
		freopen("friendcross.out", "w", stdout);
		cin >> n >> K;
		allocate();
		for(int i = 0; i < n; i++) {
			cin >> a[i];
			--a[i];
		}
		for(int i = 0; i < n; i++) {
			cin >> b[i];
			--b[i];
			revb[b[i]] = i;
		}
		build();
		for(int i = 0; i < n; i++) {
			ans += query(revb[a[i]] + 1, n-1, 0, n-1) - query(revb[a[i]] + 1, n-1, max(a[i]-K, 0), min(a[i]+K, n-1));
			update(revb[a[i]], a[i]); // do get_lb in update function
		}
		cout << ans;
	}
};

int main() {
	cin.tie(0);
	WhyDidTheCowCrossTheRoad3 solver;
	solver.solve();
	return 0;
}
