// General idea is to use many trees (represented as bitmask). To access these trees, run through a filter. Provable that this whole system has <= n + log n nodes (canceling).
#include "doll.h"
#include <bits/stdc++.h>
using namespace std;
const int N = 4e5+1;

int m, n, need, nodes, c[N], l[N], r[N], x[N], y[N];
vector<int> a;
vector<int> ls[N];

bool state[N], leaf_l[N], leaf_r[N];

void dfs(int i, int val) {
  bool org = state[i];
  state[i] ^= 1;
  if(org) {
    if(r[i] == -1) {
      // cout << i << " r" << ' ' << val << endl;
      y[i] = val;
    } else dfs(r[i], val);
  } else {
    if(l[i] == -1) {
      // cout << i << " l" << ' ' << val << endl;
      x[i] = val;
    } else dfs(l[i], val);
  }
}

void create_circuit(int _m, vector<int> _a) {
  m = _m, a = _a;
  n = a.size();
  a.insert(a.begin(), 0);
  a.push_back(0);

  for(int i = 0; i < (int) a.size() - 1; i++) ls[a[i]].push_back(a[i+1]);

  for(int i = 0; i <= m; i++) {
    if(ls[i].empty()) c[i] = 1;
    else if(ls[i].size() == 1) c[i] = ls[i][0];
    else c[i] = -1;
  }

  for(int i = 0; i < (int) a.size() - 1; i++) need += ls[a[i]].size() > 1;

  int lgn = log2(need);

  for(int i = 1; i <= lgn + 1; i++) {
    if(i <= lgn) l[i] = i+1, x[i] = -l[i];
    else {
      l[i] = 1;
      r[i] = (need % 2 ? -1 : 1);
      leaf_l[i] = leaf_r[i] = 1;
      x[i] = y[i] = -1;
    }
  }
  nodes += log2(need) + 1;
  for(int i = 1; i <= lgn; i++) {
    int sz = 1 << (lgn + 1 - i);
    if(need & sz) {
      r[i] = nodes + 1;
      y[i] = -r[i];
      for(int t = 1; t < sz; t++) {
        if(t < sz / 2) {
          l[nodes + t] = nodes + (t << 1);
          r[nodes + t] = nodes + (t << 1 | 1);
          x[nodes + t] = -l[nodes + t];
          y[nodes + t] = -r[nodes + t];
        } else {
          leaf_l[nodes + t] = leaf_r[nodes + t] = 1;
          l[nodes + t] = r[nodes + t] = -1;
        }
      }
      nodes += sz - 1;
    } else {
      r[i] = 1;
      y[i] = -1;
      leaf_r[i] = 1;
    }
    // cout << i << ' ' << nodes << ' ' << sz << ' ' << l[i] << ' ' << leaf_l[i] << ' ' << r[i] << ' ' << leaf_r[i] << endl;
  }
  // for(int i = 1; i <= nodes; i++) {
  //   cout << i << ' ' << l[i] << ' ' << r[i] << ' ' << leaf_l[i] << ' ' << leaf_r[i] << endl;
  // }
  //
  for(int i = 0; i < (int) a.size() - 1; i++) {
    if(ls[a[i]].size() <= 1) continue;
    dfs(1, a[i+1]);
  }
  vector<int> C, X, Y;
  for(int i = 0; i <= m; i++) C.push_back(c[i]);
  for(int i = 1; i <= nodes; i++) X.push_back(x[i]), Y.push_back(y[i]);

  // for(int i = 1; i <= nodes; i++) cout << -i << ' ' << x[i] << ' ' << y[i] << endl;

  answer(C, X, Y);
}
