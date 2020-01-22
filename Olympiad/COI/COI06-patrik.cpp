// sort in decreasing order of height and binary search for the left and right bounds. Update and count 
// intervals using fenwick tree
#include <bits/stdc++.h>
using namespace std;
const int N = 1 << 19;

int n, f[N], f2[N];
vector<pair<int,int> > ls;
long ans;

void update(int i) { while(i < N) ++f[i], i += i & -i; }
int query(int i) {
	int ret = 0;
	while(i > 0) ret += f[i], i -= i & -i;
	return ret;
}
int bsearch(int k) {
	int sum = 0, i = 0;
	for(int j = 18; j >= 0; j--) {
		if(sum + f[i + (1 << j)] <= k) {
			sum += f[i + (1 << j)];
			i += 1 << j;
		}
	}
	return i;
}
void update2(int i) { while(i < N) ++f2[i], i += i & -i; }
int query2(int i) {
	int ret = 0;
	while(i > 0) ret += f2[i], i -= i & -i;
	return ret;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n;
	ls.reserve(n);
	for(int i = 1, x; i <= n; i++) {
		cin >> x;
		ls.emplace_back(x, i);
	}
	sort(ls.begin(), ls.end(), greater<pair<int,int>>());
	for(int i = 0, j = 0; i < n; i = j) {
		j = i;
		for(; j < n && ls[j].first == ls[i].first; j++) {
			int idx = ls[j].second;	
			int lb, rb = min(bsearch(query(idx - 1)) + 1, n);
			int qidx = query(idx);
			int l = 1, r = idx;
			while(l < r) {
				int mid = l+r >> 1;
				if(qidx - query(mid - 1)) l = mid+1;
				else r = mid;
			}
			lb = max(l - 1, 1);
			ans += query2(rb) - query2(lb - 1);
			update2(idx);
			// cout << idx << ' ' << lb << ' ' << rb << ' ' << endl;
		}
		j = i;
		for(; j < n && ls[j].first == ls[i].first; j++) {
			update(ls[j].second);
		}
	}
	cout << ans;
}
