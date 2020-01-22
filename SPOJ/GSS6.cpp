// Classical treap. Main thing is that we have to update from top-down in addition from the usual bottom-up.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class GSS6 {

	struct node {
		int prior, cnt, val, sum, mxpref, mxsuff, agg;
		node *l, *r;
		node(int init): prior(rand()), cnt(1), mxpref(0), mxsuff(0), val(init), sum(init), agg(init), l(NULL), r(NULL) {}
		// can only create unit nodes
	};

	typedef node* pnode;

	void update(pnode t) {
		if(!t) return;
		t->cnt = (t->l ? t->l->cnt : 0) + (t->r ? t->r->cnt : 0) + 1;
		t->sum = (t->l ? t->l->sum : 0) + (t->r ? t->r->sum : 0) + t->val;
		t->mxpref = max((t->l ? t->l->mxpref : 0), (t->l ? t->l->sum : 0) + (t->r ? t->r->mxpref : 0) + t->val);
		t->mxsuff = max((t->r ? t->r->mxsuff : 0), (t->r ? t->r->sum : 0) + (t->l ? t->l->mxsuff : 0) + t->val);
		t->agg = (t->l ? t->l->mxsuff : 0) + (t->r ? t->r->mxpref : 0) + t->val;
		t->agg = max((t->l ? t->l->agg : -INF), t->agg);
		t->agg = max((t->r ? t->r->agg : -INF), t->agg);
	}

	void split(pnode t, pnode &l, pnode &r, int targ, int add) {
		if(!t) return void(l = r = NULL);
		update(t);
		int key = add + (t->l ? t->l->cnt : 0) + 1;
		if(targ < key) split(t->l, l, t->l, targ, add), r = t;
		else split(t->r, t->r, r, targ, key), l = t;
		update(t);
	}

	void merge(pnode &t, pnode l, pnode r) {
		if(!l || !r) return void(t = (l ? l : r));
		update(l), update(r);
		if(l->prior > r->prior) merge(l->r, l->r, r), t = l;
		else merge(r->l, l, r->l), t = r;
		update(t);
	}

	void insert(pnode &t, int idx, int val) {
		pnode l, r;
		split(t, l, r, idx - 1, 0);
		merge(t, l, new node(val));
		merge(t, t, r);
	}

	void del(pnode &t, int x) {
		pnode p1, p2, p3;
		split(t, p2, p3, x, 0);
		split(p2, p1, p2, x-1, 0);
		merge(t, p1, p3);
		rec_del(p2);
	}

	void rec_del(pnode t) {
		if(t->l) rec_del(t->l);
		if(t->r) rec_del(t->r);
		delete t;
	}

	void print(pnode t, ostream& cout, int add) {
		if(!t) return;
		cout << add + (t->l ? t->l->cnt : 0) + 1 << ' ' << t->val << ' ' << t->sum << ' ' << t->mxpref << ' ' << t->mxsuff << ' ' << t->agg << endl;
		print(t->l, cout, add);
		print(t->r, cout, add + (t->l ? t->l->cnt : 0) + 1);
	}

	int n, q;
	pnode root;

public:
	void run(istream &cin, ostream &cout) {
		srand(time(NULL));
		cin >> n;
		for(int i = 1, x; i <= n; i++) {
			cin >> x;
			insert(root, i, x);
		}
		cin >> q;
		while(q--) {
			char inst;
			cin >> inst;
			if(inst == 'I') {
				int x, y;
				cin >> x >> y;
				insert(root, x, y);
			} else if(inst == 'D') {
				int x;
				cin >> x;
				del(root, x);
			} else if(inst == 'R') {
				int x, y;
				cin >> x >> y;
				del(root, x);
				insert(root, x, y);
			} else {
				int x, y;
				cin >> x >> y;
				pnode p1, p2, p3;
				split(root, p2, p3, y, 0);
				split(p2, p1, p2, x-1, 0);
				assert(p2);
				cout << p2->agg << '\n';
//				print(p2, cout, 0);
//				cout << endl;
				merge(root, p1, p2);
				merge(root, root, p3);
			}
		}
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    GSS6 *obj = new GSS6();
		obj->run(in, out);
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
