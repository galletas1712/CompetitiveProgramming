// Can go from i to j iff -x[i]+t[i] <= -x[j]+t[j] && x[i]+t[i] <= x[j]+t[j]
// Partition into minimal number of LISes
// Use greedy algorithm (chain with largest ending leq current value)
// If the answer doesn't need to be recovered, use longest strictly decreasing subsequence (Dilworth's)
#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+1, INF = 1e9+1;

int n, x[N], t[N];
vector<tuple<int, int, int> > a;
set<pair<int, int> > st;
vector<vector<int> > chains;
int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> n;
	for(int i = 0; i < n; i++) {
		cin >> x[i] >> t[i];
		a.emplace_back(-x[i]+t[i], x[i]+t[i], i);
	}
	sort(a.begin(), a.end());
	chains.push_back(vector<int>());
	st.emplace(-INF, 0);
	for(auto tt: a) {
		int p, q, i;
		tie(p, q, i) = tt;
		auto it = st.upper_bound(make_pair(q, INF));
		if(it == st.begin()) {
			st.emplace(q, chains.size());
			chains.push_back(vector<int>());
			chains.back().push_back(i);
		} else {
			it = prev(it);
			int idx = it->second;
			st.erase(it);
			chains[idx].push_back(i);
			st.emplace(q, idx);
		}
	}
	cout << chains.size() << '\n';
	for(int i = 0; i < chains.size(); i++) {
		for(int j: chains[i]) cout << x[j] << ' ' << t[j] << ' ' << i+1 << '\n';
	}
}
