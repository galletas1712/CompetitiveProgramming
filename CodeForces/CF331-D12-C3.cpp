// Solution notes in iPad
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class TaskC {

	map<pair<long, long>, long> dp, res;

	long calc(long num, long g) {
		if(num == 0) return 0;
		if(dp.count(make_pair(num, g))) return dp[{num, g}];

		long lead = num;
		while(lead >= 10) lead /= 10;
		long lead_p = lead;
		while(lead_p <= num) lead_p *= 10;
		lead_p /= 10;

		long ret;
		if(lead_p == num) {
			ret = dp[{num, g}] = calc(max(num - max(g, lead), 0ll), g) + 1;
			if(num - max(g, lead) < 0) {
				res[{num, g}] = num - max(g, lead);
			} else {
				res[{num, g}] = res[{max(num - max(g, lead), 0ll), g}];
			}
		} else {
			assert(num - lead >= 0);
			ret = dp[{num, g}] = calc(num - lead_p, max(g, lead)) + calc(lead_p + res[{num - lead_p, max(g, lead)}], g);
			res[{num, g}] = res[{lead_p + res[{num - lead_p, max(g, lead)}], g}];
		}
		return ret;
	}

	int brute(int n) {
		vector<int> dp(n+1);
		for(int i = 1; i <= n; i++) {
			dp[i] = INF;
			vector<int> digits;
			int tmp = i;
			while(tmp) digits.push_back(tmp % 10), tmp /= 10;
			for(int d: digits) dp[i] = min(dp[i - d] + 1, dp[i]);
		}
		return dp[n];
	}

public:
	void solve(istream &cin, ostream &cout) {
		long n;
		cin >> n;
		cout << calc(n, 0) << endl;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskC *obj = new TaskC();
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
