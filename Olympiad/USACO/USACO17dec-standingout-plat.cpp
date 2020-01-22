/**
 * code generated by JHelper
 * More info: https://github.com/AlexeyDmitriev/JHelper
 * @author
 */

/*
 * Main idea is just to find nearest elements in the suffix array with different string ids and take the complement of their LCP
 * Need to add separators because, for example, the case ab, abab the LCP will exceed the boundaries of the string
 * Simply doing min with the string length is not enough, as we also need to min with the other string length which makes it tedious to implement
 * Therefore, using separators is an effiicent and easy way to make sure this doesn't happen
 * Also, make sure to not double count. To do this, also compute LCP with elements of same id that were previously explored to avoid this
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 2e5+1;

class standingout {

    int m, n;
    vector<string> ls;
    string s;

    int sa[N], pos[N], tmp[N], lcp[N];
    vector<int> agg_min[N];

    int id[N], lf[N], mx_lcp[N];
    long res[N];

    void build() {
        for(int i = 0; i < n; i++) sa[i] = i, pos[i] = s[i];
        int gap;
        auto cmp = [&] (int i, int j) -> bool {
            if(pos[i] != pos[j]) return pos[i] < pos[j];
            i += gap;
            j += gap;
            if(i < n && j < n) return pos[i] < pos[j];
            else return i > j;
        };
        for(gap = 1; ; gap <<= 1) {
            sort(sa, sa+n, cmp);
            for(int i = 1; i < n; i++) tmp[i] = tmp[i-1] + cmp(sa[i-1], sa[i]);
            for(int i = 0; i < n; i++) pos[sa[i]] = tmp[i];
            if(tmp[n - 1] == n - 1) break;
        }
        for(int i = 0, k = 0; i < n; i++, k = (k ? k-1 : 0)) {
            if(pos[i] == n-1) {
                k = 0;
                continue;
            }
            int j = sa[pos[i] + 1];
            while(i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
            lcp[pos[i]] = k;
        }
        for(int i = 0; i < n; i++) {
            agg_min[i].push_back(lcp[i]);
            for(int j = 0; 1 << j+1 <= i+1; j++)
                agg_min[i].push_back(min(agg_min[i][j], agg_min[i-(1<<j)][j]));
        }
//        for(int i = 0; i < n; i++) cerr << sa[i] << ' ' << id[sa[i]] << ' ' << s.substr(sa[i]) << endl;
    }

    int get_agg_min(int l, int r) {
        if(l > r) return INF;
        int lg = log2(r-l+1);
        return min(agg_min[r][lg], agg_min[l+(1<<lg)-1][lg]);
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> m;
        ls.resize(m+1);
        for(int i = 0; i < m; i++) cin >> ls[i];
        s = "";
        for(int i = 0; i < m; i++) s += ls[i] + '?';
        n = s.length();
        for(int i = 0, sum = 0; i < n; i++) {
            if(s[i] == '?') ++sum, id[i] = m;
            else id[i] = sum;
        }
        build();
        for(int i = 0, last = -1; i < n; i++) {
            if(s[i] == '?') last = i;
            else lf[id[i]] = last;
        }
        for(int i = 1, last = -1; i < n; i++) {
            if(id[sa[i-1]] != id[sa[i]] && id[sa[i-1]] != m) {
                last = i-1;
            }
            if(last == -1 | s[sa[i]] == '?') continue;
            mx_lcp[sa[i]] = max(get_agg_min(last, i-1), mx_lcp[sa[i]]);
//            int mn = INF;
//            for(int j = last; j < i; j++) mn = min(lcp[j], mn);
//            mx_lcp[sa[i]] = max(mn, mx_lcp[sa[i]]);
//            cerr << i << ' ' << sa[i] << ' ' << id[sa[i]] << ' ' << s.substr(sa[i]) << ' ' << last << ' ' << mx_lcp[sa[i]] << endl;
        }
//        cerr << endl;
        for(int i = n-2, last = n; i >= 0; i--) {
            if(id[sa[i+1]] != id[sa[i]] && id[sa[i+1]] != m) {
                last = i+1;
            }
            if(last == n || s[sa[i]] == '?') continue;
            mx_lcp[sa[i]] = max(get_agg_min(i, last-1), mx_lcp[sa[i]]);
//            int mn = INF;
//            for(int j = i; j < last; j++) mn = min(lcp[j], mn);
//            mx_lcp[sa[i]] = max(mn, mx_lcp[sa[i]]);
//            cerr << i << ' ' << sa[i] << ' ' << id[sa[i]] << ' ' << s.substr(sa[i]) << ' ' << last << ' ' << mx_lcp[sa[i]] << endl;
        }
//        cerr << endl;
        vector<int> last(m + 1, -1);
        for(int i = 0; i < n; i++) {
            if(s[sa[i]] == '?') continue;
            mx_lcp[sa[i]] = min((int) ls[id[sa[i]]].length() - (sa[i] - lf[id[sa[i]]] - 1), mx_lcp[sa[i]]);

            if(last[id[sa[i]]] != -1) mx_lcp[sa[i]] = max(get_agg_min(last[id[sa[i]]], i-1), mx_lcp[sa[i]]);
//            int mn = INF;
//            for(int j = last[id[sa[i]]]; j < i; j++) mn = min(lcp[j], mn);
//            mx_lcp[sa[i]] = max(mn, mx_lcp[sa[i]]);

            res[id[sa[i]]] += ls[id[sa[i]]].length() - (sa[i] - lf[id[sa[i]]] - 1) - mx_lcp[sa[i]];
            last[id[sa[i]]] = i;
//            cerr << s.substr(sa[i]) << ' ' << mx_lcp[sa[i]] << endl;
        }
        for(int i = 0; i < m; i++) {
            cout << res[i] << '\n';
        }
    }
};


class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        standingout *obj = new standingout();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::ifstream in("standingout.in");
    std::ofstream out("standingout.out");
    solver.solve(in, out);
    return 0;
}
