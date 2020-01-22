/*
 * We'll try to solve this problem using an approach of moving elements from the middle to the extreme indices incrementally
 * Denote b = character to move, a = prefix excluding b, c = suffix excluding b, x' as the reverse of substring x
 * To move b to the front, we want to go from abc to bac
 * To move b to the back, we want to go from abc to acb
 * Notice that none of these can be done in three moves (which is the maximum we can do)
 * However, notice that c'a'b can be reached within exactly three moves (draw tree of outcomes of all possible three operations)
 * By going to c'a'b, we are basically going to bac reversing it after the operation (b' is the same as b)
 * However, we are constrained to only move elements to the front, which means we will alternate adding to prefixes and suffixes.
 * Solution is to start at the center and incrementally add left and right
 * In the end, we reverse the string again if it is still reversed (only happens when n is odd)
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class TaskC {

	int n, cntS[26], cntT[26];
	string s, t;
	vector<int> ops;

public:
	void solve(istream &cin, ostream &cout) {

		auto shift = [&] (int k) {
			ops.push_back(k);
			string b = s.substr(n - k, k);
			reverse(b.begin(), b.end());
			s = b + s.substr(0, n - k);
		};

		cin >> n >> s >> t;
		for(char c: s) ++cntS[c - 'a'];
		for(char c: t) ++cntT[c - 'a'];
		for(int i = 0; i < 26; i++) if(cntS[i] != cntT[i]) {
			cout << -1;
			return;
		}
		int pref = 0, suff = 0;
		for(int z = 0; z < n; z++) {
			int pos = -1;
			if(z % 2 == 0) {
				for(int i = pref; i < n - suff; i++) {
					if(s[i] == t[(n-1)/2 - z/2]) {
						pos = i;
						break;
					}
				}
				++pref;
			} else {
				for(int i = suff; i < n - pref; i++) {
					if(s[i] == t[(n+1)/2 + z/2]) {
						pos = i;
						break;
					}
				}
				++suff;
			}
			if(pos == -1) continue;
			shift(n - pos);
			shift(n);
			shift(n - pos - 1);
		}
		if(n % 2) shift(n);
		cout << ops.size() << '\n';
		for(int k: ops) cout << k << ' ';
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
