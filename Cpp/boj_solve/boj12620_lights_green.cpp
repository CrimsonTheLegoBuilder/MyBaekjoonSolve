#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <random>
#include <cassert>
#include <array>
#include <tuple>
#include <deque>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<size_t> Vidx;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-10;
const int LEN = 55;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
#define LINE 1
#define CIRCLE 2
#define STRONG 0
#define WEAK 1
#define LO x
#define HI y
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3

int N, M, T, Q;
ld A[4];
struct Pos {
	ld x, y;
	Pos(ld X_ = 0, ld y_ = 0) : x(X_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} R, G; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
std::vector<Polygon> TR, TG;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d3) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	bool operator == (const Seg& o) const { return s == o.s && e == o.e; }
	bool operator < (const Seg& o) const {
		Pos v1 = e - s;
		Pos v2 = o.e - o.s;
		if (zero(v1 / v2) && v1 * v2 > 0) {
			if (!ccw(s, e, o.s)) return ccw(s, e, o.s) > 0;
			return s == o.s ? e < o.e : s < o.s;
		}
		bool f1 = O < v1;
		bool f2 = O < v2;
		if (f1 != f2) return f1;
		return v1 / v2 > 0;
	}
	ld r(const Pos& q) const { return projection(s, e, q); }
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
Seg SR[LEN], SG[LEN], SB[4];
typedef std::vector<Seg> Segs;
Segs VSR, VSG, VSB;
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool collinear(const Seg& p, const Seg& q) { return collinear(p.s, p.e, q.s, q.e); }
bool intersect(const Seg& u, const Seg& v) { return intersect(u.s, u.e, v.s, v.e); }
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator < (const Circle& q) const { return c == q.c ? r < q.r : c < q.c; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool operator <= (const Pos& p) const { return sign(r - (c - p).mag()) <= 0; }
	bool operator == (const Pos& p) const { return sign(r - (c - p).mag()) == 0; }
	bool operator > (const Pos& p) const { return sign(r - (c - p).mag()) > 0; }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	bool outside(const Circle& q) const { return sign((c - q.c).Euc() - sq((ll)r + q.r)) >= 0; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	ld rad(const Pos& p) const { return norm((p - c).rad()); }
	ld area(const ld& lo = 0, const ld& hi = 2 * PI) const { return (hi - lo) * r * r * .5; }
	ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld tz = (cos(lo) - cos(hi)) * m.y * r;
		return fan + tz - (s / e) * r * r * (ld).5;
	}
	inline friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
typedef std::vector<Circle> Circles;
Circles C;
Vld circle_line_intersections(const Circle& q, const Seg& l, const int& t = LINE) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos s = l.s, e = l.e;
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - (ll)q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	if (t == LINE) {
		if (0 < lo && lo < 1) ret.push_back(lo);
		//ret.push_back(lo);
		if (zero(det)) return ret;
		if (0 < hi && hi < 1) ret.push_back(hi);
		//ret.push_back(hi);
	}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		//if (-TOL < lo) ret.push_back(the(lo));
		if (zero(det)) return ret;
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
		//if (-TOL < hi) ret.push_back(the(hi));
	}
	return ret;
}
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) return fit(a1, 0, 1);
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
struct Arc {
	ld lo, hi;
	Arc(ld l_ = 0, ld h_ = 0) : lo(l_), hi(h_) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline friend std::istream& operator >> (std::istream& is, Arc& a) { is >> a.lo >> a.hi; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Arc& a) { os << a.lo << " " << a.hi; return os; }
};
typedef std::vector<Arc> Arcs;
//Arcs arcs[LEN];
Vld X[LEN];
Vld tangents(const Pos& p, const Circle& c, Polygon& vp, const bool& f = 0) {
	Pos v = c.c - p;
	ld l = v.mag();
	ld h = c.r, w = sqrtl(l * l - h * h);
	ld t = v.rad();
	ld lo = norm(t - atan2(h, w));
	ld hi = norm(t + atan2(h, w));
	if (f) vp.push_back(c.p(lo)), vp.push_back(c.p(hi));
	return { lo, hi };
}
bool inner_check(const Polygon& H, const Pos& q) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int j = (i + sz) % sz;
		if (ccw(H[i], H[j], q) <= 0) return 0;
	}
	return 1;
}
Pos get_pos(const Pos& l, const Seg& p, const Seg& q) {
	Pos p1 = p.s, p2 = p.e;
	Pos q1 = q.s, q2 = q.e;
	if (!inside(p2, l, p1, q1, WEAK) && !inside(p2, l, p1, q2, WEAK)) {
		if (intersect(l, p1, q1, q2) && intersect(l, p2, q1, q2)) return Pos(0, 1);
		else return Pos(0, 0);
	}
	Polygon tri = { p1, p2, l };
	if (!inner_check(tri, q1) && !inner_check(tri, q2)) return Pos(0, 0);
	ld r1 = ccw(l, p2, q1) >= 0 ? 1 : ccw(l, p1, q1) <= 0 ? 0 : 0.5;
	if (eq(r1, .5)) r1 = intersection(p, Seg(l, q1), WEAK);
	ld r2 = ccw(l, p2, q2) >= 0 ? 1 : ccw(l, p1, q2) <= 0 ? 0 : 0.5;
	if (eq(r2, .5)) r2 = intersection(p, Seg(l, q2), WEAK);
	if (r2 < r1) std::swap(r1, r2);
	return Pos(r1, r2);
}
bool inner_check(const Polygon& H, const Pos& q, const Pos& dir, const Pos& v) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int j = (i + sz) % sz;
		if (ccw(H[i], H[j], q) < 0) return 0;
		if (on_seg_strong(H[i], H[j], q) && sign((H[j] - H[i]) * dir) > 0) return 1;
		if (on_seg_strong(H[i], H[j], q) && sign((H[j] - H[i]) * dir) < 0) return 0;
	}
	return 1;
}
int inner_check(const Pos& p, const Pos& dir = Pos(), const Pos& v = Pos()) {
	int r = 0, g = 0;
	for (const Polygon& tr : TR) {
		if (inner_check(tr, p, dir, v)) { r = 1; break; }
	}
	for (const Polygon& tg : TG) {
		if (inner_check(tg, p, dir, v)) { g = 2; break; }
	}
	return r + g;
}
void query(const int& q) {
	TR.clear(); TG.clear();
	memset(A, 0, sizeof A);
	Segs VS;
	std::cin >> R >> G;
	std::cin >> N;
	for (Circle& c : C) std::cin >> c;
	C.resize(N);
	Polygon B = { Pos(0, 0), Pos(100, 0), Pos(100, 100), Pos(0, 100) };
	for (int i = 0; i < N; i++) {
		Polygon V, vp;
		Pos s, e;
		tangents(R, C[i], vp, 1);
		s = vp[0], e = vp[1];
		SR[i] = Seg(s, e);
		vp.clear();
		tangents(G, C[i], vp, 1);
		s = vp[0], e = vp[1];
		SG[i] = Seg(s, e);
	}
	for (int t = 0; t < 4; t++) {
		SB[t] = Seg(B[t], B[(t + 1) % 4]);
		Polygon VR = { Pos(0, 0) }, VG = { Pos(0, 0) };
		Seg b = Seg(B[t], B[(t + 1) % 4]);
		Vld V = { 0, 1 };
		for (int i = 0; i < N; i++) {
			Pos se;
			se = get_pos(R, b, SR[i]);
			if (!eq(se.x, se.y)) {
				VR.push_back(se);
				V.push_back(se.x);
				V.push_back(se.y);
			}
			se = get_pos(G, b, SG[i]);
			if (!eq(se.x, se.y)) {
				VG.push_back(se);
				V.push_back(se.x);
				V.push_back(se.y);
			}
		}
		VR.push_back(Pos(1, 1));
		VG.push_back(Pos(1, 1));
		std::sort(VR.begin(), VR.end());
		std::sort(VG.begin(), VG.end());
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end(), eq), V.end());
		int sz = V.size();
		for (int i = 0; i < sz - 1; i++) {
			Pos u = SB[t].p(V[i]);
			Pos v = SB[t].p(V[i + 1]);
			VS.push_back(Seg(u, v));
		}
		ld hi = 0;
		for (const Pos& p : VR) {
			if (hi < p.LO) {
				Pos s = SB[t].p(hi);
				Pos e = SB[t].p(p.LO);
				TR.push_back({ R, s, e });
				Seg s1 = Seg(R, s);
				VSR.push_back(s1);
				A[RED] += s1.green(0, 1);
				Seg s2 = Seg(s, e);
				//VSR.push_back(s2);//ignore wall
				A[RED] += s2.green(0, 1);
				Seg s3 = Seg(e, R);
				VSR.push_back(s3);
				A[RED] += s3.green(0, 1);
			}
			else hi = std::max(hi, p.HI);
		}
		hi = 0;
		for (const Pos& p : VG) {
			if (hi < p.LO) {
				Pos s = SB[t].p(hi);
				Pos e = SB[t].p(p.LO);
				TG.push_back({ G, s, e });
				Seg s1 = Seg(G, s);
				VSG.push_back(s1);
				A[GREEN] += s1.green(0, 1);
				Seg s2 = Seg(s, e);
				//VSG.push_back(s2);//ignore wall
				A[GREEN] += s2.green(0, 1);
				Seg s3 = Seg(e, G);
				VSG.push_back(s3);
				A[GREEN] += s3.green(0, 1);
			}
			else hi = std::max(hi, p.HI);
		}
	}
	for (int i = 0; i < N; i++) {
		Polygon VR = { Pos(0, 0) }, VG = { Pos(0, 0) };
		Pos se;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			se = get_pos(R, SR[i], SR[j]);
			if (!eq(se.x, se.y)) VR.push_back(se);
			se = get_pos(G, SG[i], SG[j]);
			if (!eq(se.x, se.y)) VG.push_back(se);
		}
		VR.push_back(Pos(1, 1));
		VG.push_back(Pos(1, 1));
		std::sort(VR.begin(), VR.end());
		std::sort(VG.begin(), VG.end());
		ld hi = 0;
		Vld x;
		for (const Pos& p : VR) {
			if (hi < p.LO) {
				Pos s = SR[i].p(hi);
				Pos e = SR[i].p(p.LO);
				TR.push_back({ R, s, e });
				Seg s1 = Seg(R, s);
				x = circle_line_intersections(C[i], s1, LINE);
				assert(x.size() && x[0] <= 1 + TOL);
				s = s1.p(x[0]);
				s1 = Seg(R, s);
				VSR.push_back(s1);
				A[RED] += s1.green(0, 1);
				Seg s2 = Seg(R, e);
				x = circle_line_intersections(C[i], s2, LINE);
				assert(x.size() && x[0] <= 1 + TOL);
				e = s2.p(x[0]);
				s2 = Seg(e, R);
				VSR.push_back(s2);
				A[RED] += s2.green(0, 1);
				ld lo = C[i].rad(e);
				ld hi = C[i].rad(s);
				X[i].push_back(lo);
				X[i].push_back(hi);
				if (lo < hi) A[RED] -= C[i].green(lo, hi);
				else A[RED] += C[i].green(hi, 2 * PI), A[RED] -= C[i].green(0, lo);
			}
			else hi = std::max(hi, p.HI);
		}
		hi = 0;
		for (const Pos& p : VG) {
			if (hi < p.LO) {
				Pos s = SG[i].p(hi);
				Pos e = SG[i].p(p.LO);
				TG.push_back({ G, s, e });
				Seg s1 = Seg(G, s);
				x = circle_line_intersections(C[i], s1, LINE);
				assert(x.size() && x[0] <= 1 + TOL);
				s = s1.p(x[0]);
				s1 = Seg(G, s);
				VSG.push_back(s1);
				A[GREEN] += s1.green(0, 1);
				Seg s2 = Seg(G, e);
				x = circle_line_intersections(C[i], s2, LINE);
				assert(x.size() && x[0] <= 1 + TOL);
				e = s2.p(x[0]);
				s2 = Seg(e, G);
				VSR.push_back(s2);
				A[GREEN] += s2.green(0, 1);
				ld lo = C[i].rad(e);
				ld hi = C[i].rad(s);
				X[i].push_back(lo);
				X[i].push_back(hi);
				if (lo < hi) A[GREEN] += C[i].green(lo, hi);
				else A[GREEN] += C[i].green(hi, 2 * PI), A[GREEN] += C[i].green(0, lo);
			}
			else hi = std::max(hi, p.HI);
		}
	}
	for (const Seg& s1 : VSR) {
		Vld V;
		const Pos& s = s1.s, & e = s1.e;
		if (on_seg_strong(s, e, G)) V.push_back(s1.r(G));
		for (const Seg& s2 : VSG) {
			ld inxs = intersection(s1, s2);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		int sz = V.size();
		for (int i = 0; i < sz - 1; i++) {
			Pos u = s1.p(V[i]), v = s1.p(V[i + 1]);
			VS.push_back(Seg(u, v));
		}
	}
	for (const Seg& s1 : VSG) {
		Vld V;
		const Pos& s = s1.s, & e = s1.e;
		if (on_seg_strong(s, e, R)) V.push_back(s1.r(R));
		for (const Seg& s2 : VSR) {
			ld inxs = intersection(s1, s2);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		int sz = V.size();
		for (int i = 0; i < sz - 1; i++) {
			Pos u = s1.p(V[i]), v = s1.p(V[i + 1]);
			VS.push_back(Seg(u, v));
		}
	}
	std::sort(VS.begin(), VS.end());
	VS.erase(unique(VS.begin(), VS.end()), VS.end());
	for (const Seg& se : VS) {
		Pos dir = se.e - se.s;
		Pos v = ~dir;
		if (inner_check(se.p(.5), dir, v) == 3) A[YELLOW] += se.green();
	}
	for (int i = 0; i < N; i++) {
		Vld& V = X[i];
		V.push_back(0);
		V.push_back(2 * PI);
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end(), eq), V.end());
		int sz = X[i].size();
		for (int j = 0; j < sz - 1; j++) {
			ld m = (V[j] + V[j + 1]) * .5;
			Pos mid = C[i].p(m);
			if (inner_check(mid)) A[YELLOW] -= C[i].green(V[j], V[j + 1]);
		}
	}
	ld black = 10000 - A[RED] - A[GREEN] + A[YELLOW];
	for (int i = 0; i < N; i++) black -= C[i].area();
	std::cout << "Case #" << q << ":\n";
	std::cout << black << "\n";
	std::cout << A[RED] << "\n";
	std::cout << A[GREEN] << "\n";
	std::cout << A[YELLOW] << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	for (int q = 1; q <= T; q++) query(q);
	return;
}
int main() { solve(); return 0; }//boj12620 lights