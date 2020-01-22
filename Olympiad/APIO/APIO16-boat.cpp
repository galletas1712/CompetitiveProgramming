/*
 * dp[i][j] = number of ways with the last school at i, where the value is in block j
 * Many schools may have boat counts within block j, so we need to iterate those
 * Most basic form: dp[i][j] = sum of dp[k][x], where k < i and x < j (very bad complexity, no grouping yet)
 * f[i][j] = number of ways to assign j schools to len[i] values, where one school is already fixed (its id is fixed and its value is within block i)
 * So dp[i][j] = sum of dp[k][j-1] * f[j][cnt], as dp[i][j] now keeps prefix sums of j and cnt is incremental in decreasing order of k
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 510;

class APIO16_boat {

	void add(long &a, long b) {
		a += b;
		if(a >= MOD) a -= MOD;
	}
	long mul(long a, long b) { return a*b % MOD; }
	long modPow(long b, long e) {
		if(e == 0) return 1;
		if(e == 1) return b;
		return mul(modPow(mul(b, b), e >> 1), modPow(b, e & 1));
	}

	int n, a[N], b[N];
	pair<int,int> segs[N];
	vector<int> vals;
	unordered_map<int, int> rev;

	long ans, inv[N], C[N][N], f[N][N], dp[N][N];

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		vals.push_back(0);
		for(int i = 1; i <= n; i++) {
			cin >> a[i] >> b[i];
			vals.push_back(a[i]);
			vals.push_back(b[i]+1);
		}
		sort(vals.begin(), vals.end());
		vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
		for(int i = 0; i < vals.size(); i++) rev[vals[i]] = i;
		for(int i = 1; i <= n; i++) segs[i] = {rev[a[i]], rev[b[i]+1]};

		inv[0] = 1;
		for(int i = 1; i <= n; i++) inv[i] = modPow(i, MOD-2);
		C[0][0] = 1;
		for(int i = 1; i <= n; i++) {
			C[i][0] = C[i][i] = 1;
			for(int j = 1; j < i; j++) {
				add(C[i][j], C[i-1][j-1]);
				add(C[i][j], C[i-1][j]);
			}
		}

		for(int j = 1; j < vals.size()-1; j++) {
			f[j][0] = 1;
			for(int cnt = 1; cnt <= min(vals[j+1] - vals[j], n); cnt++) {
				f[j][cnt] = mul(f[j][cnt - 1], vals[j+1] - vals[j] - (cnt - 1));
				f[j][cnt] = mul(f[j][cnt], inv[cnt]);
			}
			for(int cnt = n; cnt >= 1; cnt--) {
				for(int x = 1; x <= min(cnt-1, n); x++) {
					add(f[j][cnt], mul(f[j][x], C[cnt-1][x-1]));
				}
			}
		}

		for(int j = 0; j < vals.size(); j++) dp[0][j] = 1;
		for(int i = 1; i <= n; i++) {
			for(int j = segs[i].first; j < segs[i].second; j++) {
				int cnt = 0;
				for(int k = i-1; k >= 0; k--) {
					cnt += segs[k+1].first <= j && j < segs[k+1].second;
					add(dp[i][j], mul(dp[k][j-1], f[j][cnt]));
				}
			}
			for(int j = 1; j < vals.size(); j++) add(dp[i][j], dp[i][j-1]);
			add(ans, dp[i][vals.size() - 1]);
		}
		cout << ans << endl;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    APIO16_boat *obj = new APIO16_boat();
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
