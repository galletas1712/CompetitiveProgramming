/*
 * sort strings lexicographically
 * divide and conquer
 * divide using minimum common prefix and merge with DP
 * Can prove that sum of DP over all segments gets runtime O(n^2)
 * Need to handle mn value properly to avoid double counting the sum
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class NewTaskF {

	int n, k;
	vector<string> s;
	vector<int> lcp;

	vector<long> rec(int l, int r) {
		if(l == r) return vector<long>(2, 0);
		vector<pair<int,int> > down;
		int mn = INF;
		for(int i = l; i < r; i++) mn = min(lcp[i], mn);
		int last = l;
		for(int i = l; i < r; i++) if(lcp[i] == mn) {
				down.emplace_back(last, i);
				last = i+1;
			}
		down.emplace_back(last, r);
		vector<long> dp(min(r-l+1, k) + 1, -LINF);
		dp[0] = 0;
		for(int i = l; i < r; i++) lcp[i] -= mn;
		for(auto p: down) {
			vector<long> tmp = rec(p.first, p.second);
			for(int kk = dp.size() - 1; kk >= 0; kk--) {
				for(int len = 0; len <= min(kk, (int) tmp.size() - 1); len++) {
					dp[kk] = max(dp[kk - len] + tmp[len], dp[kk]);
				}
			}
		}
		for(int i = l; i < r; i++) lcp[i] += mn;
		for(int kk = 0; kk < dp.size(); kk++) dp[kk] += 1ll * mn * kk * (kk - 1) / 2;
		return dp;
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> k;

		s.resize(n);
		lcp = vector<int>(n);

		for(int i = 0; i < n; i++) cin >> s[i];
		sort(s.begin(), s.end());
		for(int i = 0; i < n-1; i++) {
			while(lcp[i] < min(s[i].length(), s[i+1].length()) && s[i][lcp[i]] == s[i+1][lcp[i]])
				++lcp[i];
		}
		vector<long> dp = rec(0, n-1);
		cout << dp[k];
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    NewTaskF *obj = new NewTaskF();
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
