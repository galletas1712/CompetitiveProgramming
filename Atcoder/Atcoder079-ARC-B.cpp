// https://img.atcoder.jp/arc079/editorial.pdf
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class TaskD {

	long k, n, a[51];

public:
	void solve(istream &cin, ostream &cout) {
		cin >> k;
		n = 50;
		long f = k / n;
		for(int i = 0; i < n; i++) a[i] = n-1 + f;
		for(int i = 0; i < k % n; i++) {
			a[i] += n;
			for(int j = 0; j < n; j++) if(j != i) --a[j];
		}
		cout << n << endl;
		for(int i = 0; i < n; i++) cout << a[i] << ' ';
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    TaskD *obj = new TaskD();
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
