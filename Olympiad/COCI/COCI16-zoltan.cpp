// Hint 1: problem is equivalent to assigning each element to left or right partition, where right partition is in order and left partition is in reverse.
// Hint 2: list array from start to end and end to start to see pattern.
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9+7;
const int N = 2e5+1;

void update(vector<int>& f, int i, int v) {
    ++i;
    while(i < f.size()) {
        f[i] += v;
        f[i] %= MOD;
        i += i & -i;
    }
}
int query(vector<int>& f, int i) {
    ++i;
    int ret = 0;
    while(i > 0) {
        ret += f[i];
        ret %= MOD;
        i -= i & -i;
    }
    return ret;
}

int len[2*N], pre[2*N], curr[2*N];
vector<int> a, f, dp[N];

pair<int, int> count_LIS() {
    fill(len, len+a.size()+1, 0);

    int LIS = 0;
    for(int i = 0; i < a.size(); i++) {
        int x = a[i];
        auto it = lower_bound(f.begin(), f.end(), x);
        len[i] = it - f.begin() + 1;
        assert(len[i] < N);
        LIS = max(len[i], LIS);
        if(it == f.end())
            f.push_back(x);
        else
            *it = x;
    }
    f.clear();

    dp[0].push_back(0);
    for(int i = 0; i < a.size(); i++)
        dp[len[i]].push_back(a[i]);
    for(int i = 0; i <= LIS; i++) {
        sort(dp[i].begin(), dp[i].end());
        dp[i].resize(unique(dp[i].begin(), dp[i].end()) - dp[i].begin());
    }
    for(int i = 0; i < a.size(); i++) {
        auto it = lower_bound(dp[len[i] - 1].begin(), dp[len[i] - 1].end(), a[i]);
        pre[i] = it - dp[len[i] - 1].begin() - 1;
        auto it2 = lower_bound(dp[len[i]].begin(), dp[len[i]].end(), a[i]);
        curr[i] = it2 - dp[len[i]].begin();
    }

    for(int i = 0; i <= LIS; i++) {
        dp[i].assign(dp[i].size() + 2, 0);
    }
    update(dp[0], 0, 1);
    for(int i = 0; i < a.size(); i++) { // problems are here?
        int res = query(dp[len[i] - 1], pre[i]);
        update(dp[len[i]], curr[i], res);
    }
    return {query(dp[LIS], dp[LIS].size() - 2), LIS};
}

void solve(istream &cin) {
    int n;
    cin >> n;
    a.resize(2*n - 1);
    for(int i = 0; i < n; i++) cin >> a[i];
    reverse(a.begin(), a.begin() + n);
    for(int i = n-2, curr = 0; i >= 0; i--, curr++) a[n + curr] = a[i];
    auto withStart = count_LIS();
    a.erase(a.begin() + a.size() / 2);
    auto withoutStart = count_LIS();
    if(withStart.second == withoutStart.second) {
        withStart.first -= withoutStart.first;
    } else if(withStart.second > withoutStart.second){
        withoutStart.first = 0;
    } else {
        assert(false);
    }
    for(int i = 0; i < n - withStart.second; i++) {
        withStart.first *= 2;
        withStart.first %= MOD;
    }
    for(int i = 0; i < n - withoutStart.second - 1; i++) {
        withoutStart.first *= 2;
        withoutStart.first %= MOD;
    }
    cout << withStart.second << ' ' << (withStart.first + withoutStart.first) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    solve(cin);
}
