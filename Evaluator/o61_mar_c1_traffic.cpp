#include "traffic.h"
#include <bits/stdc++.h>
using namespace std;

int n, k;
vector<int> xs, ys;
vector<tuple<int,int,int,int> > ans;
int ax1, ay1, ax2, ay2, bx1, by1, bx2, by2;
int main() {
    traffic_init(&n, &k);
    for(int i = 1; i <= n; i++) {
        if(traffic_query(i, 1, i, n) > n-1)
            xs.push_back(i);
    }
    for(int i = 1; i <= n; i++) {
        if(traffic_query(1, i, n, i) > n-1)
            ys.push_back(i);
    }
    for(int x: xs) {
        int l = 2, r = n;
        while(l < r) {
            int mid = l+r >> 1;
            if(traffic_query(x, 1, x, mid) > mid-1) r = mid;
            else l = mid+1;
        }
        ans.emplace_back(x, l-1, x, l);
        if(k == 2 && xs.size() + ys.size() == 1) {
            int l = 2, r = n;
            while(l < r) {
                int mid = l+r >> 1;
                if(traffic_query(x, 1, x, mid) > mid) r = mid;
                else l = mid+1;
            }
            ans.emplace_back(x, l-1, x, l);
        }
    }
    for(int y: ys) {
        int l = 2, r = n;
        while(l < r) {
            int mid = l+r >> 1;
            if(traffic_query(1, y, mid, y) > mid-1) r = mid;
            else l = mid+1;
        }
        ans.emplace_back(l-1, y, l, y);
        if(k == 2 && xs.size() + ys.size() == 1) {
            int l = 2, r = n;
            while(l < r) {
                int mid = l+r >> 1;
                if(traffic_query(1, y, mid, y) > mid) r = mid;
                else l = mid+1;
            }
            ans.emplace_back(l-1, y, l, y);
        }
    }
    tie(ax1, ay1, ax2, ay2) = ans[0];
    if(k == 2)
        tie(bx1, by1, bx2, by2) = ans[1];
    traffic_report(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
}
