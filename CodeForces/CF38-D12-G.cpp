/*
Binary search for the leftmost index where the suffix maximum is less than a[i]
Use treap to maintain interval maximums in order to support insert oprations
*/
#include <bits/stdc++.h>
using namespace std;

struct node {
  int cnt, prior;
  int val, mx, rightmost;
  node *l, *r;
  node(int val): cnt(1), prior(rand()), val(val), mx(val), rightmost(val), l(NULL), r(NULL) {}
};

typedef node* pnode;

void update(pnode t) {
  if(!t) return;
  t->cnt = (t->l ? t->l->cnt : 0) + (t->r ? t->r->cnt : 0) + 1;
  if(t->r) t->rightmost = t->r->rightmost;
  else t->rightmost = t->val;
  t->mx = t->val;
  if(t->l) t->mx = max(t->l->mx, t->mx);
  if(t->r) t->mx = max(t->r->mx, t->mx);
}

void split(pnode t, pnode &l, pnode &r, int targ, int add = 0) {
  if(!t) return void(l = r = NULL);
  int curr_key = add + 1 + (t->l ? t->l->cnt : 0);
  if(targ < curr_key) split(t->l, l, t->l, targ, add), r = t;
  else split(t->r, t->r, r, targ, curr_key), l = t;
  update(t);
}

void merge(pnode &t, pnode l, pnode r) {
  if(!l || !r) return void(t = (l ? l : r));
  if(l->prior > r->prior) merge(l->r, l->r, r), t = l;
  else merge(r->l, l, r->l), t = r;
  update(t);
}

int n, rev[100001];
pnode root;

void answer(pnode t) {
  if(!t) return;
  answer(t->l);
  cout << rev[t->val] << ' ';
  answer(t->r);
}

int main() {
  srand(time(NULL));
  ios::sync_with_stdio(false);
  cin.tie(0);
  cin >> n;
  for(int i = 1, a, c; i <= n; i++) {
    cin >> a >> c;
    rev[a] = i;
    int l = max(i-c, 1), r = i;
    pnode p1, p2;
    while(l < r) {
      int mid = (l+r) >> 1;
      split(root, p1, p2, mid-1);
      if(p2->mx < a) r = mid;
      else l = mid+1;
      merge(root, p1, p2);
    }
    split(root, p1, p2, l-1);
    merge(root, p1, new node(a));
    merge(root, root, p2);
  }
  answer(root);
}
