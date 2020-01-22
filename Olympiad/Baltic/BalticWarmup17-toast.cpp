// Cosine rule to find radius. Impossible cases answer 0 D.
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};


class Toast {
    int N, T;
    double D;
public:
    void solve(istream& cin, ostream& cout) {
        cin >> N >> D >> T;
        if(T % N || (T / N) >= N/2) {
            cout << 0 << ' ' << D << endl;
            return;
        }
        T /= N;
        double R1 = sqrt(2) * D / sqrt(1 - cos(2*M_PI*(T+1)/N));
        double R2 = sqrt(2) * D / sqrt(1 - cos(2*M_PI*T/N));
        cout << fixed << R1 << ' ' << R2 << endl;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    Toast solver;
    std::istream& in(std::cin);
    std::ostream& out(std::cout);
    solver.solve(in, out);
    return 0;
}
