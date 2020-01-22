// Use the property that every group has at least one node with degree < k
// If we fix a node, we can find a clique (if exists) as all other members of the clique are adjacent to it by definition
// So, we brute force using the adjacency list which takes O(2^deg) time
// However, this is too slow, but notice that if we have tried setting a node to be in the clique already, we can eliminate it.
// Since smaller degrees are more efficient, we eliminate the ones with smallest degree first.
// Therefore, we sort the beginning nodes in order of degree. 
// Using the property previously stated, the time complexity always stays within O(2^k) for each beginning node.
// Total time is O(n * 2^k) (haven't checked thoroughly)
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 5e4+1;

class NewPD {

	int n, k, ans;
	vector<int> a[N], curr;
	bool vis[N];

	void rec(int u, int i) {
		ans = max((int) curr.size(), ans);
		if(i == a[u].size() || ans == k) return;

		rec(u, i+1);

		if(vis[a[u][i]]) return;

		bool valid = true;
		for(int v: curr) {
			if(!binary_search(a[a[u][i]].begin(), a[a[u][i]].end(), v)) {
				valid = false;
				break;
			}
		}
		if(valid) {
			curr.push_back(a[u][i]);
			rec(u, i+1);
			curr.pop_back();
		}
	}

public:
	void solve(istream &cin, ostream &cout) {
		cin >> n >> k;
		for(int i = 0; i < n; i++) {
			int sz;
			cin >> sz;
			a[i].resize(sz);
			for(int j = 0; j < sz; j++) cin >> a[i][j];
			sort(a[i].begin(), a[i].end());
		}
		vector<int> ord(n);
		iota(ord.begin(), ord.end(), 0);
		sort(ord.begin(), ord.end(), [&] (int x, int y) { return a[x].size() < a[y].size(); });
		for(int i: ord) {
			vis[i] = true;
			curr.push_back(i);
			rec(i, 0);
			curr.pop_back();
		}
		cout << ans;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    NewPD *obj = new NewPD();
		obj->solve(in, out);
		delete obj;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Solver solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
