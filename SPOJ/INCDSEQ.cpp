#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 1e4+10;
const long MOD = 5e6;

int n, K, a[N];
long dp[N][51], f[N][51];

void update(int i, int k, long v) {
	++i;
	while(i < N) f[i][k] = (f[i][k] + v) % MOD, i += i & -i;
}

long query(int i, int k) {
	long ret = 0;
	++i;
	while(i > 0) {
		ret = (ret + f[i][k]) % MOD;
		i -= i & -i;
	}
	return ret;
}

void compress() {
	vector<int> vals;
	vals.push_back(-1);
	for(int i = 1; i <= n; i++) vals.push_back(a[i]);
	sort(vals.begin(), vals.end());
	vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
	unordered_map<int, int> mp;
	for(int i = 0; i < vals.size(); i++) mp[vals[i]] = i;
	for(int i = 1; i <= n; i++) a[i] = mp[a[i]];
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> K;
	for(int i = 1; i <= n; i++) cin >> a[i];
	compress();
	for(int i = 1; i <= n; i++) {
		for(int k = 1; k <= K; k++) {
			long diff = ((k > 1 ? query(a[i]-1, k-1) : 1) - dp[a[i]][k] + MOD) % MOD;
			dp[a[i]][k] = (dp[a[i]][k] + diff) % MOD;
			update(a[i], k, diff);
		}
	}
	cout << (query(N-1, K) + MOD) % MOD << endl;
}
