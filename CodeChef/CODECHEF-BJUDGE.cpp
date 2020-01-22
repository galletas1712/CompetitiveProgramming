#include <bits/stdc++.h>
using namespace std;
#define all(a) begin(a), end(a)
#define csz(a) (int) a.size()
#define pb push_back
#define epb emplace_back
#define load(a, v) fill(begin(a), end(a), v)
#define load_mem(a, v) memset(a, v, sizeof(a));
#define iostream_optimize() ios::sync_with_stdio(false); cin.tie(0);
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
void runner();
int main() {
	iostream_optimize();
	int t;
	cin >> t;
	while(t--) runner();
}
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<< END TEMPLATE >>>>>>>>>>>>>>>>>>>>>>>>>>> */
void runner() {

	int n, m;
	cin >> n >> m;

	vector<int> D(n+1), mxr(n+1);
	vector<vector<int> > A(n+1, vector<int>(m+1)), B(n+1, vector<int>(m+1));
	vector<bool> has0(n+1);
	vector<pair<int,int> > cols;

	bool allone = true;
	for(int i = 1; i <= n; i++) cin >> D[i], allone &= D[i];
	for(int i = 1; i <= n; i++) for(int j = 1; j <= m; j++) cin >> A[i][j];
	for(int i = 1; i <= n; i++) for(int j = 1; j <= m; j++) cin >> B[i][j];
	if(allone) {
		cout << "YES\n1 0\n";
		return;
	}
	for(int col = 1; col <= m; col++) {
		bool valid = true;
		int mx = 1;
		for(int i = 1; i <= n; i++) if(D[i]) {
			if(!B[i][col]) valid = false;
			mx = max(A[i][col], mx);
		}
		if(valid) cols.emplace_back(mx, col);
	}
	sort(cols.begin(), cols.end());
	int T = 1;
	for(int idx = 0; idx < cols.size(); idx++) {
		int mxt, col;
		tie(mxt, col) = cols[idx];
		// cout << "dbg " << mxt << ' ' << col << endl;
		T = max(mxt, T);
		bool valid = true;
		for(int i = 1; i <= n; i++) if(!D[i]) {
			if(!B[i][col]) has0[i] = true;
			mxr[i] = max(A[i][col], mxr[i]);
			valid &= has0[i] || mxr[i] > T;
		}
		if(valid) {
			cout << "YES\n" << T << ' ' << idx+1 << '\n';
			for(int idx2 = 0; idx2 <= idx; idx2++) cout << cols[idx2].second << ' ';
			cout << '\n';
			return;
		}
	}
	cout << "NO\n";
}