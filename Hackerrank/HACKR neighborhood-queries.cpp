/*
Use a centroid tree as per usual in radius problems and find intersection to cut out using aggregates on direct parent.
No need to be fancy about order statistics here, just binary search for it.
Using regular PST will yield O(n log^3 n), as we need three logs: bsearch, centroid tree parent traversal and PST query.
We can merge the bsearch and PST query into one log, but not in the classical way: we need to do them in parallel
with each parent traversal.
This yields a O(n log^2 n) solution, which ACs.
Note: using vector for PST will MLE. Use array instead.
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 5e4 + 1;

struct PST {
	struct node {
		int val, nxtl, nxtr;
		node() { val = 0, nxtl = -1, nxtr = -1; }
		node(int val, int nxtl, int nxtr): val(val), nxtl(nxtl), nxtr(nxtr) {};
	};
	node t[(int) 2.6e7 + 100];
	int root, tick = -1;
	PST(int n) {
		root = build(0, n-1);
	}
	int newLeaf(int v) {
		t[++tick] = node(v, -1, -1);
		return tick;
	}
	int newPar(int nxtl, int nxtr) {
		t[++tick] = node(t[nxtl].val + t[nxtr].val, nxtl, nxtr);
		return tick;
	}
	int build(int l, int r) {
		if(l == r) return newLeaf(0);
		int mid = l+r >> 1;
		return newPar(build(l, mid), build(mid+1, r));
	}
	int update(int i, int targ, int v, int l, int r) {
		if(l == r) return newLeaf(t[i].val + v);
		int mid = l+r >> 1;
		if(targ <= mid) return newPar(update(t[i].nxtl, targ, v, l, mid), t[i].nxtr); // no need to dynamically create because already built
		else return newPar(t[i].nxtl, update(t[i].nxtr, targ, v, mid+1, r));
	}
	int query(int i, int tl, int tr, int l, int r) {
		if(r < tl || l > tr) return 0;
		if(tl <= l && r <= tr) return t[i].val;
		int mid = l+r >> 1;
		return query(t[i].nxtl, tl, tr, l, mid) + query(t[i].nxtr, tl, tr, mid+1, r);
	}
};

class NQ {

	int n, q, a[N], k[N];
	vector<int> g[N], cc_vals;
	vector<pair<int,int> > lsq[N];
	vector<pair<int,int> > qrt[N];

	void compress() {
		for(int i = 0; i < n; i++) cc_vals.push_back(a[i]);
		sort(cc_vals.begin(), cc_vals.end());
		cc_vals.resize(unique(cc_vals.begin(), cc_vals.end()) - cc_vals.begin());
		unordered_map<int, int> mp;
		for(int i = 0; i < cc_vals.size(); i++) mp[cc_vals[i]] = i;
		for(int i = 0; i < n; i++) a[i] = mp[a[i]];
	}

	int sz[N];
	bool blocked[N];
	void find_centroid(int u, int p, int tot, pair<int,int>& ret) {
		sz[u] = 1;
		int mx = 0;
		for(int v: g[u]) if(v != p && !blocked[v]) {
			find_centroid(v, u, tot, ret);
			sz[u] += sz[v];
			mx = max(sz[v], mx);
		}
		mx = max(tot - sz[u], mx);
		ret = min(make_pair(mx, u), ret);
	}

	int d_own[N], d_par[N];
	vector<pair<int,int> > vals_own, vals_par;
	vector<int> pst_roots_own, pst_roots_par;
	PST *pst;

	void dfs_own(int u, int p) {
		vals_own.emplace_back(d_own[u], a[u]);
		for(int v: g[u]) if(v != p && !blocked[v]) {
			d_own[v] = d_own[u] + 1;
			dfs_own(v, u);
		}
	}

	void dfs_par(int u, int p) {
		vals_par.emplace_back(d_par[u], a[u]);
		for(int v: g[u]) if(v != p && !blocked[v]) {
			d_par[v] = d_par[u] + 1;
			dfs_par(v, u);
		}
	}

	void dfs_query(int u, int p, int root, int par) {
		for(auto tt: lsq[u]) {
			int d, i;
			tie(d, i) = tt;
			auto it = upper_bound(vals_own.begin(), vals_own.end(), make_pair(d - d_own[u], INF));
			if(it != vals_own.begin()) {
				qrt[i].emplace_back(pst_roots_own[it - vals_own.begin()], 1);
			}
			if(par != -1) {
				it = upper_bound(vals_par.begin(), vals_par.end(), make_pair(d - d_par[u], INF));
				if(it != vals_par.begin()) {
					qrt[i].emplace_back(pst_roots_par[it - vals_par.begin()], -1);
				}
			}
		}
		for(int v: g[u]) if(v != p && !blocked[v]) dfs_query(v, u, root, par);
	}

	void dec(int src, int tot, int par) {
		pair<int,int> tmp = {tot, -1};
		find_centroid(src, -1, tot, tmp);
		int cen = tmp.second;

		vals_own.clear();
		vals_par.clear();
		pst_roots_own.clear();
		pst_roots_par.clear();

		d_own[cen] = 0;
		dfs_own(cen, -1);
		sort(vals_own.begin(), vals_own.end());
		pst_roots_own.push_back(pst->root);
		for(auto p: vals_own) {
			pst_roots_own.push_back(pst->update(pst_roots_own.back(), p.second, 1, 0, cc_vals.size() - 1));
		}

		if(par != -1) {
			d_par[src] = 1;
			dfs_par(src, -1);
			sort(vals_par.begin(), vals_par.end());
			pst_roots_par.push_back(pst->root);
			for(auto p: vals_par) {
				pst_roots_par.push_back(pst->update(pst_roots_par.back(), p.second, 1, 0, cc_vals.size() - 1));
			}
		}

		dfs_query(cen, -1, cen, par);

		blocked[cen] = true;
		for(int v: g[cen]) if(!blocked[v])
			dec(v, (sz[v] > sz[cen] ? tot - sz[cen] : sz[v]), cen);
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n;
		for(int i = 0; i < n; i++) cin >> a[i];
		for(int i = 1, u, v; i < n; i++) {
			cin >> u >> v;
			--u, --v;
			g[u].push_back(v);
			g[v].push_back(u);
		}
		compress();
		cin >> q;
		for(int i = 0, u, d; i < q; i++) {
			cin >> u >> d >> k[i];
			--u;
			lsq[u].emplace_back(d, i);
		}
		pst = new PST(cc_vals.size());
		dec(0, n, -1);

		for(int i = 0; i < q; i++) {// doing strict binary search
			int s = 0, e = cc_vals.size() - 1;
			while(s < e) {
				int mid = (s+e) >> 1;
				int sum = 0, positive = 0, negative = 0;
				for(auto p: qrt[i]) {
					int pos, tp;
					tie(pos, tp) = p;
					assert(pos != -1 && pst->t[pos].nxtl != -1);
					if(pos != -1 && pst->t[pos].nxtl != -1) {
						sum += tp * pst->t[pst->t[pos].nxtl].val;
					}
				}
				if(k[i] <= sum) {
					for(auto &p: qrt[i]) if(p.first != -1) p.first = pst->t[p.first].nxtl;
					e = mid;
				} else {
					k[i] -= sum;
					for(auto &p: qrt[i]) if(p.first != -1) p.first = pst->t[p.first].nxtr;
					s = mid + 1;
				}
			}
			int sum = 0;
			for(auto p: qrt[i]) if(p.first != -1) sum += p.second * pst->t[p.first].val;
			if(k[i] > sum) cout << -1 << '\n';
			else cout << cc_vals[s] << '\n';
		}
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    NQ *obj = new NQ();
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
