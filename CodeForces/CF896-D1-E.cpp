/*
 * Main observation: we can decrease range by x in O(x) time and max - x in O(max - x) time, depending on the inequality x <= 2*max
 * Use DSU to group same values together so we don't need to deep move the values
 */
#include <bits/stdc++.h>
using namespace std;
#define long long long
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const double EPS = 1e-10;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
const int N = 1e5+100;

class TaskENew2 {

    int n, q, SQ, a[N], blk[N];

    int mx[450], tag[450], dsu[450][N];
    // tag stores relative zero
    // mx excludes tag

    int root(int b, int v) {
        return dsu[b][v] <= 0 ? v : dsu[b][v] = root(b, dsu[b][v]);
    }

    void update(int b, int l, int r, int x) {
        for(int i = l; i <= r; i++) a[i] = root(b, a[i]);
        for(int i = l; i <= r; i++) if(a[i] + tag[b] > x) {
            ++dsu[b][a[i]];
            a[i] -= x;
            --dsu[b][a[i]];
        }
    }

    void update(int b, int x) {
        if(mx[b] + tag[b] < x) return;
        if(2*x <= mx[b] + tag[b]) {
            for(int i = 1 - tag[b]; i <= x - tag[b]; i++) {
                dsu[b][root(b, i + x)] += dsu[b][root(b, i)];
                dsu[b][root(b, i)] = 0;
                dsu[b][i] = i + x;
            }
            tag[b] -= x;
        } else {
            for(int i = mx[b]; i >= x + 1 - tag[b]; i--) {
                dsu[b][root(b, i - x)] += dsu[b][root(b, i)];
                dsu[b][root(b, i)] = 0;
                dsu[b][i] = i - x;
            }
            mx[b] = x - tag[b];
        }
    }

    int query(int b, int l, int r, int x) {
        for(int i = l; i <= r; i++) a[i] = root(b, a[i]);
        int ans = 0;
        for(int i = l; i <= r; i++) {
            ans += a[i] + tag[b] == x;
//            cerr << "point_query " << b << ' ' <<  x << ' ' << i << ' ' << a[i] + tag[b] << endl;
        }
        return ans;
    }

    int query(int b, int x) {
        if(x - tag[b] < 0 || x - tag[b] > mx[b]) return 0;
//        cerr << "block_query " << b << ' ' << x << ' ' << -dsu[b][root(b, x - tag[b])] << endl;
        return -dsu[b][root(b, x - tag[b])];
    }

public:
    void solve(istream &cin, ostream &cout) {
        cin >> n >> q;
        SQ = 1 + sqrt(n);
        for(int i = 0; i < n; i++) {
            cin >> a[i];
            blk[i] = i / SQ;
            mx[blk[i]] = max(a[i], mx[blk[i]]);
            --dsu[blk[i]][a[i]];
        }
        for(int z = 0, inst, l, r, x; z < q; z++) {
            cin >> inst >> l >> r >> x;
            --l, --r;
            if(inst == 1) {
                if(blk[l] != blk[r]) {
                    update(blk[l], l, (blk[l] + 1) * SQ - 1, x);
                    update(blk[r], blk[r] * SQ, r, x);
                    for(int i = blk[l] + 1; i < blk[r]; i++) update(i, x);
                } else {
                    update(blk[l], l, r, x);
                }
            } else {
                int ans = 0;
                if(blk[l] != blk[r]) {
                    ans += query(blk[l], l, (blk[l] + 1) * SQ - 1, x);
                    ans += query(blk[r], blk[r] * SQ, r, x);
                    for(int i = blk[l] + 1; i < blk[r]; i++) ans += query(i, x);
                } else {
                    ans += query(blk[l], l, r, x);
                }
                cout << ans << '\n';
            }
        }
    }
};

class Solver {
public:
    void solve(std::istream& in, std::ostream& out) {
        TaskENew2 *obj = new TaskENew2();
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
