/*
 * Problem reduces to finding two longest edge-disjoint paths
 * A[u] = longest path from node u down into subtree
 * B[u] = longest path from node u down into subtree and another edge-disjoint path already in subtree
 * D[u] = longest path in subtree of u (can cross child subtrees)
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class Patrol {

    int n, k, ans1, ans2;
    vector<int> g[N];

    int A[N], B[N], D[N];

    void dfs(int u, int p) {
        if(p >= 1) g[u].erase(find(g[u].begin(), g[u].end(), p));
        set<pair<int,int> > pqA, pqD;
        for(int v: g[u]) {
            dfs(v, u);
            pqA.emplace(A[v], v);
            pqD.emplace(D[v], v);
            if(pqA.size() > 4) {
                pqA.erase(pqA.begin());
            }
            if(pqD.size() > 2) {
                pqD.erase(pqD.begin());
            }
            D[u] = max(max(A[u] + A[v] + 1, D[v]), D[u]);
            A[u] = max(A[v] + 1, A[u]);
            B[u] = max(B[v] + 1, B[u]); // Use some B from subtree
        }

        int cnt = 0, sumA = 0; // 3 A's
        for(auto it = pqA.rbegin(); it != pqA.rend(); ++it) {
            ++cnt;
            sumA += it->first + 1;
            if(cnt == 3) break;
        }
        B[u] = max(sumA, B[u]);

        for(int v: g[u]) { // use D[v1] and A[v2] + 1
            int theA = 0;
            for(auto it = pqA.rbegin(); it != pqA.rend(); ++it) {
                if(it->second == v) continue;
                theA = it->first + 1;
                break;
            }
            B[u] = max(D[v] + theA, B[u]);
        }

        ans1 = max(D[u], ans2); // ans needs to also include D[u], especially for k = 1
        ans2 = max(D[u], ans2);

        if(k == 1) return;

        int tmp_ans = 0;

        for(auto p: pqA) tmp_ans += p.first + 1; // at most 4 A's to build two paths going through root

        for(int v: g[u]) { // use D[v]
            int cnt = 0, sumA = 0; // use A[v1] + A[v2] + 1 + 1
            for(auto it = pqA.rbegin(); it != pqA.rend(); ++it) {
                if(it->second == v) continue;
                ++cnt;
                sumA += it->first + 1;
                if(cnt == 2) break;
            }

            int otherD = 0; // use D[v']
            for(auto it = pqD.rbegin(); it != pqD.rend(); ++it) {
                if(it->second == v) continue;
                otherD = it->first;
                break;
            }
            tmp_ans = max(D[v] + max(sumA, otherD), tmp_ans);
        }

        for(int v: g[u]) { // use B[v] + 1;
            int theA = 0; // can only use at most one A
            for(auto it = pqA.rbegin(); it != pqA.rend(); ++it) {
                if(it->second == v) continue;
                theA = it->first + 1;
                break;
            }
            tmp_ans = max(B[v] + 1 + theA, tmp_ans);
        }
        ans2 = max(tmp_ans, ans2);
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> k;
        for(int i = 1, u, v; i < n; i++) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfs(1, -1);
        if(k == 1) cout << 2*(n-1) - ans1 + 1 << endl;
        else cout << 2*(n-1) - max(ans1, ans2) + 2 << endl;
    }
};


class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Patrol *obj = new Patrol();
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
