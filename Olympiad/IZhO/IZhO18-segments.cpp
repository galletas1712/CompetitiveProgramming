/*
 * Count complements: number of segments that have less than k points in common
 * Maintain three cases
 * - len[i] >= k && l[i] > b - k + 1
 * - len[i] >= k && r[i] < a + k - 1
 * - len[i] < k
 * We can either use merge sort tree or sqrt decomposition with sorted lists in each block to maintain first two cases, but the former will MLE
 * Merge sort tree uses n log n memory whereas sqrt decomposition uses sqrt(n) + n memory
 * Third case is easily maintained with a normal sorted list
 *
 * Do sqrt decomposition on queries for rebuilding sorted list and block array, as there as deletions
 * Optimal value magic value is >= 3000
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1, MAGIC = 3000, S = 3000;

class TaskD {

	int n, T, id[N], last[N], blk[N];
	bool valid[N];
	pair<int,int> segs[N];
	vector<pair<int,int> > fl, fr;

//	vector<vector<int> > tl, tr;
//
//	int get_r(int l, int r, int targ_r) {
//		int ret = 0;
//		for(l += fr.size(), r += fr.size() + 1; l < r; l >>= 1, r >>= 1) {
//			if(l & 1) {
//				auto it = lower_bound(tr[l].begin(), tr[l].end(), targ_r);
//				ret += it - tr[l].begin();
//				++l;
//			}
//			if(r & 1) {
//				--r;
//				auto it = lower_bound(tr[r].begin(), tr[r].end(), targ_r);
//				ret += it - tr[r].begin();
//			}
//		}
//		return ret;
//	}
//
//	int get_l(int l, int r, int targ_l) {
//		int ret = 0;
//		for(l += fl.size(), r += fl.size() + 1; l < r; l >>= 1, r >>= 1) {
//			if(l & 1) {
//				auto it = upper_bound(tl[l].begin(), tl[l].end(), targ_l);
//				ret += tl[l].end() - it;
//				++l;
//			}
//			if(r & 1) {
//				--r;
//				auto it = upper_bound(tl[r].begin(), tl[r].end(), targ_l);
//				ret += tl[r].end() - it;
//			}
//		}
//		return ret;
//	}
//
//	void build() {
//		tl.clear();
//		tr.clear();
//		tl.resize(2 * fl.size());
//		tr.resize(2 * fr.size());
//		sort(fl.begin(), fl.end());
//		sort(fr.begin(), fr.end());
//		for(int i = 0; i < fl.size(); i++) tl[fl.size() + i] = {fl[i].second};
//		for(int i = 0; i < fr.size(); i++) tr[fr.size() + i] = {fr[i].second};
//		for(int i = fl.size()-1; i >= 1; i--) {
//			tl[i].resize(tl[i << 1].size() + tl[i << 1 | 1].size());
//			merge(tl[i << 1].begin(), tl[i << 1].end(), tl[i << 1 | 1].begin(), tl[i << 1 | 1].end(),
//			      tl[i].begin());
//		}
//		for(int i = fr.size()-1; i >= 1; i--) {
//			tr[i].resize(tr[i << 1].size() + tr[i << 1 | 1].size());
//			merge(tr[i << 1].begin(), tr[i << 1].end(), tr[i << 1 | 1].begin(), tr[i << 1 | 1].end(),
//			      tr[i].begin());
//		}
//	}

	vector<vector<int> > tl, tr;
	void build() {
		sort(fl.begin(), fl.end());
		sort(fr.begin(), fr.end());
		tl = vector<vector<int> >((fl.size() + S-1) / S);
		tr = vector<vector<int> >((fr.size() + S-1) / S);
		for(int i = 0; i < fl.size(); i++) tl[i / S].push_back(fl[i].second);
		for(int i = 0; i < fr.size(); i++) tr[i / S].push_back(fr[i].second);
		for(auto &v: tl) sort(v.begin(), v.end());
		for(auto &v: tr) sort(v.begin(), v.end());
	}

	int get_l(int l, int r, int targ_l) {
		int lb = (l + S-1) / S, rb = (r+1) / S - 1;
		int ret = 0;
		if(lb > rb) {
			for(int i = l; i <= r; i++) {
				ret += fl[i].second > targ_l;
			}
			return ret;
		}
		for(int i = l; i < lb * S; i++) ret += fl[i].second > targ_l;
		for(int i = lb; i <= rb; i++) ret += tl[i].end() - upper_bound(tl[i].begin(), tl[i].end(), targ_l);
		for(int i = (rb + 1) * S; i <= r; i++) ret += fl[i].second > targ_l;
		return ret;
	}

	int get_r(int l, int r, int targ_r) {
		int lb = (l + S-1) / S, rb = (r+1) / S - 1;
		int ret = 0;
		if(lb > rb) {
			for(int i = l; i <= r; i++) {
				ret += fr[i].second < targ_r;
			}
			return ret;
		}
		for(int i = l; i < lb * S; i++) ret += fr[i].second < targ_r;
		for(int i = lb; i <= rb; i++) ret += lower_bound(tr[i].begin(), tr[i].end(), targ_r) - tr[i].begin();
		for(int i = (rb + 1) * S; i <= r; i++) ret += fr[i].second < targ_r;
		return ret;
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> T;
		int lastans = 0, tick = 0, alive = 0;
		for(int i = 0; i < n; i++) {
			segs[i] = {-1, -1};
			blk[i] = i / MAGIC;
		}
		for(int i = 0, inst; i < n; i++) {
			if(i % MAGIC == 0) {
				fl.clear();
				fr.clear();
				for(int j = 0; j < i; j++) {
					if(!valid[id[j]]) continue;
					int len = segs[id[j]].second - segs[id[j]].first + 1;
					fl.emplace_back(len, segs[id[j]].first);
					fr.emplace_back(len, segs[id[j]].second);
				}
				build();
			}
			cin >> inst;
			if(inst == 1) {
				int l, r;
				cin >> l >> r;
				l ^= T * lastans;
				r ^= T * lastans;
				if(l > r) swap(l, r);
				id[i] = ++tick;
				valid[id[i]] = true;
				last[id[i]] = i;
				segs[id[i]] = {l, r};
				++alive;
			} else if(inst == 2) {
				int x;
				cin >> x;
				id[i] = x;
				valid[id[i]] = false;
				--alive;
			} else {
				int l, r, k;
				cin >> l >> r >> k;
				l ^= T * lastans;
				r ^= T * lastans;
				if(l > r) swap(l, r);
				lastans = 0;
				if(r-l+1 < k) {
					cout << lastans << '\n';
					continue;
				}
				// outside block -> three cases
				int idx = lower_bound(fl.begin(), fl.end(), make_pair(k, 0)) - fl.begin();
				lastans += idx; // len < k
				lastans += get_l(idx, fl.size() - 1, r - k + 1); // l[j] > r[i] - k[i] + 1 and len >= k
				lastans += get_r(idx, fr.size() - 1, l + k - 1); // r[j] < l[i] + k[i] - 1 and len >= k
				// within same block
				for(int j = i-1; j >= 0 && blk[j] == blk[i]; j--) {
					if(valid[id[j]]) {
						lastans += min(segs[id[j]].second, r) - max(segs[id[j]].first, l) + 1 < k;
					} else if(segs[id[j]].first != -1 && blk[last[id[j]]] < blk[i]) {
						lastans -= min(segs[id[j]].second, r) - max(segs[id[j]].first, l) + 1 < k;
					}
				}
				lastans = alive - lastans;
				cout << lastans << '\n';
			}
		}
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
