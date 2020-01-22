/*
Notice that in each segment, we don't need to flip more than 2 times
dp[i][j][p] = used i segments with j seconds on side "1" and exiting on side p

dp[i][j][0] = min(dp[i-1][x][0], dp[i-1][x-y-(l[i] - r[i-1])][1] + 1, dp[i-1][x-y][0] + 2), 0 <= y <= r[i] - l[i]
dp[i][j][1] = min(dp[i-1][x-(r[i]-r[i-1])][1], dp[i-1][x-y][0] + 1, dp[i-1][x-y-(l[i] - r[i-1])][1] + 2)

Actual limits for x-y-(l[i] - r[i-1]) determined to be [ x+r[i-1]-r[i], x+r[i-1]-l[i] ]
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 101, M = 1e5+1;

struct RMQ {
	int n;
	vector<vector<int> > t;
	RMQ() {}
	RMQ(vector<int> vals): n(vals.size()) {
		t = vector<vector<int> > (n);
		for(int i = 0; i < n; i++) {
			t[i].push_back(vals[i]);
			for(int j = 0; 1 << j+1 <= i+1; j++)
				t[i].push_back(min(t[i][j], t[i - (1 << j)][j]));
		}
	}
	int query(int l, int r) {
		if(r < 0 || l > n-1) return INF;
		l = max(0, l);
		r = min(n-1, r);
		int lg = 31 - __builtin_clz(r-l+1);
		return min(t[r][lg], t[l + (1 << lg) - 1][lg]);
	}
};

class TaskF {

	int n, k, l[N], r[N], dp[N][M][2];
	RMQ rmq[2];

	void setmin(int &a, int b) { a = min(a, b); }

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> k;
		for(int j = 0; j <= n; j++) dp[0][j][0] = dp[0][j][1] = INF;
		dp[0][0][0] = 0;
		dp[0][0][1] = 0;
		for(int i = 1; i <= k; i++) {
			cin >> l[i] >> r[i];
			for(int p = 0; p < 2; p++) {
				vector<int> vals;
				for(int j = 0; j <= n; j++) vals.push_back(dp[i-1][j][p]);
				rmq[p] = RMQ(vals);
			}
			for(int j = 0; j <= n; j++) {
				dp[i][j][0] = dp[i][j][1] = INF;
				setmin(dp[i][j][0], dp[i-1][j][0]); // no flip
				setmin(dp[i][j][0], rmq[1].query(j + r[i-1] - r[i], j + r[i-1] - l[i]) + 1); // one flip
				setmin(dp[i][j][0], rmq[0].query(j - (r[i] - l[i]), j) + 2); // two flip

				if(j - (r[i] - r[i-1]) >= 0) setmin(dp[i][j][1], dp[i-1][j - (r[i] - r[i-1])][1]);
				setmin(dp[i][j][1], rmq[0].query(j - (r[i] - l[i]), j) + 1);
				setmin(dp[i][j][1], rmq[1].query(j - (r[i] - l[i]), j) + 2);
			}
		}
		int ans = min(dp[k][n][0], (r[k] >= n ? dp[k][n - (2*n - r[k])][1] : INF));
		if(ans == INF) cout << "Hungry";
		else cout << "Full\n" << ans;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskF *obj = new TaskF();
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
