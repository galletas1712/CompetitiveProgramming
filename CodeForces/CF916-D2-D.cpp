// Persistent segment tree, but persistent trie can also be used to determine which value of x a certain string currently has
// Implementations with persistent trie suffer increased memory complexity and extra implementation, so not recommended
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1, MAX = 1e9;


struct pst {
	struct node {
		int v, nxtl, nxtr;
		node(int v, int nxtl, int nxtr): v(v), nxtl(nxtl), nxtr(nxtr) {}
	};
	vector<node> t;
	int root[N];
	int newLeaf(int v) {
		t.emplace_back(v, -1, -1);
		return t.size()-1;
	}
	int newPar(int l, int r) {
		if(l == -1) t.emplace_back(t[r].v, l, r);
		else if(r == -1) t.emplace_back(t[l].v, l, r);
		else t.emplace_back(t[l].v + t[r].v, l, r);
		return t.size() - 1;
	}
	int update(int i, int targ, int v, int l = 0, int r = MAX) {
		if(l == r) {
			if(i == -1) return newLeaf(v);
			else return newLeaf(t[i].v + v);
		}
		int mid = l+r >> 1;
		int nxtl = i == -1 || t[i].nxtl == -1 ? -1 : t[i].nxtl;
		int nxtr = i == -1 || t[i].nxtr == -1 ? -1 : t[i].nxtr;
		if(targ <= mid) return newPar(update(nxtl, targ, v, l, mid), nxtr);
		else return newPar(nxtl, update(nxtr, targ, v, mid+1, r));
	}
	int query(int i, int tl, int tr, int l = 0, int r = MAX) {
		if(tr < l || r < tl) return 0;
		if(tl <= l && r <= tr) {
			return t[i].v;
		}
		int mid = l+r >> 1;
		int res = 0;
		if(t[i].nxtl != -1) res += query(t[i].nxtl, tl, tr, l, mid);
		if(t[i].nxtr != -1) res += query(t[i].nxtr, tl, tr, mid+1, r);
		return res;
	}
} cnt, mark;

class TaskD {
	int q, tick = 0;
	int state[N];
	map<string, int> idx;

public:
	void solve(istream& cin, ostream& cout) {
		cin >> q;
		state[0] = 0;
		mark.root[0] = cnt.root[0] = 0;
		mark.t.emplace_back(0, -1, -1);
		cnt.t.emplace_back(0, -1, -1);
		for(int z = 1; z <= q; z++) {
			string inst;
			cin >> inst;

			if(inst == "set") {
				string a;
				int x;
				cin >> a >> x;
				if(!idx.count(a)) idx[a] = idx.size() + 1;
				int curr_x = mark.query(mark.root[state[z-1]], idx[a], idx[a]);
				state[z] = ++tick;
				if(curr_x != 0) {
					cnt.root[state[z]] = cnt.update(cnt.root[state[z-1]], curr_x, -1);
					mark.root[state[z]] = mark.update(mark.root[state[z-1]], idx[a], -curr_x);
				} else {
					cnt.root[state[z]] = cnt.root[state[z-1]];
					mark.root[state[z]] = mark.root[state[z-1]];
				}
				cnt.root[state[z]] = cnt.update(cnt.root[state[z]], x, 1);
				mark.root[state[z]] = mark.update(mark.root[state[z]], idx[a], x);
			} else if(inst == "remove") {
				string a;
				cin >> a;
				if(!idx.count(a)) idx[a] = idx.size() + 1;
				int curr_x = mark.query(mark.root[state[z-1]], idx[a], idx[a]);
				if(curr_x == 0) {
					state[z] = state[z-1];
					continue;
				}
				state[z] = ++tick;
				cnt.root[state[z]] = cnt.update(cnt.root[state[z-1]], curr_x, -1);
				mark.root[state[z]] = mark.update(mark.root[state[z-1]], idx[a], -curr_x);
			} else if(inst == "query") {
				string a;
				cin >> a;
				if(!idx.count(a)) idx[a] = idx.size() + 1;
				state[z] = state[z-1];
				int curr_x = mark.query(mark.root[state[z-1]], idx[a], idx[a]);
				if(curr_x == 0) cout << -1 << endl;
				else cout << cnt.query(cnt.root[state[z]], 0, curr_x-1) << endl;
			} else {
				int d;
				cin >> d;
				state[z] = state[z-d-1]; // check off by one
			}
		}
	}
};



int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	TaskD solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
