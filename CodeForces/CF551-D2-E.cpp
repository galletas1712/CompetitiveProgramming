// Use sorted lists and do lower_bound in order to check for existence within segment. Optionally, we could use this to find min and max values right away without iterating the whole segment, but I didn't want to waste time changing my implmentation that much.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int SZ = 600, N = 5e5+1;

struct qdata {
	int tp, l, r, x;
};

class TaskE {
	int n, q;
	long a[N];
	int which_block[N];
	long blanket[1000];
	vector<pair<long,int> > ls[1000];
	vector<tuple<long,int,long> > stk;
public:
	TaskE() {
		fill_n(blanket, 1000, 0);
		for(int i = 0; i < 1000; i++) ls[i] = vector<pair<long, int> >();
		stk.clear();
	}
	void stack_replace(long old_val, int idx, long new_val) {
		stk.emplace_back(old_val, idx, new_val);
	}
	void execute_replace(int block) {
		sort(stk.begin(), stk.end());
		int i = 0, j = 0;
		vector<pair<long,int> > res;
//		cout << "lists: " << endl;
//		cout << "tt: " << endl;
//		for(auto tt: stk) {
//			long a, b, c;
//			tie(a, b, c) = tt;
//			cout << a << ' ' << b << ' ' << c << endl;
//		}
//		cout << "ls[block]: " << endl;
//		for(auto p: ls[block]) cout << p.first << ' ' << p.second << endl;
//		cout << endl;
		while(i < stk.size() || j < ls[block].size()) {
			if(i == stk.size()) {
				res.push_back(ls[block][j++]);
				continue;
			}
			if(j == ls[block].size()) {
				break;
			}
			assert(j != ls[block].size());
			auto test = make_pair(get<0>(stk[i]), get<1>(stk[i]));
			if(test == ls[block][j]) res.emplace_back(get<2>(stk[i]), test.second), i++, j++;
			else if(test > ls[block][j]) res.push_back(ls[block][j++]);
			else cout << "test < ls[block][j]" << endl, assert(false);
		}
		ls[block] = res;
		stk.clear();
		sort(ls[block].begin(), ls[block].end());
//		cout << "res: " << endl;
//		for(auto p: ls[block]) cout << p.first << ' ' << p.second << endl;
//		cout << endl;
	}
	void solve(istream& cin, ostream& cout) {
		cin >> n >> q;
		for(int i = 0; i < n; i++) which_block[i] = i / SZ;
		for(int i = 0; i < n; i++) {
			cin >> a[i];
			ls[which_block[i]].emplace_back(a[i], i);
		}
		for(int i = 0; i < n; i += SZ) sort(ls[which_block[i]].begin(), ls[which_block[i]].end());
		for(int t = 0; t < q; t++) {
			qdata qq;
			cin >> qq.tp;
			if(qq.tp == 1) {
				cin >> qq.l >> qq.r >> qq.x;
				--qq.l, --qq.r;
				int l = qq.l, r = qq.r;
				int org_l_block = which_block[l], org_r_block = which_block[r];
				while(which_block[l] * SZ != l && l <= r) {
					stack_replace(a[l], l, a[l] + qq.x);
					a[l] += qq.x;
					++l;
				}
				execute_replace(org_l_block);
				while(which_block[r] * SZ + SZ - 1 != r && r >= l) {
					stack_replace(a[r], r, a[r] + qq.x);
					a[r] += qq.x;
					--r;
				}
				execute_replace(org_r_block);
				for(; l+SZ-1 <= r; l += SZ) {
					blanket[which_block[l]] += qq.x;
				}
			} else {
				cin >> qq.x;
				int small_block = INF, large_block = -INF;
				for(int i = 0; i < n; i += SZ) {
					auto it = lower_bound(ls[which_block[i]].begin(), ls[which_block[i]].end(), make_pair(qq.x - blanket[which_block[i]], -1));
					if(it != ls[which_block[i]].end() && it->first + blanket[which_block[i]] == qq.x) {
//						cout << "block " << which_block[i] << " blanket " << blanket[which_block[i]] << endl;
//						cout << "yay " << it->first << ' ' << it->second  << endl;
//						for(auto p: ls[which_block[i]]) cout << "ls " << p.first << ' ' << p.second << endl;
//						cout << endl;
						small_block = min(which_block[i], small_block);
						large_block = max(which_block[i], large_block);
					}
				}
				if(small_block == INF) {
					cout << -1 << '\n';
					continue;
				}
				int small_idx = INF, large_idx = -INF;
				for(int i = small_block * SZ; i < min((small_block + 1) * SZ, n); i++) if(a[i] + blanket[small_block] == qq.x) {
						small_idx = min(i, small_idx);
						large_idx = max(i, large_idx);
					}
				for(int i = large_block * SZ; i < min((large_block + 1) * SZ, n); i++) if(a[i] + blanket[large_block] == qq.x) {
						small_idx = min(i, small_idx);
						large_idx = max(i, large_idx);
					}
				cout << large_idx - small_idx << '\n';
			}
		}
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TaskE solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
