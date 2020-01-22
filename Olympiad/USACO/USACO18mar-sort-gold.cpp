#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class OutOfSorts {
public:
    void solve(istream &cin, ostream &cout) {
        int n;
        vector<int> a, b;
        multiset<int> sta, stb;
        cin >> n;
        a.resize(n);
        for(int i = 0; i < n; i++) {
            cin >> a[i];
            b.push_back(a[i]);
        }
        sort(b.begin(), b.end());
        int ans = 1;
        for(int i = 0; i < n; i++) {
            if(stb.count(a[i])) {
                stb.erase(stb.find(a[i]));
            } else {
                sta.insert(a[i]);
            }
            if(sta.count(b[i])) {
                sta.erase(sta.find(b[i]));
            } else {
                stb.insert(b[i]);
            }
            assert(sta.size() == stb.size());
            ans = max((int) sta.size(), ans);
        }
        cout << ans;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        OutOfSorts *obj = new OutOfSorts();
        obj->solve(in, out);
        delete obj;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Solver solver;
    std::ifstream in("sort.in");
    std::ofstream out("sort.out");
    solver.solve(in, out);
    return 0;
}
