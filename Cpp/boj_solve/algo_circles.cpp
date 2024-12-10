#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
//typedef long double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 3005;
int N, M, T, Q;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	inline Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	inline ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	inline ld rad() const { return norm(atan2(y, x)); }
	inline friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = sq((ld)r - q.r);
		return r < q.r && dist >= (c - q.c).Euc();
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	inline ld A() const { return 1. * r * r * PI; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	ld rad(const Pos& p) const { return (p - c).rad(); }
	ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
	ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld tz = (cos(lo) - cos(hi)) * m.y * r;
		return fan + tz - (s / e) * r * r * (ld).5;
	}
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	int i;
	Arc(ld LO = 0, ld HI = 0, int I = -1) : lo(LO), hi(HI), i(I) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline ld area(const Circle& cen) const { return (hi - lo) * cen.r * cen.r; }
	inline ld green(const Circle& cen) const {
		Pos LO = -Pos(1, 0).rot(lo) * cen.r;
		Pos HI = Pos(1, 0).rot(hi) * cen.r;
		Pos vec = Pos(cen.c.x, cen.c.y);
		return (area(cen) + vec / (HI + LO)) * .5;
	}
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi; return os; }
};
typedef std::vector<Arc> Arcs;
bool V[LEN];
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
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
inline ld union_area(std::vector<Circle>& VC, const int& x = -1) {
	memset(V, 0, sizeof V);
	std::sort(VC.begin(), VC.end(), cmpr);
	int sz = VC.size();
	for (int i = 0; i < sz; i++) {
		if (i == x || V[i]) continue;
		for (int j = i + 1; j < sz; j++) {
			if (j == x || V[j]) continue;
			if (VC[j] < VC[i] || VC[j] == VC[i]) { V[j] = 1; continue; }
		}
	}
	ld A = 0;
	for (int i = 0; i < sz; i++) {
		if (i == x || V[i]) continue;
		Arcs VA = { { 0, 0 } };
		for (int j = 0; j < sz; j++) {
			if (j == i) continue;
			Pos vec = VC[i].c - VC[j].c;
			int ra = VC[i].r, rb = VC[j].r;
			if (vec.Euc() >= sq(ra + rb)) continue;
			if (vec.Euc() <= sq(ra - rb)) continue;

			Vld inx = intersections(VC[i], VC[j]);
			if (!inx.size()) continue;
			ld lo = inx[0];
			ld hi = inx[1];

			Arc a1, a2;
			if (lo > hi) {
				a1 = Arc(lo, 2 * PI, j);
				a2 = Arc(0, hi, j);
				VA.push_back(a1);
				VA.push_back(a2);
			}
			else {
				a1 = Arc(lo, hi, j);
				VA.push_back(a1);
			}
		}

		std::sort(VA.begin(), VA.end());
		VA.push_back(Arc(2 * PI, 2 * PI, -2));
		ld hi = 0;
		for (const Arc& a : VA) {
			if (a.i == x || V[a.i]) continue;
			if (a.lo > hi) A += VC[i].green(hi, a.lo), hi = a.hi;
			else hi = std::max(hi, a.hi);
		}
	}
	return A;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		Disks VC(N);
		for (Circle& c : VC) std::cin >> c;
		std::cout << union_area(VC) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//ALGOSPOT CIRCLES
