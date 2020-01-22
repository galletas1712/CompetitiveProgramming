/*
Divide and conquer
At each node in the segment tree, store the index with maximum value
To maintain this, we only need to make comparisons at non-leaf nodes which will be around n-1 at worst case
To search for second maximum, we binary search on the segments. At each node, we compare the recursively computed second maximum in the child with the global maximum
and the maximum in the child without the maximum (as it will always be less than the maximum)
So, we do one comparison per level, resulting in O(log n) time

Number of comparisons is n + log(n), which should fit in n + 24
*/
#include <bits/stdc++.h>
using namespace std;

int query_cnt;
int query(int i, int j) {
  if(i == 0) return -1;
  if(j == 0) return 1;
  ++query_cnt;
  cout << "? " << i << ' ' << j << endl;
  string res;
  cin >> res;
  assert(res != "=");
  if(res == "<") return -1;
  else return 1;
}

int f[1 << 14];

void rec1(int i, int l, int r) {
  if(l == r) {
    f[i] = l;
    return;
  }
  int mid = l+r >> 1;
  rec1(i << 1, l, mid);
  rec1(i << 1 | 1, mid+1, r);
  f[i] = (query(f[i << 1], f[i << 1 | 1]) == 1 ? f[i << 1] : f[i << 1 | 1]);
}

//f[1] = maximum
int rec2(int i, int l, int r) {
  if(l == r) {
    if(l == f[1]) return 0;
    else return f[i];
  }
  int mid = l+r >> 1;
  int res_left, res_right;
  if(l <= f[1] && f[1] <= mid) {
    res_left = rec2(i << 1, l, mid);
    res_right = f[i << 1 | 1];
  } else {
    res_left = f[i << 1];
    res_right = rec2(i << 1 | 1, mid+1, r);
  }
  return (query(res_left, res_right) == 1 ? res_left : res_right);
}

int main() {
  int n;
  cin >> n;
  rec1(1, 1, n);
  int ans = rec2(1, 1, n);
  cout << "! " << ans << endl;
  assert(query_cnt <= n + 24);
}
