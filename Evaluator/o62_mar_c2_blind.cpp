#include <bits/stdc++.h>
using namespace std;
const int N = 3e5+1;

struct info {
    int inst, a, b, c;
    info(int inst, int a, int b, int c): inst(inst), a(a), b(b), c(c) {}
};

int n, m, dsu[N];
vector<info> requests;
vector<int> g[N], ls[N];
int indeg[N];
vector<int> ord;

int root(int v) { return (dsu[v] < 0 ? v : dsu[v] = root(dsu[v])); }

void merge(int u, int v) {
    if((u = root(u)) == (v = root(v))) return;
    dsu[u] += dsu[v];
    dsu[v] = u;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m;
    memset(dsu, -1, sizeof(dsu));
    for(int i = 0, inst, a, b, c; i < m; i++) {
        cin >> inst >> a >> b >> c;
        if(inst == 3) {
            merge(a, b);
            merge(b, c);
        } else {
            requests.emplace_back(inst, a, b, c);
        }
    }
    for(info curr: requests) {
        curr.a = root(curr.a);
        curr.b = root(curr.b);
        curr.c = root(curr.c);
        if(curr.inst == 1) {
            g[curr.a].push_back(curr.b);
            g[curr.a].push_back(curr.c);
            ++indeg[curr.b];
            ++indeg[curr.c];
        } else {
            g[curr.b].push_back(curr.a);
            g[curr.c].push_back(curr.a);
            indeg[curr.a] += 2;
        }
    }
    queue<int> q;
    for(int i = 1; i <= n; i++) if(root(i) == i && !indeg[i]) q.push(i);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        ord.push_back(u);
        for(int v: g[u]) {
            --indeg[v];
            if(!indeg[v]) q.push(v);
        }
    }
    for(int i = 1; i <= n; i++) ls[root(i)].push_back(i);
    reverse(ord.begin(), ord.end());

    cout << ord.size() << '\n';
    for(int x: ord) {
        cout << ls[x].size() << ' ';
        for(int i: ls[x]) cout << i << ' ';
        cout << '\n';
    }
}
