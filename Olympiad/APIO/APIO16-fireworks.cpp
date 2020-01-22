/*
Use subtask 2 solution where DP equation is:
dp[u][x] = min(sum[u][y] + abs(x - (y + wpar))), y <= x
wpar = weight of edge from u to its parent
sum[u][x] = sum of dp[v][x]
Let f(x) = sum[u][x], g(x) = dp[u][x]

If we shift the function by wpar, we get
g(x) = min(f(y) + abs(x - y)), y <= x+wpar

Represent slopes using a pq as per usual and merge the typical way with absolute value slope trick functions.
Let's leave the merging of children solutions for later.

When we combine the children solutions with the parent edge, the optimal values for g(x) are as follows (l0 is the leftmost point where f is minimum and r0 is the rightmost):
if x <= l0 - wpar -> g(x) = f(x+wpar) + wpar, slope = f(x+wpar) - f(x+wpar-1)
if l0 - wpar <= x <= l0 -> g(x) = f(l0) + abs(x - l0), slope = -1
if l0 <= x <= r0 -> g(x) = f(x), slope = 0
if x > r0 -> g(x) = f(r0) + abs(x - r0), slope = 1
Notice that for the first case, the slope is equivalent to the original slope before the shift by wpar. Let's now shift g(x) back by wpar:
if x <= l0 -> g(x) = f(x) + wpar, slope = f(x) - f(x-1) = original slope
if l0 <= x <= l0 + wpar -> g(x) = f(l0 + wpar) + abs(x - (l0 + wpar))
if l0 + wpar <= x <= r0 + wpar -> g(x) = f(x)
if x > r0 + wpar -> g(x) = f(r0 + wpar) + abs(x - (r0 + wpar))

For the first case, slope is always >= 1, so the cost of coming back by wpar will never exceed our benefit of going down more. We take the maximum coord possible.
For the second case, we know that we should not go past l0 + wpar (in original, before first shift) since we do not benefit by going more right.
Third case and fourth case are obvious.

Some more observations to be made:
- After we combine the children solutions with the parent edge, the rightmost slope is always 1.
- When we merge children solutions together, the rightmost slope is equal to the number of children.

This gives us a way of storing the actual DP values aside from the slope. We store the y-coordinate of the rightmost vertex which is equal to the minimum.
When we merge two functions, we look at the one with the leftmost right coordinate, and add the absolute value of the rightmost right coordinate subtracted with that number to the rightmost right coordinate.
We can easily find where the minimum will be by popping starting from the right, using observation two to find the number of times we can pop.
That means that we pop until we reach l0.
Don't forget that after merging children solutions, l0+w and r0+w are new slope changing points that should be added to the pq.
*/

#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};


class Fireworks {
	int n, m;
	vector<vector<pair<int,long> > > g;
	vector<priority_queue<long> > pq;
	vector<long> val;

	void dfs(int u, long wpar) {
		long cnt = 0;
		for(auto pr: g[u]) {
			int v;
			long w;
			tie(v, w) = pr;
			dfs(v, w);
			if(pq[u].empty()) {
				val[u] += val[v];
			} else if(pq[v].top() > pq[u].top()) {
				val[u] += val[v] + 1ll * cnt * abs(pq[v].top() - pq[u].top());
			} else {
				val[u] += val[v] + abs(pq[u].top() - pq[v].top());
			}
			if(pq[v].size() > pq[u].size()) {
				swap(pq[u], pq[v]);
			}
			while(!pq[v].empty()) pq[u].push(pq[v].top()), pq[v].pop();
			++cnt;
		}
		long r;
		while(cnt--) {
			r = pq[u].top();
			pq[u].pop();
			val[u] -= 1ll * cnt * (r - pq[u].top()); // already --
		}
		if(!g[u].empty()) {
			long l = pq[u].top();
			pq[u].pop();
			pq[u].push(l + wpar);
			pq[u].push(r + wpar);
		} else {
			pq[u].push(wpar);
			pq[u].push(wpar);
		}
	}

public:
	void solve(istream& cin, ostream& cout) {
		cin >> n >> m;
		n += m;
		g = vector<vector<pair<int,long> > >(n);
		pq = vector<priority_queue<long> >(n);
		val = vector<long>(n);
		for(int i = 1, p, c; i < n; i++) {
			cin >> p >> c;
			g[p-1].emplace_back(i, c);
		}
		dfs(0, 0);
		cout << val[0] << endl;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Fireworks solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
