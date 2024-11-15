#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acosl(-1);
const int LEN = 3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

#define STRONG 0
#define WEAK 1
#define SCALE 100

ld cos_2nd(const ld& a, const ld& b, const ld& c) {
	ld num = sq(a) + sq(b) - sq(c);
	ld den = 2 * a * b;
	ld t = num / den;
	return std::abs(acosl(std::min(std::max(t, -(ld)1.0), (ld)1.0)));
}
struct Pos3D {
	int x, y, z;
	Pos3D(int x_ = 0, int y_ = 0, int z_ = 0) : x(x_), y(y_), z(z_) {}
	ll operator * (const Pos3D& p) const { return (ll)x * p.x + (ll)y * p.y + (ll)z * p.z; }
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D& operator *= (const int& n) { x *= n; y *= n; z *= n; return *this; }
	ll Euc() const { return *this * *this; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
};
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos c_ = Pos(), ld r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && eq(r, q.r); }
	bool operator < (const Circle& q) const { return c == q.c ? r < q.r : c < q.c; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool outside(const Circle& q) const { return sign((c - q.c).Euc() - sq(r + q.r)) > 0; }
	bool inside(const Circle& q) const { return sign((c - q.c).Euc() - sq(r - q.r)) <= 0; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	inline friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ld ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	Vld ret = {};
	ret.push_back(norm(rd - h));
	if (zero(h)) return ret;
	ret.push_back(norm(rd + h));
	return ret;
}
bool check(const Circle& A, const Circle& B, const Circle& C) {
	Pos a = A.c;
	Vld AB = intersections(A, B);
	Vld AC = intersections(A, C);
	if (AB.empty() || AC.empty()) return 0;
	Pos b1 = A.p(AB[0]), b2 = Pos();
	if (AB.size() > 1) b2 = A.p(AB[1]);
	Pos c1 = A.p(AC[0]), c2 = Pos();
	if (AC.size() > 1) c2 = A.p(AC[1]);
	if (AB.size() == 1 && AC.size() == 1) return b1 == c1;
	if (AB.size() == 1) return inside(c2, a, c1, b1);
	if (AC.size() == 1) return inside(b2, a, b1, c1);
	return inside(c2, a, c1, b1) || inside(c2, a, c1, b2) || inside(b2, a, b1, c1) || inside(b2, a, b1, c2);
}
bool check(const Pos& a, const Pos& b, const Pos& c, const ld& r, const int& seq) {
	Circle A = { a, r * 3 }, B = { b, r }, C = { c, r * 3 };
	if (seq != 1) A.r = r, B.r = r, C.r = r * 5;
	if (A.outside(B) || B.outside(C) || C.outside(A)) return 0;
	if (A.inside(B) || B.inside(C) || C.inside(A)) return 1;
	return check(A, B, C) || check(B, C, A) || check(C, A, B);
}
ld bi_search(const Pos& a, const Pos& b, const Pos& c, ld s, ld e, const int& seq = 1) {
	int cnt = 30; while (cnt--) {
		ld m = (s + e) * .5;
		if (check(a, b, c, m, seq)) e = m;
		else s = m;
	}
	return (s + e) * .5;
}
void query(const int& t) {
	Pos3D q1, q2, q3;
	std::cin >> q1 >> q2 >> q3;
	q1 *= SCALE; q2 *= SCALE; q3 *= SCALE;
	ld l1 = (q1 - q2).mag(), l2 = (q2 - q3).mag(), l3 = (q3 - q1).mag();
	Vld vd = { l1, l2, l3 };
	std::sort(vd.begin(), vd.end());
	ld d1 = vd[0], d2 = vd[1], d3 = vd[2];
	if (eq(d3, (d1 + d2))) { std::cout << "Case #" << t << ": " << d3 / 6 / SCALE << "\n"; return; }
	ld th = cos_2nd(d1, d3, d2);
	Pos a = O, b = Pos(d1, 0).rot(th), c = Pos(d3, 0);
	ld r1 = bi_search(a, b, c, TOL, d3, 1);
	ld r2 = bi_search(a, b, c, TOL, d3, 2);
	std::cout << "Case #" << t << ": " << std::min(r1, r2) / SCALE << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	int T; std::cin >> T;
	for (int t = 1; t <= T; t++) query(t);
	return;
}
int main() { solve(); return 0; }//boj24004 Blackhole

/*

1
1 0 0
0 1 0
0 0 1
0.3698193848
0.369819407659825345
0.369819407659848
0.369819407659826
0.369819407431317

1
1000 0 0
0 1000 0
0 0 1000
369.819407659825345
369.819407659848821
369.819407659848480

1
0 1 0
0 -1 0
5 0 0

1
-250 250 0
-250 -250 0
750 0 0
250.000000000000

1
1000 1000 1000
1000 -1000 -1000
-1000 1000 -1000
739.638815319696164
739.638815319697756
739.638815319696505
739.638815319697756
739.638815343539363
739.638815319101127
739.638815319697301
739.638815319697301

1
-1000 0 0
1000 0 0
-800 1 1
333.333333333333258

1
-1000 0 0
1000 0 0
0 340 0
333.340724243661384

*/
