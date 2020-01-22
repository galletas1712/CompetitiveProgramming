// Use idea of prefix sums. Try to calculate them while making the area dimensions of each query as large as possible.
#include "treasure.h"
#include <bits/stdc++.h>
using namespace std;

int a[101][101];
map<tuple<int,int,int,int>, int> mp;
int query(int x1, int y1, int x2, int y2) {
    if(mp.count(make_tuple(x1, y1, x2, y2))) return mp[make_tuple(x1, y1, x2, y2)];
    return mp[make_tuple(x1, y1, x2, y2)] = countTreasure(x1, y1, x2, y2);
}
void findTreasure (int N) {
    for(int i = 1; i <= N; i++) for(int j = 1; j <= N; j++) {
        if(i > N/2 && j > N/2) {
            a[i][j] = query(1, 1, i, j);
        } else if(i > N/2 && j <= N/2) {
            a[i][j] = query(1, 1, i, N) - query(1, j+1, i, N);
        } else if(i <= N/2 && j > N/2) {
            a[i][j] = query(1, 1, N, j) - query(i+1, 1, N, j);
        } else {
            a[i][j] = query(1, 1, N, N) - query(i+1, 1, N, N) - query(1, j+1, N, N) + query(i+1, j+1, N, N);
        }
    }
    for(int i = 1; i <= N; i++) for(int j = 1; j <= N; j++) if(a[i][j] - a[i-1][j] - a[i][j-1] + a[i-1][j-1]) Report(i, j);
}
