//https://github.com/SpeedOfMagic/CompetitiveProgramming/blob/master/CCO/18-GradientDescent.cpp
#include <bits/stdc++.h>
using namespace std;

int R, C, K;

map<pair<int,int>, int> mp;
int query(int x, int y) {
	if(mp.count(make_pair(x, y))) return mp[{x, y}];
	cout << "? " << x << ' ' << y << endl;
	int res;
	cin >> res;
	mp[{x, y}] = res;
	return res;
}

int main() {
	cin >> R >> C >> K;
	int r_s = 1, r_e = R, c_s = 1, c_e = C;
	while(r_s < r_e-1 || c_s < c_e-1) {
		int r_mid = r_s + r_e >> 1, c_mid = c_s + c_e >> 1;
		int ret = query(r_mid, c_mid);
		if(r_s < r_e-1) {
			int r_ret = query(r_mid + 1, c_mid);
			if(ret < r_ret) r_e = r_mid;
			else r_s = r_mid;
		}
		if(c_s < c_e-1) {
			int c_ret = query(r_mid, c_mid + 1);
			if(ret < c_ret) c_e = c_mid;
			else c_s = c_mid;
		}
	}
	cout << "! " << min(min(query(r_s, c_s), query(r_s, c_e)), min(query(r_e, c_s), query(r_e, c_e))) << endl;
}
