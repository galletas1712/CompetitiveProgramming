//Use stack and four DP arrays
#include <bits/stdc++.h>

using namespace std;
#define long long long
const long MOD = 1e9 + 7, LINF = 1e18 + 1e16;
const int INF = 1e9 + 1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};


class Monsters {
	int n, m;
	vector<string> s;
	vector<vector<long> > up, down, dp1, dp2, dp3, dp4;
public:
	void init() {
		dp1 = vector<vector<long> >(n + 1, vector<long>(m + 1));
		dp2 = vector<vector<long> >(n + 1, vector<long>(m + 1));
		dp3 = vector<vector<long> >(n + 1, vector<long>(m + 1));
		dp4 = vector<vector<long> >(n + 1, vector<long>(m + 1));
		up = vector<vector<long> >(n + 1, vector<long>(m + 1));
		down = vector<vector<long> >(n + 1, vector<long>(m + 1));
		s.resize(n);
	}

	void solve(istream &cin, ostream &cout) {
		cin >> n >> m;
		init();
		for (int i = 0; i < n; i++) cin >> s[i];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (!i) up[i][j] = (s[i][j] == '1' ? -1 : 0);
				else up[i][j] = (s[i][j] == '1' ? up[i - 1][j] : i);
			}
		}
		for (int i = n - 1; i >= 0; i--) {
			for (int j = 0; j < m; j++) {
				if (i == n - 1) down[i][j] = (s[i][j] == '1' ? n : n - 1);
				else down[i][j] = (s[i][j] == '1' ? down[i + 1][j] : i);
			}
		}
		for (int i = 0; i < n; i++) {
			stack<pair<long, long> > stk;
			long sum = 0;
			auto f = [&](int j, long &dp_cell) {
				long cnt = 1;
				while (!stk.empty() && stk.top().first >= i - up[i][j]) {
					sum -= stk.top().first * stk.top().second;
					cnt += stk.top().second;
					stk.pop();
				}
				sum += cnt * (i - up[i][j]);
				dp_cell = sum;
				stk.emplace(i - up[i][j], cnt);
			};
			for (int j = 0; j < m; j++) f(j, dp1[i][j]);
			while (!stk.empty()) stk.pop();
			sum = 0;
			for (int j = m - 1; j >= 0; j--) f(j, dp2[i][j]);
		}
		for (int i = n - 1; i >= 0; i--) {
			stack<pair<long, long> > stk;
			long sum = 0;
			auto f = [&](int j, long &dp_cell) {
				long cnt = 1;
				while (!stk.empty() && stk.top().first >= down[i][j] - i) {
					sum -= stk.top().first * stk.top().second;
					cnt += stk.top().second;
					stk.pop();
				}
				sum += cnt * (down[i][j] - i);
				dp_cell = sum;
				stk.emplace(down[i][j] - i, cnt);
			};
			for (int j = 0; j < m; j++) f(j, dp3[i][j]);
			while (!stk.empty()) stk.pop();
			sum = 0;
			for (int j = m - 1; j >= 0; j--) f(j, dp4[i][j]);
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++)
				dp1[i][j] += (i ? dp1[i - 1][j] : 0) + (j ? dp1[i][j - 1] : 0) - (i && j ? dp1[i - 1][j - 1] : 0);
		for (int i = 0; i < n; i++)
			for (int j = m - 1; j >= 0; j--)
				dp2[i][j] += (i ? dp2[i - 1][j] : 0) + (j < m - 1 ? dp2[i][j + 1] : 0) -
							(i && j < m - 1 ? dp2[i - 1][j + 1] : 0);
		for (int i = n - 1; i >= 0; i--)
			for (int j = 0; j < m; j++)
				dp3[i][j] += (i < n - 1 ? dp3[i + 1][j] : 0) + (j ? dp3[i][j - 1] : 0) -
							(i < n - 1 && j ? dp3[i + 1][j - 1] : 0);
		for (int i = n - 1; i >= 0; i--)
			for (int j = m - 1; j >= 0; j--)
				dp4[i][j] += (i < n - 1 ? dp4[i + 1][j] : 0) + (j < m - 1 ? dp4[i][j + 1] : 0) -
							(i < n - 1 && j < m - 1 ? dp4[i + 1][j + 1] : 0);

		long ans = LINF;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				long two = (i ? dp1[i - 1][m - 1] : 0) + (j < m - 1 ? dp2[n - 1][j + 1] : 0) +
						  (i < n - 1 ? dp4[i + 1][0] : 0) + (j ? dp3[0][j - 1] : 0);
				long one = (i && j ? dp1[i - 1][j - 1] : 0) + (i && j < m - 1 ? dp2[i - 1][j + 1] : 0) +
						  (i < n - 1 && j < m - 1 ? dp4[i + 1][j + 1] : 0) + (i < n - 1 && j ? dp3[i + 1][j - 1] : 0);
				ans = min(two - one, ans);
			}
		}
		cout << ans << endl;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Monsters solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
