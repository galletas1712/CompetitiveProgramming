#include "pointlib.h"
#include <bits/stdc++.h>
using namespace std;
const int N = 3010, M = 2e6+10, INF = 1e9;

int n, d[N][N], x[N], y[N], id[M][2];
vector<int> ord, so_far;

inline int dist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

bool rec(int idx, int tx, int ty, int mnx, int mxx) {
    mnx = min(tx, mnx);
    mxx = max(tx, mxx);

    if(mxx - mnx > 1000000 || id[tx][ty] != -INF) return false;

    int i = ord[idx];
    bool valid = true;
    for(int j: so_far) if(dist(tx, ty, x[j], y[j]) != d[i][j]) {
        valid = false;
        break;
    }
    if(!valid) return false;
//    cerr << "valid " <<  ord[idx] << ' ' << tx << ' ' << ty << endl;
//    cerr << "stack" << endl;
//    for(int i: ord) cerr << i << ' ' << x[i] << ' ' << y[i] << endl;
//    cerr << endl;

    so_far.push_back(i);
    id[tx][ty] = i;
    x[i] = tx, y[i] = ty;

    if(idx == n-1) return true;

    int nxt = ord[idx + 1];

    bool ret = false;
    ret |= rec(idx + 1, M/2 + d[0][nxt], 0, mnx, mxx);
    if(!ret) ret |= rec(idx + 1, M/2 - d[0][nxt], 0, mnx, mxx);
    if(!ret) ret |= rec(idx + 1, M/2 + d[0][nxt] - 1, 1, mnx, mxx);
    if(!ret) ret |= rec(idx + 1, M/2 + 1 - d[0][nxt], 1, mnx, mxx);

    if(!ret) {
        so_far.pop_back();
        id[tx][ty] = -INF;
        x[i] = y[i] = -INF;
    }

    return ret;
}

int main() {
    fill(x, x+N, -INF);
    fill(y, y+N, -INF);
    for(int i = 0; i < M; i++) id[i][0] = id[i][1] = -INF;
    n = point_init();
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            d[i][j] = query(i, j);
        }
    }
    for(int i = 0; i < n; i++) ord.push_back(i);
    sort(ord.begin(), ord.end(), [] (int i, int j) { return d[0][i] < d[0][j]; });
    rec(0, M/2, 0, M/2, 0);

    int mx_x = *max_element(x, x+n);
    if(mx_x > 1000000) {
        for(int i = 0; i < n; i++) x[i] -= mx_x - 1000000;
    }

    for(int i = 0; i < n; i++) {
        assert(0 <= x[i] && x[i] <= 1000000);
        assert(0 <= y[i] && y[i] <= 1);
        for(int j = 0; j < n; j++) {
            assert(dist(x[i], y[i], x[j], y[j]) == d[i][j]);
        }
    }

    for(int i = 0; i < n; i++) printf("%d %d\n", x[i], y[i]);
}
