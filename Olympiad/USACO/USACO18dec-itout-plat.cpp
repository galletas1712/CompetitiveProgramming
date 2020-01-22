// Problem reduces to finding the k-th lexicographically smallest complement of an LIS
// This further reduces to finding the k-th lexicographically largest LIS (prove using induction, I got this lemma as a hint)
// Determine length of LIS starting at each node -> store in dp[i]
// Determine the number of LIS's starting at that node -> store in f[i] (use DST to calculate, as we have three conditions to satisfy here)
// PST will MLE!!!
// Since we have the property that the LIS at each element in our set must be strictly decreasing by one, we don't have to binary search
// This makes our final step fairly trivial :)
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1 << 17;

struct ST {
	 int t[2*N];
	 void update(int i, int v) {
	 	for(t[i += N] = v; i > 1; i >>= 1) t[i>>1] = max(t[i], t[i^1]);
	 }
	 int query(int l, int r) {
	 	int res = 0;
	 	for(l += N, r += N+1; l < r; l >>= 1, r >>= 1) {
	 		if(l & 1) res = max(t[l++], res);
	 		if(r & 1) res = max(t[--r], res);
	 	}
	 	return res;
	 }
};

//struct node {
//	long v;
//	int nxtl, nxtr;
//	node() { v = 0, nxtl = nxtr = -1; }
//	node(long v, int nxtl, int nxtr): v(v), nxtl(nxtl), nxtr(nxtr) {}
//};
//
//struct DST {
//	vector<node> t;
//	int root;
//	DST() { t.push_back(node()); root = 0; }
//	int newLeaf(long v) {
//		t.push_back(node(v, -1, -1));
//		return t.size() - 1;
//	}
//	int newPar(int l, int r) {
//		t.push_back(node(min((l != -1 ? t[l].v : 0) + (r != -1 ? t[r].v : 0), LINF), l, r));
//		return t.size() - 1;
//	}
//	void update(int targ, int v) {
//		root = update(targ, v, root, 0, N-1);
//	}
//	int update(int targ, long v, int i, int l, int r) { // increment operation
////		cout << "otw " << targ << ' ' << v << ' ' << i << ' ' << l << ' ' << r << endl;
//		if(l == r) {
////			cout << "leaf " << t[i].v << ' ' << v << endl;
//			return newLeaf(min(t[i].v + v, LINF));
//		}
//		int mid = l+r >> 1;
//		if(targ <= mid) {
//			if(t[i].nxtl == -1) t[i].nxtl = newLeaf(0);
//			return newPar(update(targ, v, t[i].nxtl, l, mid), t[i].nxtr);
//		} else {
//			if(t[i].nxtr == -1) t[i].nxtr = newLeaf(0);
//			return newPar(t[i].nxtl, update(targ, v, t[i].nxtr, mid+1, r));
//		}
//	}
//	long query(int tl, int tr) {
//		return query(tl, tr, root, 0, N-1);
//	}
//	long query(int tl, int tr, int i, int l, int r) {
////		cout << "query_otw " << tl << ' ' << tr << ' ' << i << ' ' << l << ' ' << r << endl;
//		if(i == -1 || r < tl || l > tr) return 0;
//		if(tl <= l && r <= tr) return t[i].v;
//		int mid = l+r >> 1;
//		return min(query(tl, tr, t[i].nxtl, l, mid) + query(tl, tr, t[i].nxtr, mid+1, r), LINF);
//	}
//};

struct DST {
	int l, r;
	long v;
	DST *nxtl, *nxtr;

	DST():nxtl(NULL), nxtr(NULL), v(0), l(-1), r(-1) {};
	DST(int l, int r): nxtl(NULL), nxtr(NULL), v(0), l(l), r(r) {}
	void init(int l, int r) {
		this->l = l;
		this->r = r;
	}

	void elongate() {
		if(!nxtl) {
			nxtl = new DST();
			nxtr = new DST();
			int mid = l+r >> 1;
			nxtl->init(l, mid);
			nxtr->init(mid+1, r);
		}
	}

	void update(int targ, long v) {
		if(l == r) {
			this->v += v;
			this->v = min(LINF, this->v);
			return;
		}
		elongate();
		int mid = l+r >> 1;
		if(targ <= mid) nxtl->update(targ, v);
		else nxtr->update(targ, v);
		this->v = min(nxtl->v + nxtr->v, LINF);
	}

	long query(int tl, int tr) {
		if(r < tl || l > tr) return 0;
		if(tl <= l && r <= tr) return this->v;
		elongate();
		int mid = l+r >> 1;
		return min(nxtl->query(tl, tr) + nxtr->query(tl, tr), LINF);
	}
};

class itout {
	int n, a[N], rev[N], dp[N];
	long k, f[N];
	DST *dst[N];
	ST *st;
	vector<pair<int, long> > ls[N];

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> k;
		for(int i = 1; i <= n; i++) {
			cin >> a[i];
			rev[a[i]] = i;
		}

		st = new ST();
		for(int i = n; i >= 1; i--) {
			dp[i] = st->query(a[i], n) + 1;
			st->update(a[i], dp[i]); // permutation -> guaranteed never to update any cell more than once
		}
		delete st;

		for(int i = 1; i <= n; i++) dst[i] = new DST(1, n);
		for(int i = n; i >= 1; i--) {
			f[i] = (dp[i] != 1 ? dst[dp[i] - 1]->query(a[i], n) : 1);
			dst[dp[i]]->update(a[i], f[i]);
			ls[dp[i]].emplace_back(i, f[i]);
		}
		for(int i = 1; i <= n; i++) delete dst[i];

		vector<int> res;
		int begin_index = 1;
		for(int len = n; len >= 1; len--) if(!ls[len].empty()) {
			vector<pair<int, long> > tmp;
			for(auto p: ls[len]) {
				if(p.first >= begin_index) tmp.push_back(p);
			}
			ls[len] = tmp;
			sort(ls[len].begin(), ls[len].end(), [&] (pair<int, long> x, pair<int, long> y) { return a[x.first] < a[y.first]; });

			long cnt = 0;
			while(cnt + ls[len].back().second < k) {
				cnt += ls[len].back().second;
				ls[len].pop_back();
			}
			res.push_back(a[ls[len].back().first]);
			k -= cnt;
			begin_index = ls[len].back().first + 1;
		}
		sort(res.begin(), res.end());

		cout << n - res.size() << '\n';
		for(int i = 1; i <= n; i++) {
			if(!binary_search(res.begin(), res.end(), i)) cout << i << '\n';
		}
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    itout *obj = new itout();
		obj->solve(in, out);
		delete obj;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Solver solver;
	std::ifstream in("itout.in");
	std::ofstream out("itout.out");
	solver.solve(in, out);
	return 0;
}
