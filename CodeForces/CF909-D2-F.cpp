/* First subtask:
if N is odd, then there is no solution
if p[i] & i is 0, then i is a supermask of p[i]
it follows that we can reverse the order of ~p[i] to p[i] and assign them in that reversed order (see sample 1, when the two endpoints are 1 and 6)
if N is even, we can always decompose consecutive values like this

Second subtask is from editorial
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class TaskF {
	int n, p[N], q[N];

	bool solve1() {
		if(n % 2) return false;
		iota(p, p + n + 1, 0);
		int j = n;
		for (int i = n; i >= 1;) {
			while (j >= 1 && (i & j)) --j;
			assert(j >= 1);
			reverse(p + j, p + i + 1);
			i = j - 1;
		}
		return true;
	}

	bool solve2() {
		if(n < 6 || __builtin_popcount(n) == 1) return false;
		if(n == 6) {
			vector<int> res = {3, 6, 2, 5, 1, 4};
			for(int i = 0; i < 6; i++) q[i+1] = res[i];
			return true;
		}
		vector<int> res = {7, 3, 6, 5, 1, 2, 4};
		for(int i = 0; i < 7; i++) q[i+1] = res[i];
		for(int i = 8; i <= n; i <<= 1) {
			int j = min((i << 1) - 1, n);
			for(int k = i; k <= j; k++) q[k] = k;
			for(int k = i; k < j; k++) swap(q[k], q[k+1]);
		}
		return true;
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		if(solve1()) {
			cout << "YES" << endl;
			for(int i = 1; i <= n; i++) cout << p[i] << ' ', assert(!(p[i] & i));
		} else cout << "NO";
		cout << endl;
		if(solve2()) {
			cout << "YES" << endl;
			for(int i = 1; i <= n; i++) cout << q[i] << ' ', assert(q[i] & i);
		} else cout << "NO";
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
