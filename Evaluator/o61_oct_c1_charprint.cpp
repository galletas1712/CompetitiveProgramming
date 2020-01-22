// Use element of that character at the smallest index
// Handle mode 1 using fenwick trees
#include <bits/stdc++.h>
using namespace std;
const int N = 3e5+1;

int n, m, mode;
string s, t;
priority_queue<int, vector<int>, greater<int> > pq[26];

int f[N];
void update(int i, int v) {
	++i;
	while(i < N) f[i] += v, i += i & -i;
}
int query(int i) {
	++i;
	int ret = 0;
	while(i > 0) ret += f[i], i -= i & -i;
	return ret;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> mode >> s >> t;
	n = s.length(), m = t.length();
	for(int i = 0; i < n; i++) {
		pq[s[i] - 'a'].push(i);
		update(i, 1);
	}
	long ans = 0;
	for(int i = 0; i < m; i++) {
		if(pq[t[i] - 'a'].empty()) cout << -1, exit(0);
		ans += query(pq[t[i] - 'a'].top());
		update(pq[t[i] - 'a'].top(), -mode);
		pq[t[i] - 'a'].pop();
	}
	cout << ans;
}
