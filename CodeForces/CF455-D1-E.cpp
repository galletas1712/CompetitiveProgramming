// Notice that a[l] needs to be a minimum. Can prove via contradicton.
// Objective function is s[y] + a[l]*(x-y) + l*a[l] - s[l], y-x+1 <= l <= y
// Use CHT + segtree
#include <bits/stdc++.h>
#define long long long 
using namespace std;
const int N = 1 << 17;
const long LINF = 1e17;

int n, m;
long a[N], s[N];

struct Line {
    long m, c;
    Line(long m, long c) : m(m), c(c) {}
    long get(long x) { return m * x + c; }
    long operator<(Line rhs) { return m > rhs.m; }
};

struct CHT {
    vector<Line> stk;

    bool bad(Line l1, Line l2, Line l3) {
		return (double) (l1.m-l3.m)*(l2.c-l1.c) >= (double) (l1.m-l2.m)*(l3.c-l1.c);
	}
    void update(Line v) {
        while(stk.size() > 1 && bad(stk[stk.size()-2], stk[stk.size()-1], v)) stk.pop_back();
		stk.push_back(v);
    }
    long query(long x) {
        if(stk.empty()) return LINF;
        int l = 0, r = stk.size()-1;
		while(r-l > 1) {
			int mid = l+r >> 1;
			if(stk[mid+1].get(x) <= stk[mid].get(x)) l = mid;
			else r = mid;
		}
		return min(stk[l].get(x), stk[r].get(x));
    }
};

CHT t[2*N];

void build() {
    for(int i = 1; i <= n; i++) t[N+i].update(Line(a[i], 1ll * i * a[i] - s[i]));
    for(int i = N-1; i >= 1; i--) {
        vector<Line> tmp;
        merge(t[i << 1].stk.begin(), t[i << 1].stk.end(), t[i << 1 | 1].stk.begin(), t[i << 1 | 1].stk.end(), back_inserter(tmp));
        for(Line v: tmp) t[i].update(v);
    }
}

long query(int l, int r, long x) {
    long ret = LINF;
    for(l += N, r += N+1; l < r; l >>= 1, r >>= 1) {
        if(l & 1) ret = min(t[l++].query(x), ret);
        if(r & 1) ret = min(t[--r].query(x), ret);
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    for(int i = 1; i <= n; i++) {
        cin >> a[i];
        s[i] = s[i-1] + a[i];
    }
    build();
    cin >> m;
    for(int i = 0, x, y; i < m; i++) {
        cin >> x >> y;
        long res = s[y] + query(y-x+1, y, x-y);
        cout << res << '\n';
    }
}
