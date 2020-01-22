// Use KMP to determine how many characters have been matched up so far
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};


class NewTaskE {
	string s, revs;
	int m;

	vector<int> match(string s, string t) {
		vector<int> f(t.length() + 1);
		f[0] = -1;
		f[1] = 0;
		for(int i = 2; i <= t.length(); i++) {
			f[i] = f[i-1];
			while(f[i] != -1 && t[f[i]] != t[i - 1]) f[i] = f[f[i]];
			++f[i];
		}
		vector<int> res;
		for(int i = 0, curr = 0; i < s.length(); i++) {
			while(curr != -1 && s[i] != t[curr]) curr = f[curr];
			++curr;
			res.push_back(curr);
			if(curr == t.length()) curr = f[curr];
		}
		return res;
	}

public:
	void solve(istream& cin, ostream& cout) {
		cin >> s >> m;
		revs = s;
		reverse(revs.begin(), revs.end());
		int ans = 0;
		for(int z = 0; z < m; z++) {
			string t, revt;
			cin >> t;
			if(t.length() == 1) continue;
			revt = t;
			reverse(revt.begin(), revt.end());
			vector<int> res = match(s, t);
			vector<int> res_rev = match(revs, revt);
			for(int i = 1; i < res.size(); i++) res[i] = max(res[i-1], res[i]);
			for(int i = 1; i < res_rev.size(); i++) res_rev[i] = max(res_rev[i-1], res_rev[i]);
			reverse(res_rev.begin(), res_rev.end());
			for(int i = 0; i < res.size()-1; i++) {
//				cout << z << ' ' << i << ' ' << res[i] << ' ' << res_rev[i+1] << ' ' << endl;
				if(res[i] && res_rev[i+1] && res[i] + res_rev[i+1] >= t.length()) {
					++ans;
					break;
				}
			}
		}
		cout << ans << endl;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	NewTaskE solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
