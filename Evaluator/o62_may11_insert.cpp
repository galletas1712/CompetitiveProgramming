#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 1;

int n, a[N];
char sign[N];
long pref[N], pref_abs[N], ans;
vector<int> subs;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n;
    sign[1] = '+';
    for(int i = 0; i < 2*n - 1; i++) {
        if (i % 2) cin >> sign[i / 2 + 2];
        else cin >> a[i / 2 + 1];
    }
    for(int i = 1; i <= n; i++) {
        pref[i] = pref[i-1] + a[i] * (sign[i] == '+' ? 1 : -1);
        pref_abs[i] = pref_abs[i-1] + a[i];
        if(sign[i] == '-') {
            subs.push_back(i);
        }
    }
    long ans = pref[n];
    for(int i = 1; i < subs.size(); i++) {
        long s1 = pref[subs[i-1] - 1];
        long s2 = pref_abs[subs[i] - 1] - pref_abs[subs[i-1] - 1];
        long s3 = pref_abs[n] - pref_abs[subs[i] - 1];
        ans = max(s1 - s2 + s3, ans);
    }
    cout << ans << endl;
}
