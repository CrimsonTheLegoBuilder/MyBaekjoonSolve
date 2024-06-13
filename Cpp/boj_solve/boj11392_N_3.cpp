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
//typedef __float128 ld;
typedef std::vector<ld> vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 205;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
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
ld A[LEN][LEN];
struct Pos {
	ld x, y;
	ld t;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) { t = norm(atan2l(y, x)); }
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
inline bool cmp_rot(const Pos& p, const Pos& q) { return zero(p.t - q.t) ? p.i > q.i : p.t < q.t; }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return sign(ret);
}
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline void norm(const Pos& s, const Pos& e, Pos& p, Pos& q) { if (dot(s, e, p, q) < 0) std::swap(p, q); }
inline ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline Polygon convex_cut(const std::vector<Pos>& ps, const Pos& b1, const Pos& b2) {
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
inline Polygon sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
		std::sort(ret.begin(), ret.end());
		ret.erase(unique(ret.begin(), ret.end()), ret.end());
#ifdef DEBUG
		std::cout << "b1, b2:: " << b1 << " | " << b2 << "\n";
		std::cout << "ret.size:: " << ret.size() << "\nPos:: ";
		for (Pos& p : ret) std::cout << p << " ";
		std::cout << "\n";
#endif
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
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.i; return os; }
};
typedef std::vector<Arc> Arcs;
struct Circle {
	Pos c;
	int r;
	vld VA;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) { VA = { 0, 2 * PI }; }
	bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	inline bool operator < (const Circle& q) const {
		ll rsub = sq(r - q.r);
		return r < q.r && rsub >(c - q.c).Euc() - TOL;
	}
	inline bool operator ^ (const Circle& q) const {
		ll rsum = sq(r + q.r);
		return rsum < (c - q.c).Euc() + TOL;
	}
	inline bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	inline bool operator >= (const Pos& p) const { return r * r + TOL > (c - p).Euc(); }
	inline bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	inline Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	inline Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	inline ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	inline ld A() const { return 1. * r * r * PI; }
	inline ld green(const int& s, const int& e) const {
		ld ret = 0;
		ld hi;
		hi = 0;
		for (const Arc& ac : VA) {
#ifdef DEBUG
			std::cout << "ac:: " << ac << "\n";
			std::cout << "ac.lo : " << ac.lo << " hi : " << hi << "\n";
#endif
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (sign(ac.lo - hi) > 0) ret += Arc(hi, ac.lo).green(*this), hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		return ret;
	}
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
	inline bool operator < (const Triangle& t) const;
};
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
inline ld Arc::area(const Circle& cen) const { return (hi - lo) * cen.r * cen.r; }
inline ld Arc::green(const Circle& cen) const {
#ifdef DEBUG
	std::cout << "green:: " << lo << " " << hi << " " << cen << "\n";
#endif
	Pos LO = -Pos(1, 0).rot(lo) * cen.r;
	Pos HI = Pos(1, 0).rot(hi) * cen.r;
	Pos vec = Pos(cen.c.x, cen.c.y);
#ifdef DEBUG
	std::cout << "green::Arc:: " << (area(cen) + vec / (HI + LO)) * .5 << "\n";
#endif
	return (area(cen) + vec / (HI + LO)) * .5;
}
struct Seg {
	Pos s, e;
	ld l;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) { l = (s - e).mag(); }
	inline ld green(const ld& v) const { return cross(O, s, e) * v * .5; }
};
inline void norm(const Seg& S, Pos& p, Pos& q) { norm(S.s, S.e, p, q); }
inline Pos ratio(const Seg& S, Pos& p, Pos& q) {
	norm(S, p, q);
	return Pos();
}
struct Triangle {
	Pos a, b, c;
	vld VA, VB, VC;
	Triangle(Pos p = Pos(), Pos q = Pos(), Pos r = Pos()) {
		if (ccw(p, q, r) < 0) std::swap(q, r);
		a = p; b = q; c = r;
		VA = { 0, 1 };
		VB = { 0, 1 };
		VC = { 0, 1 };
	}
	inline ld area() const { return cross(a, b, c); }
	inline int inner_check(const Pos& p, const Pos& v) const {
		ld f1 = cross(a, b, p);
		ld f2 = cross(b, c, p);
		ld f3 = cross(c, a, p);
		if (sign(f1) < 0 || sign(f2) < 0 || sign(f3) < 0) return 0;
		if (zero(f1)) return (b - a) / v > 0 ? 2 : 0;//on_seg && centripetal
		if (zero(f2)) return (c - b) / v > 0 ? 2 : 0;
		if (zero(f3)) return (a - c) / v > 0 ? 2 : 0;
		return 1;
	}
	inline bool inner_check(const Triangle& q) const {
		auto inner = [&](const Pos& p) -> bool {
			return sign(ccw(a, b, p)) >= 0
				&& sign(ccw(b, c, p)) >= 0
				&& sign(ccw(c, a, p)) >= 0;
			};
		return inner(q.a) && inner(q.b) && inner(q.c);
	}
	inline bool operator < (const Triangle& q) const {
		bool f1 = inner_check(q);
		bool f2 = !zero(area() - q.area());
		return f1 && f2;
	}
	inline bool operator == (const Triangle& q) const {
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
			if (sign(ac.lo - hi) > 0) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(a, b).green(v);
		hi = 0; v = 0;
		for (const Arc& ac : VB) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (sign(ac.lo - hi) > 0) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(b, c).green(v);
		hi = 0; v = 0;
		for (const Arc& ac : VC) {
			if (ac.i < s || ac.i > e || V[ac.i]) continue;
			if (sign(ac.lo - hi) > 0) v += ac.lo - hi, hi = ac.hi;
			else hi = std::max(hi, ac.hi);
		}
		ret += Seg(c, a).green(v);
		return ret;
	}
	inline bool operator < (const Circle& q) const { return q >= a && q >= b && q >= c; }
};
inline bool Circle::operator < (const Triangle& t) const {
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
		type = t;
		if (type == triangle) T = Triangle(Pos(a, b), Pos(c, d), Pos(e, f));
		else if (type == circle) C = Circle(Pos(a, b), c);
	}
	inline bool operator < (const Confetti& cf) const {
		if (type == triangle && cf.type == triangle) return T < cf.T;
		if (type == circle && cf.type == circle) return C < cf.C;
		if (type == triangle && cf.type == circle) return T < cf.C;
		if (type == circle && cf.type == triangle) return C < cf.T;
	}
	inline bool operator == (const Confetti& cf) const {
		if (type != cf.type) return 0;
		if (type == triangle) return T == cf.T;
		else if (type == circle) return C == cf.C;
	}
	inline ld green(const int& s, const int& e) const {
		if (type == triangle) return T.green(s, e);
		else if (type == circle) return C.green(s, e);
	}
} C[LEN];
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;
	ld J = b * b - 4 * a * c;
	//if (J < TOL) return {};
	if (sign(J) < 0) return {};
	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);
	if (hi < 0 || 1 < lo) return {};
	return { { lo, hi } };//ratio, ratio
}
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Circle& c) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	return circle_line_intersections(s, e, c.c, c.r);
}
inline std::vector<Pos> intersection(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();

	if (vec.Euc() > sq(ra + rb)) return {};
	if (vec.Euc() < sq(ra - rb)) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	if (zero(h)) return { Pos(-1, norm(rd)) };
	return { Pos(norm(rd - h), norm(rd + h)) };
}
inline void arc_init() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			if (C[j] == C[i]) continue;
			if (C[i].type == triangle && C[j].type == triangle) {
#ifdef DEBUG
				std::cout << "T && T\n";
#endif
				Pos& a = C[i].T.a, b = C[i].T.b, c = C[i].T.c;
				Polygon clip = { C[j].T.a, C[j].T.b, C[j].T.c };
				Polygon seg, tmp;
				seg = { a, b };
				tmp = sutherland_hodgman(seg, clip);
				if (tmp.size()) {
					std::sort(tmp.begin(), tmp.end());
					tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
					for (const Pos& p : tmp) {
						ld s = (p - a).mag() / (b - a).mag();
						C[i].T.VA.push_back(s);
					}
				}
				seg = { b, c };
				tmp = sutherland_hodgman(seg, clip);
#ifdef DEBUG
				std::cout << "DEBUG::\n";
				std::cout << "tmp size :: " << tmp.size() << "\nPOS:: ";
				for (Pos& p : tmp) std::cout << p << "\n";
				std::cout << "DEBUG::\n";
#endif
				if (tmp.size() > 1) {
					std::sort(tmp.begin(), tmp.end());
					tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
					for (const Pos& p : tmp) {
						ld s = (p - b).mag() / (c - b).mag();
						C[i].T.VB.push_back(s);
					}
				}
				seg = { c, a };
				tmp = sutherland_hodgman(seg, clip);
#ifdef DEBUG
				std::cout << "DEBUG::\n";
				std::cout << "tmp size :: " << tmp.size() << "\nPOS:: ";
				for (Pos& p : tmp) std::cout << p << "\n";
				std::cout << "DEBUG::\n";
#endif
				if (tmp.size() > 1) {
					std::sort(tmp.begin(), tmp.end());
					tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
					for (const Pos& p : tmp) {
						ld s = (p - c).mag() / (a - c).mag();
						C[i].T.VC.push_back(s);
					}
				}
			}//T && T
			if (C[i].type == circle && C[j].type == circle) {
#ifdef DEBUG
				std::cout << "C && C\n";
#endif
				Pos vec = C[i].C.c - C[j].C.c;
				int ra = C[i].C.r, rb = C[i].C.r;
				if (vec.Euc() >= sq(ra + rb)) continue;
				if (vec.Euc() <= sq(ra - rb)) continue;

				auto inx = intersection(C[i].C, C[j].C);
				if (!inx.size()) continue;
				ld lo = inx[0].x;
				ld hi = inx[0].y;
				C[i].C.VA.push_back(lo);
				C[i].C.VA.push_back(hi);
			}//C && C
			if (C[i].type == triangle && C[j].type == circle) {
#ifdef DEBUG
				std::cout << "T && C\n";
#endif
				Pos& a = C[i].T.a, b = C[i].T.b, c = C[i].T.c;
				Circle d = C[j].C;
				Polygon inx;
				inx = circle_line_intersections(a, b, d);
#ifdef DEBUG
				std::cout << "T && T DEBUG:: a, b\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					ld s = inx[0].x;
					ld e = inx[0].y;
					if (s > 0 - TOL) C[i].T.VA.push_back(s);
					if (e < 1 + TOL) C[i].T.VA.push_back(e);
				}
				inx = circle_line_intersections(b, c, d);
#ifdef DEBUG
				std::cout << "T && T DEBUG:: b, c\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					ld s = inx[0].x;
					ld e = inx[0].y;
					if (s > 0 - TOL) C[i].T.VB.push_back(s);
					if (e < 1 + TOL) C[i].T.VB.push_back(e);
				}
				inx = circle_line_intersections(c, a, d);
#ifdef DEBUG
				std::cout << "T && T DEBUG:: c, a\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					ld s = inx[0].x;
					ld e = inx[0].y;
					if (s > 0 - TOL) C[i].T.VB.push_back(s);
					if (e < 1 + TOL) C[i].T.VB.push_back(e);
				}
			}//T && C
			if (C[i].type == circle && C[j].type == triangle) {
#ifdef DEBUG
				std::cout << "C && T\n";
#endif
				Circle d = C[i].C;
				Pos& a = C[j].T.a, b = C[j].T.b, c = C[j].T.c;
				Polygon inx, tmp;
				inx = circle_line_intersections(a, b, d);
#ifdef DEBUG
				std::cout << "C && T DEBUG:: a, b\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					Pos ratio = inx[0];
					if (0 - TOL < ratio.x) {
						Pos s = a + (b - a) * ratio.x;
						C[i].C.VA.push_back(Pos(s.x - d.c.x, s.y - d.c.y). t);
					}
					if (ratio.y < 1 + TOL) {
						Pos e = a + (b - a) * ratio.y;
						C[i].C.VA.push_back(Pos(e.x - d.c.x, e.y - d.c.y).t);
					}
				}
				inx = circle_line_intersections(b, c, d);
#ifdef DEBUG
				std::cout << "C && T DEBUG:: b, c\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					Pos ratio = inx[0];
					if (0 - TOL < ratio.x) {
						Pos s = a + (b - a) * ratio.x;
						C[i].C.VA.push_back(Pos(s.x - d.c.x, s.y - d.c.y).t);
					}
					if (ratio.y < 1 + TOL) {
						Pos e = a + (b - a) * ratio.y;
						C[i].C.VA.push_back(Pos(e.x - d.c.x, e.y - d.c.y).t);
					}
				}
				inx = circle_line_intersections(c, a, d);
#ifdef DEBUG
				std::cout << "C && T DEBUG:: c, a\n";
				if (inx.size()) std::cout << inx[0] << "\n";
				else std::cout << "fuck\n";
#endif
				if (inx.size()) {
					Pos ratio = inx[0];
					if (0 - TOL < ratio.x) {
						Pos s = a + (b - a) * ratio.x;
						C[i].C.VA.push_back(Pos(s.x - d.c.x, s.y - d.c.y).t);
					}
					if (ratio.y < 1 + TOL) {
						Pos e = a + (b - a) * ratio.y;
						C[i].C.VA.push_back(Pos(e.x - d.c.x, e.y - d.c.y).t);
					}
				}
			}//C && T
		}
		if (C[i].type == triangle) {
			std::sort(C[i].T.VA.begin(), C[i].T.VA.end());
			std::sort(C[i].T.VB.begin(), C[i].T.VB.end());
			std::sort(C[i].T.VC.begin(), C[i].T.VC.end());
		}
		else if (C[i].type == circle) {
			std::sort(C[i].C.VA.begin(), C[i].C.VA.end());
		}
	}
#ifdef DEBUG
	for (int i = 0; i < N; i++) {
		if (C[i].type == triangle) {
			std::cout << "VA::\n";
			for (ld& a : C[i].T.VA) std::cout << (long double)a << " ";
			std::cout << "\nVB::\n";
			for (ld& a : C[i].T.VB) std::cout << (long double)a << " ";
			std::cout << "\nVC::\n";
			for (ld& a : C[i].T.VC) std::cout << (long double)a << " ";
			std::cout << "\n";
		}
		if (C[i].type == circle) {
			std::cout << "VA::\n";
			for (ld& a : C[i].C.VA) std::cout << (long double)a << " ";
			std::cout << "\n";
		}
	}
#endif
	return;
}
inline void green_seg(const Seg& S, const vld& VA, const int& I) {
	if (!ccw(O, S.s, S.e)) return;
	int sz = VA.size();
	Pos pet = ~(S.e - S.s);
	Pos fug = -pet;
	for (int i = 0; i < sz - 1; i++) {
		ld d = VA[i + 1] - VA[i];
		if (zero(d)) continue;
		ld ratio = (VA[i + 1] + VA[i]) * .5;
		Pos m = S.s + (S.e - S.s) * ratio;

		int prev = -1, col = -1, nxt1 = N, nxt2 = N;
		for (int j = I - 1; j >= 0; j--) {
			if (C[j].type == triangle) {
				int f = C[j].T.inner_check(m, pet);//centripetal
				if (f == 2 || C[j].T.inner_check(m, fug) == 2) col = std::max(col, j);
				if (f) prev = std::max(prev, j);
			}
			else if (C[j].type == circle) if (C[j].C >= m) prev = std::max(prev, j);
			if (prev >= j && col >= j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == triangle) {
				if (C[j].T.inner_check(m, pet)) nxt1 = std::min(nxt1, j);//centripetal
				if (C[j].T.inner_check(m, fug)) nxt2 = std::min(nxt2, j);//centrifugal
			}
			else if (C[j].type == circle) {
				if (C[j].C >= m) {
					nxt1 = std::min(nxt1, j);
					nxt2 = std::min(nxt2, j);
				}
			}
			if (nxt1 < N && nxt2 < N) break;
		}
		
		//ld a = cross(O, S.s, S.e) * .5 * d;
		ld a = S.green(d);
		if (prev != -1 && prev > col) {
			for (int j = I; j < nxt2; j++) A[j][prev] -= a;
		}
		for (int j = I; j < nxt1; j++) A[j][I] += a;
	}
	return;
}
inline void green_circle(const Circle& c, const vld& VA, const int& I) {
	int sz = VA.size();
	for (int i = 0; i < sz - 1; i++) {
		ld d = VA[i + 1] - VA[i];
		if (zero(d)) continue;
		ld t = (VA[i + 1] + VA[i]) * .5;
		Pos R = Pos(c.r, 0).rot(t);
		Pos pet = -R;
		Pos m = c.c + R;

		int prev = -1, nxt = N;
		for (int j = I - 1; j >= 0; j--) {
			if (C[j].type == triangle) if (C[j].T.inner_check(m, pet)) prev = std::max(prev, j);
			else if (C[j].type == circle) {
				if (C[j].C == c) continue;
				if (C[j].C >= m) prev = std::max(prev, j);
			}
			if (prev > j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == triangle) if (C[j].T.inner_check(m, pet)) nxt = std::min(nxt, j);
			else if (C[j].type == circle) if (C[j].C == c || C[j].C >= m) nxt = std::min(nxt, j);
			if (nxt < N) break;
		}

		ld a = Arc(VA[i], VA[i + 1]).green(c);
		if (prev != -1) {
			for (int j = I; j < nxt; j++) A[j][prev] -= a;
		}
		for (int j = I; j < nxt; j++) A[j][I] += a;
	}
	return;
}
inline void green() {
	memset(A, 0, sizeof A);
	for (int i = 0; i < N; i++) {
		if (C[i].type == triangle) {
			green_seg(Seg(C[i].T.a, C[i].T.b), C[i].T.VA, i);
			green_seg(Seg(C[i].T.b, C[i].T.c), C[i].T.VB, i);
			green_seg(Seg(C[i].T.c, C[i].T.a), C[i].T.VC, i);
		}
		else if (C[i].type == circle) green_circle(C[i].C, C[i].C.VA, i);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= i; j++) std::cout << (long double)A[i][j] << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	return;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	int ret = 0;
	std::cin >> N;
	int t = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> t;
		if (t == 1) std::cin >> a >> b >> c >> d >> e >> f;
		else if (t == 2) std::cin >> a >> b >> c;
		C[i] = Confetti(t - 1, a, b, c, d, e, f, i);
	}
	arc_init();
	green();
	return;
}
int main() { solve(); return 0; }//boj11392 Confetti refer to cki86201

/*
2
1 0 0 2 0 0 2
1 1 1 -1 1 1 -1

2
1 0 0 2 0 0 2
1 2 2 2 0 0 2
*/
