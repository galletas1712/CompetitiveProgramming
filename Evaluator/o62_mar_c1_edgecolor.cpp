#include <bits/stdc++.h>
using namespace std;
const int N = 6e5+1;

int n, m;
vector<pair<int, pair<int,int>>> lsl[N], lsr[N];
bool res[N];

struct parr {
    int tick;
    vector<int> arr;
    stack<tuple<int,int,int> > stk;

    parr(int n, int v) {
        arr = vector<int>(n, v);
        tick = 0;
        stk = stack<tuple<int,int,int> >();
    }
    void set(int i, int v) {
        ++tick;
        stk.emplace(tick, i, arr[i]);
        arr[i] = v;
    }
    int get(int i) {
        return arr[i];
    }
    void rollback(int t) {
        while(!stk.empty()) {
            int tt, i, v;
            tie(tt, i, v) = stk.top();
            if(tt <= t) return;
            arr[i] = v;
            stk.pop();
        }
    }
};

parr *dsu, *P;

pair<int,int> root(int v) {
    if(dsu->get(v) < 0) return {v, P->get(v)};
    else {
        pair<int,int> tmp = root(dsu->get(v));
        return {tmp.first, tmp.second ^ P->get(v)};
    }
}

bool merge(int u, int v) {
    pair<int,int> rtu = root(u), rtv = root(v);
    if(rtu.first == rtv.first) {
        return (rtu.second ^ rtv.second) == 1;
    }
    if(dsu->get(rtu.first) > dsu->get(rtv.first)) swap(rtu, rtv);
    dsu->set(rtu.first, dsu->get(rtu.first) + dsu->get(rtv.first));
    dsu->set(rtv.first, rtu.first);
    P->set(rtv.first, rtu.second ^ rtv.second ^ 1);
    return true;
}

void rec(int l, int r, bool can) {
    if(l == r) {
        res[l] = can;
        return;
    }
    int mid = l+r >> 1;
    bool can_left = can;
    bool can_right = can;

    if(can) {
        int dsu_T = dsu->tick;
        int P_T = P->tick;
        for (int i = mid; i <= r; i++) {
            for (auto tt: lsr[i]) {
                int lf = tt.first;
                pair<int, int> edge = tt.second;
                if (lf <= l) {
                    can_left &= merge(edge.first, edge.second);
                }
            }
        }
        rec(l, mid, can_left);
        dsu->rollback(dsu_T);
        P->rollback(P_T);

        dsu_T = dsu->tick;
        P_T = P->tick;
        for(int i = l; i <= mid+1; i++) {
            for(auto tt: lsl[i]) {
                int rg = tt.first;
                pair<int, int> edge = tt.second;
                if(rg >= r) {
                    can_right &= merge(edge.first, edge.second);
                }
            }
        }
        rec(mid+1, r, can_right);
        dsu->rollback(dsu_T);
        P->rollback(P_T);
    } else {
        rec(l, mid, can);
        rec(mid+1, r, can);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;

    dsu = new parr(n+1, -1);
    P = new parr(n+1, 0);
    map<pair<int,int>, int> mp;

    for(int t = 0, inst, u, v; t < m; t++) {
        cin >> inst >> u >> v;
        if(u > v) swap(u, v);
        pair<int,int> curr = {u, v};
        if(inst == 1) {
            mp[curr] = t;
        } else {
            lsl[mp[curr]].emplace_back(t-1, curr);
            lsr[t-1].emplace_back(mp[curr], curr);
            mp.erase(curr);
        }
    }
    for(auto tt: mp) {
        pair<int,int> curr;
        int t;
        tie(curr, t) = tt;
        lsl[t].emplace_back(m-1, curr);
        lsr[m-1].emplace_back(t, curr);
    }
    rec(0, m-1, 1);
    for(int i = 0; i < m; i++) {
        cout << (res[i] ? "yes" : "no") << '\n';
    }
}
