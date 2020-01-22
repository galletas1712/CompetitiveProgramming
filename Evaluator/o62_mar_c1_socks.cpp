#include <bits/stdc++.h>
#include "sockslib.h"
using namespace std;
const int N = 2010;

int n, nxt[N], rev_nxt[N];
bool matched[N];
int main() {
  n = num();
  n *= 2;
  nxt[n] = n + 1;
  for(int i = n-1; i >= 1; i--) {
      int l = i, r = n;
      while(l < r) {
        int mid = l+r+1 >> 1;
        vector<int> v;
        for(int j = i; j <= mid; j++) {
            if(!matched[j]) v.push_back(j);
        }
        if(ask(v) == v.size()) l = mid;
        else r = mid-1;
      }
      nxt[i] = l+1;
      if(nxt[i] > n) continue;
      int j = l;
      while(nxt[j] != l+1) --j;
      vector<int> tmp = {i, j};
          match(l + 1, j);
          matched[l + 1] = matched[j] = true;
          nxt[l + 1] = -1;
          nxt[j] = -1;
  }
}
