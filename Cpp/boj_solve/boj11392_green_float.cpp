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
typedef std::vector<ld> vld;
const ld INF = 1e17;
const ld TOL = 1e-11;
const ld PI = acos(-1);
const int LEN = 205;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

#define triangle 0
#define circle 1
//#define DEBUG
//#define ASSERT

int N, M, T, Q;
bool V[LEN];
ld A[LEN];
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
	//ld mag() const { return sqrt(Euc()); }
	ld mag() const { return hypotl(x, y); }
	Pos unit() const { return *this / mag(); }
	inline ld rad() const { return norm(atan2l(y, x)); }
	inline friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
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
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline void norm(const Pos& s, const Pos& e, Pos& p, Pos& q) { if (dot(s, e, p, q) < 0) std::swap(p, q); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Polygon convex_cut(const std::vector<Pos>& ps, const Pos& b1, const Pos& b2) {
	std::vector<Pos> qs;
	int n = ps.size();
	for (int i = 0; i < n; i++) {
		Pos p1 = ps[i], p2 = ps[(i + 1) % n];
		int d1 = ccw(b1, b2, p1), d2 = ccw(b1, b2, p2);
		if (d1 >= 0) qs.push_back(p1);
		if (d1 * d2 < 0) qs.push_back(intersection(p1, p2, b1, b2));
	}
	return qs;
}
Polygon sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
	}
	return ret;
}
struct Triangle;
struct Circle;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	int i;
	Arc(ld LO = 0, ld HI = 0, int I = -1) : lo(LO), hi(HI), i(I) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline ld area(const Circle& cen) const;
	inline ld green(const Circle& cen) const;
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi; return os; }
};
typedef std::vector<Arc> Arcs;
struct Circle {
	Pos c;
	int r;
	Arcs VA;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) { VA.clear(); }
	bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	inline bool operator < (const Circle& q) const {
		ll rsub = sq(r - q.r);
		return r < q.r && rsub > (c - q.c).Euc() - TOL;
	}
	inline bool operator ^ (const Circle& q) const {
		ll rsum = sq(r + q.r);
		return rsum < (c - q.c).Euc() + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	inline ld A() const { return 1. * r * r * PI; }
	inline ld green(const int& s, const int& e) const {
		ld ret = 0;
		ld hi;
		hi = 0;
		for (const Arc& ac : VA) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (ac.lo > hi) ret += Arc(hi, ac.lo).green(*this), hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		return ret;
	}
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
	bool operator < (const Triangle& t) const;
};
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
inline ld Arc::area(const Circle& cen) const { return (hi - lo) * cen.r * cen.r; }
inline ld Arc::green(const Circle& cen) const {
	Pos LO = -Pos(1, 0).rot(lo) * cen.r;
	Pos HI = Pos(1, 0).rot(hi) * cen.r;
	Pos vec = Pos(cen.c.x, cen.c.y);
	return (area(cen) + vec / (HI + LO)) * .5;
}
struct Seg {
	Pos s, e;
	ld l;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) { l = (s - e).mag(); }
	inline ld green(const ld& v) const { return cross(O, s, e) * v; }
};
inline void norm(const Seg& S, Pos& p, Pos& q) { norm(S.s, S.e, p, q); }
inline Pos ratio(const Seg& S, Pos& p, Pos& q) {
	norm(S, p, q);
	return Pos();
}
struct Triangle {
	Pos a, b, c;
	Arcs VA, VB, VC;
	Triangle(Pos p = Pos(), Pos q = Pos(), Pos r = Pos()) {
		if (ccw(p, q, r) < 0) std::swap(q, r);
		a = p; b = q; c = r;
		VA.clear(); VB.clear(); VC.clear();
	}
	inline ld area() const { return cross(a, b, c); }
	bool inner_check(const Triangle& q) const {
		auto inner = [&](const Pos& p) -> bool {
			return ccw(a, b, p) >= 0 && ccw(b, c, p) >= 0 && ccw(c, a, p) >= 0;
			};
		return inner(q.a) && inner(q.b) && inner(q.c);
	}
	bool operator < (const Triangle& q) const {
		bool f1 = inner_check(q);
		bool f2 = !zero(area() - q.area());
		return f1 && f2;
	}
	bool operator == (const Triangle& q) const {
		bool f1 = inner_check(q);
		bool f2 = zero(area() - q.area());
		return f1 && f2;
	}
	inline ld green(const int& s, const int& e) const {
		ld ret = 0;
		ld v;
		ld hi;
		hi = 0; v = 0;
		for (const Arc& ac : VA) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (ac.lo > hi) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(a, b).green(v);
		hi = 0; v = 0;
		for (const Arc& ac : VB) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (ac.lo > hi) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(b, c).green(v);
		hi = 0; v = 0;
		for (const Arc& ac : VC) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (ac.lo > hi) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(c, a).green(v);
		return ret;
	}
	bool operator < (const Circle& q) const { return q >= a && q >= b && q >= c; }
};
bool Circle::operator < (const Triangle& t) const {
	ld d1 = dist(t.a, t.b, c);
	ld d2 = dist(t.b, t.c, c);
	ld d3 = dist(t.c, t.a, c);
	return sign(d1 - r) >= 0 && sign(d2 - r) >= 0 && sign(d3 - r) >= 0;
}
struct Confetti {
	bool type;
	Triangle T;
	Circle C;
	int i;
	Confetti(int t = 0, int a = 0, int b = 0, int c = 0, int d = 0, int e = 0, int f = 0, int I = -1) : i(I) {
		t--;
		type = t;
		if (type == triangle) T = Triangle(Pos(a, b), Pos(c, d), Pos(e, f));
		else if (type == circle) C = Circle(Pos(a, b), c);
	}
	bool operator < (const Confetti& cf) const {
		if (type == triangle && cf.type == triangle) return T < cf.T;
		if (type == circle && cf.type == circle) return C < cf.C;
		if (type == triangle && cf.type == circle) return T < cf.C;
		if (type == circle && cf.type == triangle) return C < cf.T;
	}
	bool operator == (const Confetti& cf) const {
		if (type != cf.type) return 0;
		if (type == triangle) return T == cf.T;
		else if(type == circle) return C == cf.C;
	}
	inline ld green(const int& s, const int& e) const {
		if (type == triangle) return T.green(s, e);
		else if (type == circle) return C.green(s, e);
	}
} C[LEN];
//std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
//	ld d = dist(p1, p2, o);
//	if (std::abs(d) > r) return {};
//	Pos vec = p2 - p1;
//	Pos m = intersection(p1, p2, o, o + ~vec);
//	ld distance = vec.mag();
//	ld ratio = sqrt(r * r - d * d);
//	Pos m1 = m - vec * ratio / distance;
//	Pos m2 = m + vec * ratio / distance;
//	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
//	return { m1, m2 };//p1->p2
//}
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;
	ld J = b * b - 4 * a * c;
	if (J < TOL) return {};
	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);
	if (hi < 0 || 1 < lo) return {};
	return { { lo, hi } };//ratio, ratio
}
inline std::vector<Pos> intersection(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();

	if (vec.Euc() > sq(ra + rb) - TOL) return {};
	if (vec.Euc() < sq(ra - rb) + TOL) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	if (zero(h)) return {};
	return { Pos(norm(rd - h), norm(rd + h)) };
}
inline void arc_init() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			if (C[i].type == triangle && C[j].type == triangle) {

			}
			if (C[i].type == circle && C[j].type == circle) {

			}
			if (C[i].type == triangle && C[j].type == circle) {

			}
			if (C[i].type == circle && C[j].type == triangle) {

			}
		}
		std::sort(C[i].T.VA.begin(), C[i].T.VA.end());
		std::sort(C[i].T.VB.begin(), C[i].T.VB.end());
		std::sort(C[i].T.VC.begin(), C[i].T.VC.end());
		std::sort(C[i].C.VA.begin(), C[i].C.VA.end());
	}
	return;
}
inline void green(const int& I) {
	memset(A, 0, sizeof A);	
	for (int k = 0; k <= I; k++) {
		memset(V, 0, sizeof V);
		for (int i = k; i <= I; i++) {
			for (int j = i + 1; j < I; j++) {
				if (C[i] < C[j]) V[i] = 1;
				if (C[j] < C[i] || C[j] == C[i]) V[j] = 1;
			}
		}
		ld union_area = 0;
		for (int i = k; i < I; i++) if (!V[i]) union_area += C[i].green(k, I);
		A[k] = union_area;
	}
	for (int k = 0; k <= I; k++) std::cout << A[k + 1] - A[k] << " ";
	std::cout << "\n";
	return;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	int ret = 0;
	std::cin >> N;
	int t = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> t;
		if (t == 1) std::cin >> a >> b >> c >> d >> e >> f;
		else if (t == 2) std::cin >> a >> b >> c;
		C[i] = Confetti(t, a, b, c, d, e, f, i);
	}
	arc_init();
	for (int i = 0; i < N; i++) green(i);
	return;
}
int main() { solve(); return 0; }//boj11392 Confetti
