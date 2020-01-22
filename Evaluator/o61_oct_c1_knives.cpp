// Consider the height at maximum width for each blade. For each query, we can compute the width at the given height if we have the height at maximum width, since the difference is always going up/down by one.
// After we find the formula, we can see that it is easily maintainable using a fenwick tree.
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 1 << 20;

struct fenwick {
	long f[N];
	void update(int i, int v) { ++i; while(i < N) f[i] += v, i += i & -i; }
	long query(int i) {
		++i;
		long ret = 0;
		while(i > 0) ret += f[i], i -= i & -i;
		return ret;
	}
	long query(int l, int r) {
		return query(r) - query(l-1);
	}
} cnt[2];

int n;
long L, Q, mnidx[N], mxidx[N], sum[2];
bool state[N];

inline long get_mxidx(int i) {
	return (state[i] ? mnidx[i] : mxidx[i]);
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n >> L >> Q;
	sum[0] = L * n;
	cnt[0].update(L, n);
	for(int i = 0; i < n; i++) mxidx[i] = L;
	for(int t = 0; t < Q; t++) {
		char inst;
		long a, b;
		cin >> inst >> a;
		if(inst == 's' || inst == 'f') {
			cnt[state[a]].update(get_mxidx(a), -1);
			sum[state[a]] -= get_mxidx(a);
			if(inst == 's') {
				cin >> b;
				mxidx[a] = (mxidx[a] + b - 1) % L + 1;
				mnidx[a] = (mnidx[a] + b) % L;
			} else state[a] ^= 1;
			cnt[state[a]].update(get_mxidx(a), 1);
			sum[state[a]] += get_mxidx(a);
		} else {
			long res = 0;
			res += cnt[0].query(a, L) * L + cnt[0].query(0, L) * a - sum[0];
			res += cnt[1].query(0, a) * L + sum[1] - cnt[1].query(0, L) * a;
			cout << res <<'\n';
		}
	}
}
