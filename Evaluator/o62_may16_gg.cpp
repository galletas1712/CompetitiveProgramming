#include <bits/stdc++.h>
#define long long long
using namespace std;
const long LINF = 1e18;

long K, X1, Y1, X2, Y2;
long sample_dist[51][12][12];
void generate_sample_dist() {
    for(int p = 2; p <= K; p++) {
        for(int i = 0; i < 12; i++) {
            for(int j = 0; j < 12; j++) {
                if(i != j) sample_dist[p][i][j] = LINF;
            }
        }
        for(int i = 0; i < 12; i++) {
            if(i == 1 || i == 4 || i == 7 || i == 10) {
                sample_dist[p][i][(i + 1) % 12] = sample_dist[p][(i + 1) % 12][i] = 1;
            } else {
                sample_dist[p][i][(i + 1) % 12] = sample_dist[p][(i + 1) % 12][i] = (1ll << (p - 1)) - 1;
            }
        }
        for(int k = 0; k < 12; k++) {
            for(int i = 0; i < 12; i++) {
                for(int j = 0; j < 12; j++) {
                    sample_dist[p][i][j] = min(sample_dist[p][i][k] + sample_dist[p][k][j], sample_dist[p][i][j]);
                }
            }
        }
    }
}
void print_sample_dist(int k) {
    for(int i = 0; i < 12; i++) {
        for(int j = i; j < 12; j++) {
            cerr<< k << ' ' << i << ' ' << j << ' ' << sample_dist[k][i][j] << endl;
        }
    }
}


long get_component(long k, long x, long y) {
    long x_shift = x >> (k - 1) & 1;
    long y_shift = y >> (k - 1) & 1;
    assert(0 <= x_shift * 2ll + y_shift && x_shift * 2ll + y_shift <= 3);
    return x_shift * 2ll + y_shift;
}

array<long, 12> get_dist(long k, long x, long y, array<long, 4> pre) {
    assert(k >= 2);
    int component = get_component(k, x, y);
    array<long, 12> ret;
    ret.fill(LINF);
    if(component == 0) {
        for(int i = 0; i < 12; i++) {
            ret[i] = min(pre[0] + sample_dist[k][9][i], ret[i]);
            ret[i] = min(pre[2] + sample_dist[k][8][i], ret[i]);
            ret[i] = min(pre[1] + sample_dist[k][10][i], ret[i]);
            ret[i] = min(pre[3] + (1ll << k-1) - 1 + min(sample_dist[k][8][i], sample_dist[k][10][i]), ret[i]);
        }
    } else if(component == 1) {
        for(int i = 0; i < 12; i++) {
            ret[i] = min(pre[1] + sample_dist[k][0][i], ret[i]);
            ret[i] = min(pre[3] + sample_dist[k][1][i], ret[i]);
            ret[i] = min(pre[0] + sample_dist[k][11][i], ret[i]);
            ret[i] = min(pre[2] + (1ll << k-1) - 1 + min(sample_dist[k][1][i], sample_dist[k][11][i]), ret[i]);
        }
    } else if(component == 2) {
        for(int i = 0; i < 12; i++) {
            ret[i] = min(pre[2] + sample_dist[k][6][i], ret[i]);
            ret[i] = min(pre[3] + sample_dist[k][5][i], ret[i]);
            ret[i] = min(pre[0] + sample_dist[k][7][i], ret[i]);
            ret[i] = min(pre[1] + (1ll << k-1) - 1 + min(sample_dist[k][5][i], sample_dist[k][7][i]), ret[i]);
        }
    } else {
        for(int i = 0; i < 12; i++) {
            ret[i] = min(pre[3] + sample_dist[k][3][i], ret[i]);
            ret[i] = min(pre[1] + sample_dist[k][2][i], ret[i]);
            ret[i] = min(pre[2] + sample_dist[k][4][i], ret[i]);
            ret[i] = min(pre[0] + (1ll << k-1) - 1 + min(sample_dist[k][2][i], sample_dist[k][4][i]), ret[i]);
        }
    }
    return ret;
}

array<long, 4> rec(long k, long x, long y) {
    if(k == 1) {
        auto ret = array<long, 4>({x + y, x + abs(y - 1), abs(x - 1) + y, abs(x - 1) + abs(y - 1)});
        return ret;
    }
    array<long, 4> tmp = rec(k - 1, x & ((1ll << k - 1) - 1), y & ((1ll << k - 1) - 1));
    array<long, 12> res = get_dist(k, x, y, tmp);
    array<long, 4> ret = {res[9], res[0], res[6], res[3]};
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> K >> X1 >> Y1 >> X2 >> Y2;
    while(K > 1) {
        if(get_component(K, X1, Y1) != get_component(K, X2, Y2)) break;
        --K;
    }
    if(K == 1) {
        X1 = X1 & 1;
        Y1 = Y1 & 1;
        X2 = X2 & 1;
        Y2 = Y2 & 1;
        cout << abs(X1 - X2) + abs(Y1 - Y2);
        return 0;
    }
    generate_sample_dist();
    array<long, 4> res1 = rec(K - 1, X1 & ((1ll << K - 1) - 1), Y1 & ((1ll << K - 1) - 1));
    array<long, 4> res2 = rec(K - 1, X2 & ((1ll << K - 1) - 1), Y2 & ((1ll << K - 1) - 1));
    array<long, 12> res_to_1 = get_dist(K, X1, Y1, res1);
    array<long, 12> res_to_2 = get_dist(K, X2, Y2, res2);
    long ans = LINF;
    for(int i = 0; i < 12; i++) {
        ans = min(res_to_1[i] + res_to_2[i], ans);
    }
    cout << ans << endl;
}
