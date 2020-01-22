// Problem reduces to finding the sum of areas of every possible rectangle
// Iterate lower right corner and use stack to separate different possible heights
// Some rearranging of aggregate formula will give a separable equation
#include <bits/stdc++.h>
#define long long long
using namespace std;
const int N = 2002;

struct block {
    long h, l, r;
    block(long h, long l, long r): h(h), l(l), r(r) {}
};

inline long get_agg1(block tmp) {
    return (tmp.r-tmp.l+1) * tmp.h * (tmp.h + 1) / 2;
}

inline long get_agg2(block tmp) {
    return (tmp.r * (tmp.r + 1) / 2 - tmp.l * (tmp.l - 1) / 2) * (tmp.h * (tmp.h + 1) / 2); // negative
}

int n, m, last[N], up[N][N];
char s[N][N];
long ans;

long brute() {
    long actual = 0;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(s[i][j] == '#') continue;
            int mn = up[i][j];
            long res = 0;
            for(int k = j; k >= 0; k--) {
                mn = min(up[i][k], mn);
                res += (mn * (mn + 1) / 2) * (j-k+1); // aggregate formula here
            }
            actual += res;
//            cerr << res << ' ';
        }
//        cerr << endl;
    }
    return actual;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    for(int i = 0; i < n; i++) cin >> s[i];

    fill(last, last+N, -1);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(s[i][j] == '#') last[j] = i;
            else up[i][j] = i - last[j];
        }
    }

    for(int i = 0; i < n; i++) {
        vector<block> stk;
        long sum1 = 0, sum2 = 0;
        for(int j = 0, last = -1; j < m; j++) {
            if(!up[i][j]) {
                sum1 = 0;
                sum2 = 0;
                while(!stk.empty()) stk.pop_back();
                last = j;
                continue;
            }
            while(!stk.empty() && stk.back().h >= up[i][j]) {
                block tmp = stk.back();
                sum1 -= get_agg1(tmp);
                sum2 += get_agg2(tmp);
                stk.pop_back();
            }
            block tmp = block(up[i][j], (stk.empty() ? last : stk.back().r + 1), j - 1);
            sum1 += get_agg1(tmp);
            sum2 -= get_agg2(tmp);
            stk.push_back(tmp);
            ans += sum1 * j + sum2; // sum2 is already negative
        }
    }
    cout << ans << endl;
}
