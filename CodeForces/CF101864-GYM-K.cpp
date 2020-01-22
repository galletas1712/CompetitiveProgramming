// if Y is negative, insert one by one and deep delete the other
// if Y is positive, we can do lazy explicit treap
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#define long long long
using namespace std;

struct item {
	long key, prior;
	long val, lz;
	item *l, *r;
	item(long key, long val): key(key), prior(rand()), val(val), lz(0), l(NULL), r(NULL) {}
};
typedef item* pitem;
pitem root;

void apply(pitem t) {
	if(!t) return;
	t->key += t->lz;
	if(t->l) t->l->lz += t->lz;
	if(t->r) t->r->lz += t->lz;
	t->lz = 0;
}

void split(pitem t, pitem &l, pitem &r, long v) {
	apply(t);
	if(!t) return void(l = r = NULL);
	if(t->key <= v) split(t->r, t->r, r, v), l = t;
	else split(t->l, l, t->l, v), r = t;
}

void merge(pitem &t, pitem l, pitem r) {
	apply(l), apply(r);
	if(!l || !r) return void(t = l ? l : r);
	if(l->prior > r->prior) merge(l->r, l->r, r), t = l;
	else merge(r->l, l, r->l), t = r;
}

void add(pitem &t, pitem x) {
	apply(t);
	if(!t) t = x;
	else if(x->prior > t->prior) split(t, x->l, x->r, x->key), t = x;
	else if(x->key < t->key) add(t->l, x);
	else add(t->r, x);
}


void fill_res(pitem t, vector<pair<long,long> >& res) {
	apply(t);
	if(!t) return;
	res.emplace_back(t->val, t->key);
	fill_res(t->l, res);
	fill_res(t->r, res);
}

void print_answer(pitem root) {
	apply(root);
		vector<pair<long,long> > res;
		fill_res(root, res);
		sort(res.begin(), res.end());
		for(auto p: res) cout << p.second << ' ';
		cout << endl;
}

void apply_all(pitem t, pitem &ins) {
	if(!t) return;
	apply(t);
	add(ins, new item(t->key, t->val));
	apply_all(t->l, ins);
	apply_all(t->r, ins);
}

void deep_delete(pitem t) {
	if(!t) return;
	deep_delete(t->l);
	deep_delete(t->r);
	delete t;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	srand(time(NULL));
	int t;
	cin >> t;
	while(t--) {
		int n, q;
		cin >> n >> q;
		for(int i = 0, x; i < n; i++) {
			cin >> x;
			add(root, new item(x, i));
		}
		for(int i = 0, tp, x, y; i < q; i++) {
			cin >> tp >> x >> y;
				if(tp == 1) {
					pitem r1, r2;
					split(root, r1, r2, x);
					if(r1) {
						r1->lz -= y;
						if(y < 0) {
							apply_all(r1, r2);
							deep_delete(r1);
							root = r2;
						} else merge(root, r1, r2);
					} else merge(root, r1, r2);
				} else {
					pitem r1, r2;
					split(root, r1, r2, x-1);
					if(r2) {
						r2->lz += y;
						if(y < 0) {
							apply_all(r2, r1);
							deep_delete(r2);
							root = r1;
						} else merge(root, r1, r2);
					} else merge(root, r1, r2);
				}
		}
		print_answer(root);
		deep_delete(root);
		root = NULL;
	}
}
