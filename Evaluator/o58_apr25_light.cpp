#include <bits/stdc++.h>
using namespace std;

int dsu[(int) 4e5 + 1];

int root(int v) {
    return dsu[v] < 0 ? v : dsu[v] = root(dsu[v]);
}

void merge(int u, int v) {
    if((u = root(u)) == (v = root(v))) return;
    dsu[u] += dsu[v];
    dsu[v] = u;
}

bool solve() {
    int n, m;
    cin >> n >> m;

    fill(dsu, dsu + 4*n*m, -1);
    vector<vector<int> > a(n, vector<int>(m));
    vector<vector<int> > pre(n, vector<int>(m, -1));
    vector<vector<int> > nxt(n, vector<int>(m, -1));
    vector<vector<array<int, 2> > > options(n, vector<array<int, 2> >(m));

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> a[i][j];
            options[i][j] = {-1, -1};
        }
    }
    for(int i = 0; i < n; i++) {
        if(a[i][0] > 0) pre[i][0] = 0;
        for(int j = 1; j < m; j++) {
            pre[i][j] = (a[i][j] > 0 ? j : pre[i][j-1]);
        }
        if(a[i][m-1] > 0) nxt[i][m-1] = m-1;
        for(int j = m-2; j >= 0; j--) {
            nxt[i][j] = (a[i][j] > 0 ? j : nxt[i][j+1]);
        }
        for(int j = 0; j < m; j++) if(!a[i][j]) {
            if(pre[i][j] != -1 && nxt[i][j] != -1 && a[i][pre[i][j]] == a[i][nxt[i][j]]) {
                options[i][j][0] = a[i][pre[i][j]];
            }
        }
    }

//    for(int i = 0; i < n; i++, cerr << endl) for(int j = 0; j < m; j++) {
//            cerr << pre[i][j] << ' ';
//        }
//    cerr << endl;
//    for(int i = 0; i < n; i++, cerr << endl) for(int j = 0; j < m; j++) {
//            cerr << nxt[i][j] << ' ';
//        }
//    cerr << endl;

    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) pre[i][j] = nxt[i][j] = -1;

    for(int j = 0; j < m; j++) {
        if(a[0][j] > 0) pre[0][j] = 0;
        for(int i = 1; i < n; i++) {
            pre[i][j] = (a[i][j] > 0 ? i : pre[i-1][j]);
        }
        if(a[n-1][j] > 0) nxt[n-1][j] = n-1;
        for(int i = n-2; i >= 0; i--) {
            nxt[i][j] = (a[i][j] > 0 ? i : nxt[i+1][j]);
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) if(!a[i][j]) {
            if(pre[i][j] != -1 && nxt[i][j] != -1 && a[pre[i][j]][j] == a[nxt[i][j]][j]) {
                if(options[i][j][0] == -1) options[i][j][0] = a[pre[i][j]][j];
                else options[i][j][1] = a[pre[i][j]][j];
            }
        }
    }

//    for(int i = 0; i < n; i++, cerr << endl) for(int j = 0; j < m; j++) {
//        cerr << pre[i][j] << ' ';
//    }
//    cerr << endl;
//    for(int i = 0; i < n; i++, cerr << endl) for(int j = 0; j < m; j++) {
//        cerr << nxt[i][j] << ' ';
//    }
//    cerr << endl;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(a[i][j]) continue;
//            cerr << i << ' ' << j << ' ' << options[i][j][0] << ' ' << options[i][j][1] << endl;
            if(options[i][j][0] == -1) return false;
            if(options[i][j][1] != -1) {
                merge(options[i][j][0], options[i][j][1] + n*m);
                merge(options[i][j][1], options[i][j][0] + n*m);
            } else {
                merge(0, options[i][j][0]);
            }
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(root(a[i][j]) == root(a[i][j] + n*m)) return false;
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while(t--) {
        cout << (solve() ? "YES": "NO") << endl;
    }
}
