// Notice that if we draw a line between the i and (i+2) mod k, no points can lie outside of the hull, as otherwise it won't be a convex hull
// So, we simply need to find the number of points that create a left turn between i, p, (i+2) mod k
#include <bits/stdc++.h>
#define long int64_t
using namespace std;

struct vec {
    long x, y;
    vec(long x, long y): x(x), y(y) {}
    long operator^(const vec& rhs) const {
        return x*rhs.y - rhs.x*y;
    }
    bool operator==(const vec& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

int n, ans;
vector<vec> a, hull;
vector<int> idx;

bool bad(vec p1, vec p2, vec p3) {
//    cerr << "bad_request " << p1.x << ' ' << p1.y << ' ' << p2.x << ' ' << p2.y << ' ' << p3.x << ' ' << p3.y << endl;
    vec A = vec(p2.x - p1.x, p2.y - p1.y);
    vec B = vec(p3.x - p2.x, p3.y - p2.y);
    return (A ^ B) <= 0;
}

bool colinear_p1(vec p1, vec p2, vec p3) {
    return (vec(p2.x - p1.x, p2.y - p1.y) ^ vec(p3.x - p1.x, p3.y - p1.y)) == 0;
}

int main() {
    cin >> n;
    for(int i = 0, x, y; i < n; i++) {
        cin >> x >> y;
        a.emplace_back(x, y);
        if(y < a[0].y || (y == a[0].y && x < a[0].x))
            swap(a.back(), a[0]);
    }
    sort(a.begin() + 1, a.end(), [] (vec x, vec y) {
        vec A = vec(x.x - a[0].x, x.y - a[0].y);
        vec B = vec(y.x - a[0].x, y.y - a[0].y);
        long cross = A ^ B;
        if(!cross) {
            return A.x * A.x + A.y * A.y < B.x * B.x + B.y * B.y;
        }
        return cross > 0;
    });
    for(int i = 0; i < min((int) a.size(), 3); i++) {
        hull.push_back(a[i]);
        idx.push_back(i);
    }
    for(int i = 3; i < a.size(); i++) {
        while(hull.size() >= 3 && bad(hull[hull.size() - 2], hull.back(), a[i])) {
            hull.pop_back();
            idx.pop_back();
        }
        hull.push_back(a[i]);
        idx.push_back(i);
    }
    while(hull.size() >= 3 && bad(hull[hull.size() - 2], hull.back(), hull[0])) {
        hull.pop_back();
        idx.pop_back();
    }

//    cerr << "POINTS" << endl;
//    for(auto p: a) cerr << p.x << ' ' << p.y << endl;
//    cerr << "HULL" << endl;
//    assert(hull.size() == idx.size());
//    for(int i = 0; i < hull.size(); i++) {
//        cerr << hull[i].x << ' ' << hull[i].y << ' ' << idx[i] << endl;
//    }

    if(hull.size() < 3) {
        cout << 0 << endl;
        return 0;
    }

    for(int cen = 0; cen < hull.size(); cen++) {
        int l = idx[(cen + hull.size() - 1) % hull.size()];
        int r = idx[(cen + 1) % hull.size()];
        vector<vec> pts;
//        cerr << "l " << l << " r " << r << endl;
        for(int i = l; i != r; i = (i+1) % a.size()) pts.push_back(a[i]);
        pts.push_back(a[r]);
        for(int i = l-1; i >= 0; i--) {
            if(!colinear_p1(a[0], a[i], a[l])) break;
            pts.push_back(a[i]);
        }

        int res = 0;
        for(vec p: pts) {
            vec A = vec(p.x - a[l].x, p.y - a[l].y);
            vec B = vec(a[r].x - p.x, a[r].y - p.y);
            res += (A ^ B) >= 0;
        }
        ans = max(res, ans);
    }
    cout << ans << endl;
}
