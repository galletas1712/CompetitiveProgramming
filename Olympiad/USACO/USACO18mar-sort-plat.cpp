#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1;

int n, a[N], part[N];
pair<int,int> b[N];
long long ans;
int main() {
	freopen("sort.in", "r", stdin);
	freopen("sort.out", "w", stdout);
	cin.tie(0);
	cin >> n;
	for(int i = 1; i <= n; i++) {
		cin >> a[i];
		b[i] = {a[i], i};
	}
	sort(b+1, b+n+1);
	part[0] = part[n] = 0;
	for(int i = 1, mx = 0; i < n; i++) {
		mx = max(b[i].second, mx);
		part[i] = max(mx - i, 1); // max 1 in case it is already partitioned before bubble
	}
	for(int i = 1; i <= n; i++) ans += max(part[i-1], part[i]);
	cout << ans;
}
