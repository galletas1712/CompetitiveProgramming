/*
For K = 0, the cost for a certain x is sum of |x-(B[i]-A[i])|, which can be handled using the slope trick
For K != 0, we can prove that if we sort points by x-intercept = B[i] - A[i], the n - K points that we will use form a contiguous segment
Therefore, we can just use a sliding window. Luckily, deleting a point from the set is easy (just do the reverse of appending)
*/
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+1;

class Simfonija {

    int n, k, A[N], B[N];
    vector<long> ord;

    long C, ans = LINF;
    multiset<long> l, r;

    void rebalance() {
        while(l.size() > r.size()) r.insert(*l.rbegin()), l.erase(prev(l.end()));
        while(r.size() > l.size()) l.insert(*r.begin()), r.erase(r.begin());
    }

    void add(long x) {
        if(l.empty() && r.empty()) {
            l.insert(x);
            r.insert(x);
            return;
        }
        if(r.empty() || x <= *l.rbegin()) {
            C += *l.rbegin() - x;
            l.insert(x);
            l.insert(x);
        } else if(l.empty() || x >= *r.begin()) {
            C += x - *r.begin();
            r.insert(x);
            r.insert(x);
        } else { // in between
            // no need to update C as originating from same point
            l.insert(x);
            r.insert(x);
        }

        rebalance();
    }

    void remove(long x) {
        bool inLeft = l.find(x) != l.end();
        bool inRight = r.find(x) != r.end();
        if(inLeft && !inRight) {
            l.erase(l.find(x));
            l.erase(l.find(x));
        } else if(!inLeft && inRight) {
            r.erase(r.find(x));
            r.erase(r.find(x));
        } else if(inLeft && inRight){
            l.erase(l.find(x));
            r.erase(r.find(x));
        } else {
            return;
        }

        rebalance();

        if(l.empty() && r.empty()) {
            C = 0;
            return;
        }
        if(r.empty() || x <= *l.rbegin()) {
            C -= *l.rbegin() - x;
        } else if(l.empty() || x >= *r.begin()) {
            C -= x - *r.begin();
        }
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> k;
        k = n - k; // use complement segment instead
        for(int i = 0; i < n; i++) cin >> A[i];
        for(int i = 0; i < n; i++) {
            cin >> B[i];
            ord.push_back(B[i] - A[i]);
        }
        sort(ord.begin(), ord.end());
        for(int i = 0; i < k; i++) {
            add(ord[i]);
        }
        ans = min(C, ans);
        for(int i = k; i < n; i++) {
            remove(ord[i - k]);
            add(ord[i]);
            ans = min(C, ans);
        }
        cout << ans << endl;
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        Simfonija *obj = new Simfonija();
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
