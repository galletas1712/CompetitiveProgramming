#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class SUBLEX {

    int n, q, k, sa[N], pos[N], tmp[N], lcp[N];
    long f[N];
    char s[N];

    void buildSA() {
        int gap;
        for(int i = 0; i < n; i++) sa[i] = i, pos[i] = s[i];
        auto cmp = [&] (int i, int j) {
            if(pos[i] != pos[j]) return pos[i] < pos[j];
            i += gap, j += gap;
            return i < n && j < n ? pos[i] < pos[j] : i > j;
        };
        for(gap = 1; ; gap <<= 1) {
            sort(sa, sa + n, cmp);
            for(int i = 1; i < n; i++) tmp[i] = tmp[i-1] + cmp(sa[i-1], sa[i]);
            for(int i = 0; i < n; i++) pos[sa[i]] = tmp[i];
            if(tmp[n - 1] == n - 1) break;
        }
    }

    void kasai() {
        for(int i = 0, k = 0; i < n; i++, k = (k ? k - 1 : 0)) {
            if(pos[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[pos[i] + 1];
            while(i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
            lcp[pos[i]] = k;
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> s;
        n = strlen(s);
        buildSA();
        kasai();
        f[0] = n - sa[0];
        for(int i = 1; i < n; i++) {
            f[i] = n - sa[i] - lcp[i-1];
        }
        for(int i = 1; i < n; i++) f[i] += f[i-1];
        cin >> q;
        while(q--) {
            int k;
            cin >> k;
            int l = 0, r = n - 1;
            while(l < r) {
                int mid = l+r >> 1;
                if(f[mid] >= k) r = mid;
                else l = mid + 1;
            }
            int ed = n - (f[l] - k);
            for(int i = sa[l]; i < ed; i++) cout << s[i];
            cout << '\n';
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        SUBLEX *obj = new SUBLEX();
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
