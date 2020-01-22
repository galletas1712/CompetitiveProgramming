// Model the requirements as an equation, rearange it and complete the square to make a circle equation
// Then you have to find the common point of the two circles that are minimum
// https://stackoverflow.com/questions/3349125/circle-circle-intersection-points
#include <bits/stdc++.h>
using namespace std;
#define ld long double
const long MOD = 1e9+7, LINF = 1e18 + 1e16;
const int INF = 1e9+1;
const ld EPS = 1e-13;
const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

class HouseLoc {

	ld a, b, x[5], y[5], r[2];
	ld sq(ld x) { return x*x; }

	struct point {
		ld x, y;
		point operator+(point rhs) {
			return (point) {x + rhs.x, y + rhs.y};
		}
		point operator-(point rhs) {
			return (point) {x - rhs.x, y - rhs.y};
		}
		point operator*(ld a) {
			return (point) {x * a, y * a};
		}
		point operator/(ld a) {
			return (point) {x / a, y / a};
		}
		bool operator<(point rhs) {
			if(x < rhs.x - EPS) return true;
			if(abs(x - rhs.x) < EPS && y < rhs.y - EPS) return true;
			return false;
		}
		ld sqsz() {
			return x*x + y*y;
		}
	} P[5];

public:
	void solve(istream &cin, ostream &cout) {
		cout << fixed << setprecision(2);
		cin >> a >> b;
		a *= a;
		b *= b;
		for(int i = 0; i < 4; i++) cin >> x[i] >> y[i];
		P[0] = {(a * x[1] - x[0]) / (a-1), (a * y[1] - y[0]) / (a-1)};
		r[0] = sqrt((sq(x[0]) + sq(y[0]) - a * (sq(x[1]) + sq(y[1]))) / (a-1) + sq(P[0].x) + sq(P[0].y));

		P[1] = {(b * x[3] - x[2]) / (b-1), (b * y[3] - y[2]) / (b-1)};
		r[1] = sqrt((sq(x[2]) + sq(y[2]) - b * (sq(x[3]) + sq(y[3]))) / (b-1) + sq(P[1].x) + sq(P[1].y));

		ld dist2 = (P[1] - P[0]).sqsz();
		ld dist = sqrt(dist2);

		if(abs(P[0].x - P[1].x) < EPS && abs(P[0].y - P[1].y) < EPS && abs(r[0] - r[1]) < EPS) {
			cout << P[0].x - r[0] << ' ' << P[0].y << endl;
			return;
		}

		if(dist - EPS > r[0] + r[1] || dist < abs(r[0] - r[1]) - EPS) {
			cout << "Impossible!";
			return;
		}

		ld A = (sq(r[0]) - sq(r[1]) + dist2) / (2.0 * dist); // distance from P0 (first center) to P2 (intersection of line connecting radii and midpoint of intersections)
		ld H = sqrt(sq(r[0]) - sq(A)); // distance from P2 to intersections
		P[2] = P[0] + (P[1] - P[0]) * A / dist;

		point orth = (P[1] - P[0]) * H / dist; // vector pointing from P2 to intersections
		swap(orth.x, orth.y);
		orth.x *= -1;
		P[3] = P[2] + orth;
		P[4] = P[2] - orth;
		if(P[3] < P[4]) cout << P[3].x << ' ' << P[3].y << endl;
		else cout << P[4].x << ' ' << P[4].y << endl;
	}
};

class Solver {
public:
	void solve(std::istream& in, std::ostream& out) {
	    HouseLoc *obj = new HouseLoc();
		obj->solve(in, out);
		delete obj;
	}
};

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	Solver solver;
	std::istream& in(std::cin);
	std::ostream& out(std::cout);
	solver.solve(in, out);
	return 0;
}
