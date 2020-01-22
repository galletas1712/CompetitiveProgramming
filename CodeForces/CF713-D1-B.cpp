#include <bits/stdc++.h>
using namespace std;

int n;

int query(int x1, int y1, int x2, int y2) {
  if(x1 > x2 || y1 > y2) return 0;
  cout << "? " << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
  int ret;
  cin >> ret;
  return ret;
}

tuple<int,int,int,int> get_xbounds() {
  int l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(1, 1, mid, n) >= 1) r = mid;
    else l = mid+1;
  }
  int left_x2 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(mid, 1, left_x2, n) >= 1) l = mid;
    else r = mid-1;
  }
  int left_x1 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(mid, 1, n, n) >= 1) l = mid;
    else r = mid-1;
  }
  int right_x1 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(right_x1, 1, mid, n) >= 1) r = mid;
    else l = mid+1;
  }
  int right_x2 = l;
  return make_tuple(left_x1, left_x2, right_x1, right_x2);
}

tuple<int,int,int,int> get_ybounds() {
  int l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(1, 1, n, mid) >= 1) r = mid;
    else l = mid+1;
  }
  int left_y2 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(1, mid, n, left_y2) >= 1) l = mid;
    else r = mid-1;
  }
  int left_y1 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(1, mid, n, n) >= 1) l = mid;
    else r = mid-1;
  }
  int right_y1 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(1, right_y1, n, mid) >= 1) r = mid;
    else l = mid+1;
  }
  int right_y2 = l;
  return make_tuple(left_y1, left_y2, right_y1, right_y2);
}

pair<int,int> get_xbounds(int y1, int y2) {
  int l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(1, y1, mid, y2) >= 1) r = mid;
    else l = mid+1;
  }
  int x2 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(mid, y1, x2, y2) >= 1) l = mid;
    else r = mid-1;
  }
  int x1 = l;
  return make_pair(x1, x2);
}

pair<int,int> get_ybounds(int x1, int x2) {
  int l = 1, r = n;
  while(l < r) {
    int mid = l+r >> 1;
    if(query(x1, 1, x2, mid) >= 1) r = mid;
    else l = mid+1;
  }
  int y2 = l;

  l = 1, r = n;
  while(l < r) {
    int mid = l+r+1 >> 1;
    if(query(x1, mid, x2, y2) >= 1) l = mid;
    else r = mid-1;
  }
  int y1 = l;
  return make_pair(y1, y2);
}

int main() {
  ios::sync_with_stdio(false);
  cin >> n;
  int left_y1, left_y2, right_y1, right_y2;
  tie(left_y1, left_y2, right_y1, right_y2) = get_ybounds();
  int left_x1, left_x2, right_x1, right_x2;
  tie(left_x1, left_x2, right_x1, right_x2) = get_xbounds();
  if(left_y1 <= left_y2 && left_y2 < right_y1 && right_y1 <= right_y2) {
    tie(left_x1, left_x2) = get_xbounds(left_y1, left_y2);
    tie(right_x1, right_x2) = get_xbounds(right_y1, right_y2);
  } else {
    tie(left_y1, left_y2) = get_ybounds(left_x1, left_x2);
    tie(right_y1, right_y2) = get_ybounds(right_x1, right_x2);
  }
  cout << "! " << left_x1 << ' ' << left_y1 << ' ' << left_x2 << ' ' << left_y2 << ' ' << right_x1 << ' ' << right_y1 << ' ' << right_x2 << ' ' << right_y2 << endl;
}
