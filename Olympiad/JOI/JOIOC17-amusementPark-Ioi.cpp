/*
 * See solution in file Joi
 */
#include "Ioi.h"
#include <bits/stdc++.h>
using namespace std;
const int N = 1e4+1;

struct Solver_Ioi {
    int n, m, mxd, mxdv, P, V, color[N], last[N];
    long long X;
    vector<int> g1[N], g2[N];

    bool vis[N];
    int par[N], depth[N];

    void init_dfs(int u) {
        vis[u] = true;
        if (depth[u] > mxd) {
            mxd = depth[u];
            mxdv = u;
        }
        for (int v: g1[u]) {
            if (vis[v]) continue;
            g2[u].push_back(v);
            par[v] = u;
            depth[v] = depth[u] + 1;
            init_dfs(v);
        }
    }

    void colorBig(int u) {
        color[u] = depth[u] % 60;
        for (int v: g2[u]) {
            colorBig(v);
        }
    }

    int curr_count;
    int st[N];
    vector<int> ord;

    void prepColorSmall1(int u) {
        color[u] = (curr_count >= 60 ? -1 : curr_count);
        ++curr_count;

        if (color[u] != -1) {
            st[u] = ord.size();
            ord.push_back(u);
        }

        for (int v: g2[u]) {
            prepColorSmall1(v);
            if(color[u] != -1 && ord.back() != u) ord.push_back(u);
        }
    }

    void traverseUp(int v, set<int> &distinct_mods) { // check both cases where v is in top set and bottom set
        assert(v == last[v]);
        int idx = st[v];
        while(distinct_mods.size() < 60) {
            long long tmp = Move(ord[idx]);
            assert(color[ord[idx]] != -1);
            X |= tmp << color[ord[idx]];
            distinct_mods.insert(color[ord[idx]]);
            idx = (idx + 1) % ord.size();
        }
        // this function assumes that v has not been visited yet
    }

    void solve() {
        par[0] = -1;
        fill(color, color + N, -1);
        fill(last, last + N, -1);
        init_dfs(0);

        if (mxd >= 59) {
            set<int> distinct_mods;
            colorBig(0);
            distinct_mods.insert(color[P]);
            X |= 1ll * V << color[P];
            for(int v = par[P]; v >= 0 && distinct_mods.size() < 60; v = par[v]) {
                long long tmp = Move(v);
                distinct_mods.insert(color[v]);
                X |= tmp << color[v];
            }
            vector<int> ord;
            for(int v = mxdv; v > 0; v = par[v]) ord.push_back(v);
            reverse(ord.begin(), ord.end());
            for(int v: ord) {
                if(distinct_mods.size() >= 60) break;
                long long tmp = Move(v);
                distinct_mods.insert(color[v]);
                X |= tmp << color[v];
            }
            return;
        }

        prepColorSmall1(0);
        ord.pop_back();

        for (int i = 0; i < n; i++) {
            last[i] = i;
            while (color[last[i]] == -1) last[i] = par[last[i]];
        }

        vector<int> sortByDepth;
        for(int i = 0; i < n; i++) sortByDepth.push_back(i);
        sort(sortByDepth.begin(), sortByDepth.end(), [&] (int i, int j) { return depth[i] < depth[j]; });
        for(int i: sortByDepth) {
            if(color[i] != -1) continue;

            int idx = st[last[i]];
            int depth_diff = depth[i] - depth[last[i]];

            set<int> distinct_mods;
            while(distinct_mods.size() < 60 - depth_diff) {
                distinct_mods.insert(color[ord[idx]]);
                idx = (idx + 1) % ord.size();
            }

            for(int v = par[i]; v != last[v]; v = par[v]) { // assures that there are no duplicate mods on the way up. Crucial for last subtask
                assert(color[v] != -1);
                distinct_mods.insert(color[v]);
            }

            for(int mod = 0; mod < 60; mod++) {
                if(!distinct_mods.count(mod)) {
                    color[i] = mod;
                    break;
                }
            }
            assert(ord[st[last[i]]] == last[i]);
        }

        set<int> distinct_mods; // keep the set of all colors visited so far
        distinct_mods.insert(color[P]);
        X |= 1ll * V << color[P];

        if(last[P] == P) {
            int nxt = ord[(st[P] + 1) % ord.size()];
            assert(last[nxt] == nxt);
            traverseUp(nxt, distinct_mods);
        } else {
            int v;
            for(v = par[P]; last[v] != v; v = par[v]) {
                assert(last[v] == last[P]);
                long long tmp = Move(v);
                distinct_mods.insert(color[v]);
                X |= tmp << color[v];
            }
            traverseUp(v, distinct_mods);
        }
    }

    Solver_Ioi(int n, int m, int P, int V, int A[], int B[]) : n(n), m(m), P(P), V(V) {
        for (int i = 0; i < m; i++) {
            g1[A[i]].push_back(B[i]);
            g1[B[i]].push_back(A[i]);
        }
    }
};

long long Ioi(int n, int m, int A[], int B[], int P, int V, int T) {
    Solver_Ioi *solverIoi = new Solver_Ioi(n, m, P, V, A, B);
    solverIoi->solve();
    return solverIoi->X;
}
