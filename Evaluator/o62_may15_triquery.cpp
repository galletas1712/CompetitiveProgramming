#include <bits/stdc++.h>
using namespace std;
const int N = 6e3+100, M = 2e3 + 100, shift = 2010;

int n, m, a[N][N];
struct fenwick {
    int f2d[N][M];

    void update_2d(int x, int y, int add) {
        x += shift;
        for(int i = x; i < N; i += i & -i) {
            for(int j = y; j < M; j += j & -j) {
                f2d[i][j] += add;
            }
        }
    }

    int query_2d(int x, int y) {
        x += shift;
        int ret = 0;
        for(int i = x; i > 0; i -= i & -i) {
            for(int j = y; j > 0; j -= j & -j) {
                ret += f2d[i][j];
            }
        }
        return ret;
    }
    int query_box(int x1, int y1, int x2, int y2) {
        return query_2d(x2, y2) - query_2d(x1 - 1, y2) - query_2d(x2, y1 - 1) + query_2d(x1 - 1, y1 - 1);
    }
} fen1, fen2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for(int i = 0, inst, R, C, X; i < m; i++) {
        cin >> inst >> R >> C >> X;
        if(inst == 1) {
            --X;
            int res1 = fen1.query_box(R + C - X, 1, R + C, M - 1);
            int res2 = fen1.query_box(R + C - X, R + 1, R + C, M - 1);
            int res3 = fen2.query_box(R + C - X, C + 1, R + C, M - 1);
            int res4 = fen1.query_box(R + C - X, 1, R + C - X, M - 1) - fen1.query_box(R + C - X, R + 1, R + C - X, M - 1) - fen2.query_box(R + C - X, C + 1, R + C - X, M - 1);
            cout << res1 - res2 - res3 - res4 / 2 << '\n';
        } else {
            fen1.update_2d(R + C, R, X - a[R][C]);
            fen2.update_2d(R + C, C, X - a[R][C]);
            a[R][C] = X;
        }
    }
}
