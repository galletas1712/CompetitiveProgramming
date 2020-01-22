#include <bits/stdc++.h>
using namespace std;
const int N = 1e5+10;

int n, m;
char s[N];
int sa[N], pos[N], tmp[N], lcp[N], t[2*N];
set<int> st;

void buildSA() {
    int gap;
    auto cmp = [&] (int i, int j) {
        if(pos[i] != pos[j]) return pos[i] < pos[j];
        i += gap;
        j += gap;
        return ((i < N && j < N) ? pos[i] < pos[j] : i > j);
    };

    for(int i = 0; i < n; i++) sa[i] = i, pos[i] = s[i];


    for(gap = 1; ; gap <<= 1) {
        sort(sa, sa+n, cmp);
        for(int i = 1; i < n; i++) tmp[i] = tmp[i-1] + cmp(sa[i-1], sa[i]);
        for(int i = 0; i < n; i++) pos[sa[i]] = tmp[i];
        if(tmp[n-1] == n-1) break;
    }
}

void buildLCP() {
    for(int i = 0, k = 0; i < n; i++, k = (k ? k-1 : 0)) {
        if(pos[i] == n-1) {
            k = 0;
            continue;
        }
        int j = sa[pos[i] + 1];
        while(i + k < n && j + k < n && s[i + k] == s[j + k]) ++k;
        lcp[pos[i]] = k;
    }
}

int query(int l, int r) {
    int ret = INT_MAX;
    for(l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
        if(l & 1) ret = min(t[l++], ret);
        if(r & 1) ret = min(t[--r], ret);
    }
    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin >> n >> m >> s;
    buildSA();
    buildLCP();
    for(int i = 0; i < n; i++) t[i + n] = lcp[i];
    for(int i = n-1; i >= 1; i--) t[i] = min(t[i << 1], t[i << 1 | 1]);
    st.insert(pos[0]);
    for(int i = 0, inst, idx; i < m; i++) {
        cin >> inst >> idx;
        --idx;
        idx = pos[idx];
        if(inst == 1) {
            st.insert(idx);
        } else {
            auto it = st.lower_bound(idx);
            if(it != st.end() && *it == idx) {
                cout << n - sa[idx] << '\n';
                continue;
            }
            int res = 0;
            if(it != st.end()) {
                res = max(query(idx, *it - 1), res);
            }
            if(it != st.begin()) {
                it = prev(it);
                res = max(query(*it, idx - 1), res);
            }
            cout << res << '\n';
        }
    }
}
