/*
 * It's easy to see the if the requirement were n/2 instead of n/3, the problem would be easy.
 * It would look like this:
 * 0000....
 * ....0000
 *
 * If s was a permutation, we would fix n/3 duplicates on one side and make the 2n/3 rest unique.
 * We also need to make sure that when we split, the values don't intersect (if b[i] != 0 -> b[i] > a[j], where j is in lower segment and i is in upper segment)
 * 1 2 3 4 | 5 6 7 8 9 10 11 12
 * 0 0 0 0 | 5 1 7 2 9 3  11 4
 * 1 2 3 4 | 0 5 0 6 0 7  0  8
 *
 * Once this has been established, it is easy to extrapolate to a general solution.
 * Just change all values of a and b to match for corresponding s[i], except for the _1_2_3_4... in array a.
 * That needs to be fixed because its purpose is to make sure that the values don't intersect, and if we matched it, this may not hold anymore.
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class TaskC {
	int n;
public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		vector<int> s(n), a(n), b(n), tmp;
		map<int, int> pos;
		for(int i = 0; i < n; i++) {
			cin >> s[i];
			tmp.push_back(s[i]);
			pos[s[i]] = i;
		}
		sort(tmp.begin(), tmp.end());
		for(int i = (tmp.size() + 2) / 3; i < n; i += 2) {
			a[pos[tmp[i]]] = tmp[i];
		}
		for(int i = (tmp.size() + 2) / 3 + 1; i < n; i += 2) {
			a[pos[tmp[i]]] = (i - ((tmp.size() + 2) / 3 - 1)) / 2;
		}
		cout << "YES" << endl;
		for(int x: a) cout << x << ' ';
		cout << endl;
		for(int i = 0; i < n; i++) cout << s[i] - a[i] << ' ';
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
