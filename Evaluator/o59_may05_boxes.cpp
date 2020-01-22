/*
 * Rename indices as continguous segments
 * Can prove that we will never split a contiguous segment
 * dp[l][r] = maximum cost to remove all nodes in range l, r
 * f[l][curr][targ_cnt] = maximum cost to remove all segments with value equal to that at l, curr and the sum of lengths of such segments is targ_cnt.
 * For this function, we only calculate the value of dp in between each segment, as in the end we will do targ_cnt^3 when we actually use the value
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 210;

class Boxes {

    long cube(long x) {
        return x * x * x;
    }

    int n, org[N];
    vector<int> val, cnt;
    vector<int> ls[N];
    int pre[N];

    long dp[N][N], f[N][N][N];
    bool mark1[N][N], mark2[N][N][N];
    long rec1(int l, int r) {
        if(mark1[l][r]) return dp[l][r];
        mark1[l][r] = true;
        dp[l][r] = -LINF;
        if(l > r) return dp[l][r] = 0;
        if(l == r) return dp[l][r] = cube(cnt[l]);

        for(int i = l; i < r; i++) {
            dp[l][r] = max(rec1(l, i) + rec1(i + 1, r), dp[l][r]);
        }
        if(val[l] == val[r]) {
            int sum = 0;
            for(int i = l; i <= r; i++) if(val[i] == val[l]) sum += cnt[i]; // check bounds
            assert(sum < N);
            for(int targ_cnt = 1; targ_cnt <= sum; targ_cnt++) { // optimize this loop?
                dp[l][r] = max(rec2(l, r, targ_cnt) + cube(targ_cnt), dp[l][r]);
            }
        }
//        cerr << "rec_1 " << l << ' ' << r << ' ' << dp[l][r] << endl;
        return dp[l][r];
    }

    long rec2(int l, int curr, int targ_cnt) {
        if(mark2[l][curr][targ_cnt])  return f[l][curr][targ_cnt];
        mark2[l][curr][targ_cnt] = true;
        f[l][curr][targ_cnt] = -LINF;
        if(l == curr) {
//            cerr << "rec_2_base " << l << ' ' << curr << ' ' << targ_cnt << ' ' << cnt[curr] << endl;
            if(targ_cnt == cnt[curr]) return f[l][curr][targ_cnt] = 0;
            else return f[l][curr][targ_cnt];
        }
        int i = pre[curr];
        while(i != pre[l]) {
            f[l][curr][targ_cnt] = max(rec2(l, i, targ_cnt - cnt[curr]) + rec1(i + 1, curr - 1), f[l][curr][targ_cnt]);
//            cerr << "iterate_rec_2 " << l << ' ' << curr << ' ' << targ_cnt << ' ' << targ_cnt - cnt[curr] << ' ' << rec2(l, i, targ_cnt - cnt[curr]) << ' ' << i << ' ' << rec1(i + 1, curr - 1) << endl;
            i = pre[i];
        }
        if(f[l][curr][targ_cnt] < 0) f[l][curr][targ_cnt] = -LINF;
//        cerr << "rec_2 " << l << ' ' << curr << ' ' << targ_cnt << ' ' << f[l][curr][targ_cnt] << endl;
        return f[l][curr][targ_cnt];
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0; i < n; i++) cin >> org[i];
        int last = 0;
        for(int i = 0; i < n; i++) {
            if(i && org[i] != org[i-1]) {
                val.push_back(org[i-1]);
                cnt.push_back(i - last);
                last = i;
            }
        }
        val.push_back(org[n-1]);
        cnt.push_back(n - last);
        n = val.size(); // notice change of definition
        for(int i = 0; i < n; i++) ls[val[i]].push_back(i);
        for(int x = 0; x < N; x++) {
            int last = -1;
            for(int i: ls[x]) {
                pre[i] = last;
                last = i;
            }
        }
//        for(int i = 0; i < n; i++) cerr << val[i] << ' ' << cnt[i] << endl;
        long res = rec1(0, n-1);
        cout << res << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Boxes *obj = new Boxes();
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
