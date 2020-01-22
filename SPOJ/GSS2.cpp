//https://www.quora.com/How-can-the-SPOJ-problem-GSS2-be-solved
#include <bits/stdc++.h>
using namespace std;
#define all(a) begin(a), end(a)
#define csz(a) (int) a.size()
#define pb push_back
#define epb emplace_back
#define mp make_pair
#define load(a, v) fill(begin(a), end(a), v)
#define load_mem(a, v) memset(a, v, sizeof(a));
#define iostream_optimize() ios::sync_with_stdio(false); cin.tie(0);
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
long P1, P2;
seed_seq seq {
	(uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
	(uint64_t) __builtin_ia32_rdtsc(),
	(uint64_t) (uintptr_t) make_unique<char>().get()
};
void gen_bases() {
	mt19937 rng(seq);
	P1 = uniform_int_distribution<int>(0, MOD-1)(rng);
	P2 = uniform_int_distribution<int>(0, MOD-1)(rng);
}
void runner();
int main() {
	iostream_optimize();
	runner();
}
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<< END TEMPLATE >>>>>>>>>>>>>>>>>>>>>>>>>>> */
const int N = 1 << 17; // CHANGE!!!!!1

int n, q, a[N], res[N];
vector<pair<int,int> > qq[N];
int last[2*N];

long actual[2*N], lz_actual[2*N], mx[2*N], lz_mx[2*N];
void apply(int i) {
	mx[i] = max(actual[i] + lz_mx[i], mx[i]);
	actual[i] += lz_actual[i];
	if(i < N) {
		lz_mx[i << 1] = max(lz_mx[i << 1], lz_actual[i << 1] + lz_mx[i]);
		lz_mx[i << 1 | 1] = max(lz_mx[i << 1 | 1], lz_actual[i << 1 | 1] + lz_mx[i]);
		lz_actual[i << 1] += lz_actual[i];
		lz_actual[i << 1 | 1] += lz_actual[i];
	}
	lz_actual[i] = lz_mx[i] = 0;
}

void update(int tl, int tr, int v, int i = 1, int l = 0, int r = N-1) {
	apply(i);
	if(l > tr || r < tl) return;
	if(tl <= l && r <= tr) {
		lz_actual[i] += v;
		lz_mx[i] = max(lz_actual[i], lz_mx[i]);
		apply(i);
		return;
	}
	int mid = l+r >> 1;
	update(tl, tr, v, i << 1, l, mid);
	update(tl, tr, v, i << 1 | 1, mid+1, r);
	mx[i] = max(mx[i << 1], mx[i << 1 | 1]);
	actual[i] = max(actual[i << 1], actual[i << 1 | 1]);
}

long query(int tl, int tr, int i = 1, int l = 0, int r = N-1) {
	apply(i);
	if(l > tr || r < tl) return 0;
	if(tl <= l && r <= tr) return mx[i];
	int mid = l+r >> 1;
	return max(query(tl, tr, i << 1, l, mid), query(tl, tr, i << 1 | 1, mid+1, r));
}

long s[N], f[N];
void update_dummy(int tl, int tr, int v) {
	for(int i = tl; i <= tr; i++) s[i] += v;
	for(int i = tl; i <= tr; i++) f[i] = max(s[i], f[i]);
}

long query_dummy(int tl, int tr) {
	long ret = 0;
	for(int i = tl; i <= tr; i++) ret = max(f[i], ret);
	return ret;
}

void runner() {
	cin >> n;
	for(int i = 0; i < n; i++) cin >> a[i];
	cin >> q;
	for(int i = 0, l, r; i < q; i++) {
		cin >> l >> r;
		--l, --r;
		qq[r].emplace_back(l, i);
	}
	fill(last, last+2*N, -1);
	for(int r = 0; r < n; r++) {
		update(last[N + a[r]] + 1, r, a[r]);
		for(auto p: qq[r]) {
			int l, i; tie(l, i) = p;
			res[i] = query(l, r);
		}
		last[N + a[r]] = r;
	}
	for(int i = 0; i < q; i++) cout << res[i] << '\n';
}
