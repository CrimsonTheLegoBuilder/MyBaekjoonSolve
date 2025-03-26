#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld TOL = 1e-13;
const ld PI = acos(-1);
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

int N;
ll A[LEN];
struct Pdd {
	ld x, y;
	Pdd(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	ld operator / (const Pdd& p) const { return x * p.x + y * p.y; }
	Pdd operator + (const Pdd& p) const { return Pdd(x + p.x, y + p.y); }
	Pdd operator - (const Pdd& p) const { return Pdd(x - p.x, y - p.y); }
	Pdd rot(const ld& t) const { return Pdd(x * cos(t) - y * sin(t), x * sin(t) + y * cos(t)); }
};
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ~ () const { return { -y, x }; }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
	Pdd p() const { return Pdd(x, y); }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld rad(const Pos& d1, const Pos& d2, const Pos& d3) { return rad(d2 - d1, d3 - d1); }
Pdd operator + (const Pos& p, const Pdd& q) { return Pdd(p.x + q.x, p.y + q.y); }
ld operator / (const Pdd& p, const Pos& q) { return p.x * q.y - p.y * q.x; }
Pdd operator - (const Pdd& p, const Pos& q) { return Pdd(p.x - q.x, p.y - q.y); }
ld intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pdd& q2) {
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	//ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	//if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return a1;
}
Pos bi_search(const Polygon& H, const Pos& c, const int& i1, const int& i2, ld& ix) {
	int sz = H.size();
	auto fit = [&](const int& i) -> int { return i % sz; };
	int f = ccw(c, H[i1], H[i2]);
	assert(f);
	Pos v = H[i2] - H[i1];
	Pos h2 = f > 0 ? H[i2] : H[i1] - v;
	ld t = PI - rad(H[i1], h2, c), r;
	Pdd m0 = H[i1] + v.p().rot(t);
	if (f > 0) {
		r = rad(H[i1], h2, H[(i1 - 1 + sz) % sz]);
		if (r <= t) return Pos(-1, -1);
	}
	else {
		r = rad(H[i1], h2, H[(i1 + 1) % sz]);
		if (r >= t) return Pos(-1, -1);
	}
	int s = 1, e = N - 1;
	while (s < e) {
		int m = s + e >> 1;
		r = rad(H[i1], h2, H[fit(i1 + m)]);
		if (eq(t, r)) { ix = 0; return Pos(fit(i1 + m), -1); }
		if (r < t) s = m + 1;
		else e = m;
	}
	ix = intersection(H[fit(i1 + s - 1 + sz)], H[fit(i1 + s)], H[i1], m0);
	return Pos(fit(i1 + s - 1 + sz), fit(i1 + s));
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);
	//freopen("mirror.in", "r", stdin);
	//freopen("mirror.out", "w", stdout);
	std::cin >> N; Polygon H(N); for (Pos& p : H) std::cin >> p;
	Pos c; std::cin >> c;
	for (int i = 0; i < N; i++) A[i + 1] = A[i] + cross(c, H[i], H[(i + 1) % N]);
	for (int i = 0; i < N; i++) {
		int i1 = i, i2 = (i + 1) % N;
		ld ix = 0, al = 0, ar = 0;
		Pos pl = bi_search(H, c, i1, i2, ix);
		if (pl.x == -1) al = 0;
		else {
			if (pl.y == -1) {
				int j = pl.x;
				if (i1 > j) al = A[i1] - A[j] + cross(c, H[i1], H[j]);
				else al = A[N] - (A[j] - A[i1] + cross(c, H[j], H[i1]));
			}
			else {
				int j0 = pl.x, j1 = pl.y;
				if (i1 > j1) al = A[i1] - A[j1] + cross(c, H[i1], H[j1]);
				else al = A[N] - (A[j1] - A[i1] + cross(c, H[j1], H[i1]));
				al += cross(H[i1], H[j0], H[j1]) * (1 - ix);
			}
		}
		Pos pr = bi_search(H, c, i2, i1, ix);
		if (pr.x == -1) ar = 0;
		else {
			if (pr.y == -1) {
				int j = pr.x;
				if (i2 < j) ar = A[j] - A[i2] + cross(c, H[j], H[i2]);
				else ar = A[N] - (A[i2] - A[j] + cross(c, H[i2], H[j]));
			}
			else {
				int j0 = pr.x, j1 = pr.y;
				if (i2 < j0) ar = A[j0] - A[i2] + cross(c, H[j0], H[i2]);
				else ar = A[N] - (A[i2] - A[j0] + cross(c, H[i2], H[j0]));
				ar += cross(H[i2], H[j0], H[j1]) * ix;
			}
		}
		std::cout << (A[N] - al - ar) * .5 << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj28942