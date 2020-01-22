// DP with recurrence based of 559C. Notice that we got down to around lg s to reach s == 1
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e3+10, M = 2e5+10, K = 31;

class TaskE {
	int n, m, k, s;
	vector<pair<int,int> > a;
	long fact[M], invf[M];
	long dp[N][K], paths[N][N], fs[K];

	long modPow(long b, long e) {
		if(e == 0) return 1;
		if(e == 1) return b;
		return modPow(b*b % MOD, e >> 1) * modPow(b, e & 1) % MOD;
	}

public:
	void run(istream &cin, ostream &cout) {
		cin >> n >> m >> k >> s;

		fact[0] = 1;
		for(int i = 1; i <= n + m; i++) fact[i] = fact[i-1] * i % MOD;
		invf[0] = 1;
		for(int i = 1; i <= n + m; i++) invf[i] = modPow(fact[i], MOD-2);
		fs[0] = s;
		for(int i = 1; i < K; i++) fs[i] = fs[i-1] + 1 >> 1;

		a.emplace_back(1, 1);
		a.emplace_back(n, m);
		for(int i = 0, x, y; i < k; i++) {
			cin >> x >> y;
			a.emplace_back(x, y);
		}
		sort(a.begin(), a.end());
		k += 2;

		for(int i = 0; i < k; i++) {
			for(int j = 0; j < k; j++) {
				int dx = abs(a[i].first - a[j].first);
				int dy = abs(a[i].second - a[j].second);
				paths[i][j] =  fact[dx + dy] * invf[dx] % MOD * invf[dy] % MOD;
			}
		}

		dp[0][0] = 1;
		for(int i = 1; i < k; i++) {
			for(int x = 1; x < K; x++) {
				for(int j = 0; j < i; j++) if(a[j].first <= a[i].first && a[j].second <= a[i].second) {
					dp[i][x] += dp[j][x-1] * paths[i][j] % MOD;
					dp[i][x] %= MOD;
				}
				for(int j = 0; j < i; j++) if(a[j].first <= a[i].first && a[j].second <= a[i].second) {
					dp[i][x] -= dp[j][x] * paths[i][j] % MOD;
					dp[i][x] = (dp[i][x] % MOD + MOD) % MOD;
				}
			}
		}
		long P = 0, sum_less = 0;
		for(int x = 1; x < K; x++) {
			P += dp[k-1][x] * fs[x-1] % MOD;
			sum_less += dp[k-1][x];
			P %= MOD;
			sum_less %= MOD;
		}
		P += paths[0][k-1] - sum_less;
		P = (P % MOD + MOD) % MOD;
		cout << (P * modPow(paths[0][k-1], MOD-2) % MOD + MOD) % MOD;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskE *obj = new TaskE();
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
