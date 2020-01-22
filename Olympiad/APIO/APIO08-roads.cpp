/*
 * Observe that when we add in a new edge that creates a cycle,
 * If that cycle has at least one edge of the opposite color, we can delete that edge to remove the cycle
 * Also observe that we can keep doing this for one color, as long as no cycles are created that have only that color
 */
#include <bits/stdc++.h>
using namespace std;
const int N = 2e4+1, M = 1e5+1;

int n, m, k, dsu[N];
vector<pair<int,int> > edges[2];
vector<tuple<int,int,int> > ans;

int root(int v) {
    return dsu[v] < 0 ? v : dsu[v] = root(dsu[v]);
}

bool merge(int u, int v) {
    if((u = root(u)) == (v = root(v))) return false;
    dsu[u] += dsu[v];
    dsu[v] = u;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m >> k;
    for(int i = 0, u, v, c; i < m; i++) {
        cin >> u >> v >> c;
        --u, --v;
        edges[c].emplace_back(u, v);
    }

    // check if k is too small to fill in the spaces left in the forest after merging all the non-cobblestone edges we can
    memset(dsu, -1, sizeof(dsu));
    for(auto e: edges[1]) {
        int u, v;
        tie(u, v) = e;
        merge(u, v);
    }
    int cnt_cc = 0;
    for(int i = 0; i < n; i++) cnt_cc += root(i) == i;
    if(k < cnt_cc - 1) {
        cout << "no solution" << endl;
        return 0;
    }

    // Because of the presence of bridges, when we merge the cobblestone paths towards the end of our program,
    // we need to merge the nodes that are on the spanning tree of non-cobblestone components first, in order to ensure
    // that it is possible to reach all nodes. If we didn't visit this sort, we might only visit a limited number of cobblestone edges,
    // which might not cover all the edges needed to produce a spanning tree
    map<pair<int,int>, bool> on_shortList;
    for(auto e: edges[0]) {
        int u, v;
        tie(u, v) = e;
        if(merge(u, v)) on_shortList[e] = true;
    }
    sort(edges[0].begin(), edges[0].end(), [&] (pair<int,int> e1, pair<int,int> e2) { return on_shortList[e1] > on_shortList[e2]; });

    // Check if k is too large to produce an acyclic graph even when we have merged all of the cobblestone edges we can and compute answer
    memset(dsu, -1, sizeof(dsu));
    cnt_cc = n;
    int possible_marked_edges = 0;
    for(auto e: edges[0]) {
        int u, v;
        tie(u, v) = e;
        if(possible_marked_edges < k && merge(u, v)) {
            --cnt_cc;
            ++possible_marked_edges;
            ans.emplace_back(u, v, 0);
        }
    }
    if(k > possible_marked_edges) {
        cout << "no solution" << endl;
        return 0;
    }
    assert(possible_marked_edges == k);

    // fill in the remaining non-cobblestone edges
    for(auto e: edges[1]) {
        int u, v;
        tie(u, v) = e;
        if(merge(u, v)) {
            ans.emplace_back(u, v, 1);
        }
    }
    assert(ans.size() == n-1); // this assert helped me a lot in debugging
    for(auto e: ans) {
        int u, v, c;
        tie(u, v, c) = e;
        cout << u+1 << ' ' << v+1 << ' ' << c << '\n';
    }
}
/*
Useful test cases:
5 7 2
1 3 0
4 5 1
3 2 0
5 3 1
4 3 0
1 2 1
4 2 1

5 7 1
1 3 0
4 5 1
3 2 0
5 3 1
4 3 0
1 2 1
4 2 1

5 7 3
1 3 0
4 5 1
3 2 0
5 3 1
4 3 0
1 2 1
4 2 1

9 10 2
1 2 1
2 3 1
4 5 1
5 6 1
7 8 1
8 9 1
3 4 0
4 8 0
8 3 0
3 7 0

9 10 3
1 2 1
2 3 1
4 5 1
5 6 1
7 8 1
8 9 1
3 4 0
4 8 0
8 3 0
3 7 0

9 10 4
1 2 1
2 3 1
4 5 1
5 6 1
7 8 1
8 9 1
3 4 0
4 8 0
8 3 0
3 7 0

6 6 3
1 2 1
2 3 0
3 4 1
4 5 0
5 6 1
6 1 0

5 6 3
1 2 0
1 3 0
1 4 0
2 3 1
3 4 1
3 5 0
*/
