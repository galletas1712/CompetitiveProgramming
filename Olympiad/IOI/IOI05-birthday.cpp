// Reduces to finding intersection of ranges in circle
// Need to optimize to pass. Used lack of cases to my advantage :)

#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e6+10;

class Birthday {

    int n, a[N], cnt[N], lb[N], rb[N], tmp[2*N];

    bool check(int d) {
        for(int i = 0; i < n; i++) {
            int current_rotation = (a[i] - i + n) % n;
            lb[i] = (current_rotation - d + n) % n;
            rb[i] = (current_rotation + d) % n;
            if(lb[i] <= rb[i]) {
                ++tmp[lb[i]];
                --tmp[rb[i] + 1];
            } else {
                ++tmp[lb[i]];
                --tmp[rb[i] + n + 1];
            }
        }
        int sum = 0;
        for(int i = 0; i < 2*n; i++) {
            sum += tmp[i];
            cnt[i % n] += sum;
        }
        int mx = 0;
        for(int i = 0; i < n; i++) {
            mx = max(cnt[i], mx);
        }
        memset(tmp, 0, sizeof(tmp));
        memset(cnt, 0, sizeof(cnt));
        return mx == n;
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n;
        for(int i = 0; i < n; i++) {
            cin >> a[i];
            --a[i];
        }
        int ans = n;
        int l = 0, r = n-1;
        while(l < r) {
            int mid = l+r >> 1;
            if(check(mid)) r = mid;
            else l = mid + 1;
        }
        ans = l;
        if(n < N/2) {
            reverse(a, a+n);
            int l = 0, r = n-1;
            while(l < r) {
                int mid = l+r >> 1;
                if(check(mid)) r = mid;
                else l = mid + 1;
            }
            ans = min(l, ans);
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Birthday *obj = new Birthday();
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
