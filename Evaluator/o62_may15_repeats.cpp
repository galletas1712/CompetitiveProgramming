#include <bits/stdc++.h>
using namespace std;

int n, k;
string s;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> k >> s;
    n = s.length();
    int ans = 0;
    for(int gap = 2; gap <= n; gap += 2) {
        int curr_diff = 0;
        for(int i = 0; i < gap / 2; i++) {
            curr_diff += s[i] != s[i + gap / 2];
        }
        if(curr_diff <= k) ++ans;
        for(int i = 1; i + gap - 1 < n; i++) {
            curr_diff -= s[i - 1] != s[i - 1 + gap / 2];
            curr_diff += s[i + gap - 1] != s[i + gap / 2 - 1];
            if(curr_diff <= k) ++ans;
        }
    }
    cout << ans << endl;
}
