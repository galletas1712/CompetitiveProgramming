/*
 * If maximum depth is >= 60, we can simply use depth mods 60 to key the bits
 * Otherwise, we have to create a subtree from the root of size 60 (in this case, subtree simply means a portion of the tree, not a subtree in the rooted tree)
 * For each vertex not in the subtree, we traverse its ancestors until we get to the subtree (remember, there are < 60 of such ancestors)
 * We then consider position of the lowest ancestor (that is in the subtree) in the euler tour sequence of the subtree
 * We already have a number of distinct mods by traversing the ancestors, so we only need a circular subarray of the euler tour sequence of the subtree
 * Notice that every adjacent element in the euler tour of the subtree is adjacent in the tree, so all edges exist
 * This requires only 120 moves
 * Let x be difference between the depth between P and the first ancestor of P not in the subtree
 * The number of moves <= x*1 + (60-x)*2 <= 120, since x < 60 by assumption of low maximum depth
 */
#include "Joi.h"
#include <bits/stdc++.h>
using namespace std;
const int N = 1e4+1;

struct Solver_Joi {
    int n, m, mxd, mxdv, color[N], last[N];
    long long X;
    vector<int> g1[N], g2[N];

    bool vis[N];
    int par[N], depth[N];
    void init_dfs(int u) {
        vis[u] = true;
        if(depth[u] > mxd) {
            mxd = depth[u];
            mxdv = u;
        }
        for(int v: g1[u]) {
            if(vis[v]) continue;
            g2[u].push_back(v);
            par[v] = u;
            depth[v] = depth[u] + 1;
            init_dfs(v);
        }
    }

    void colorBig(int u) {
        color[u] = depth[u] % 60;
        for(int v: g2[u]) {
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

    void solve() {
        par[0] = -1;
        fill(color, color+N, -1);
        fill(last, last+N, -1);
        init_dfs(0);

        if(mxd >= 59) {
            colorBig(0);
            for(int i = 0; i < n; i++) {
                assert(color[i] != -1 && 0 <= color[i] && color[i] < 60);
                MessageBoard(i, X >> color[i] & 1);
            }
            return;
        }

        prepColorSmall1(0);

        for(int i = 0; i < n; i++) {
            last[i] = i;
            while(color[last[i]] == -1) last[i] = par[last[i]];
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

            for(int v = par[i]; v != last[v]; v = par[v]) {
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

        for(int i = 0; i < n; i++) {
            assert(color[i] != -1 && 0 <= color[i] && color[i] < 60);
            MessageBoard(i, X >> color[i] & 1);
        }
    }

    Solver_Joi(int n, int m, long long X, int A[], int B[]): n(n), m(m), X(X) {
        for(int i = 0; i < m; i++) {
            g1[A[i]].push_back(B[i]);
            g1[B[i]].push_back(A[i]);
        }
    }
};

void Joi(int n, int m, int A[], int B[], long long X, int T) {
    Solver_Joi *solver = new Solver_Joi(n, m, X, A, B);
    solver->solve();
}
