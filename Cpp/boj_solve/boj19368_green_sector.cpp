#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acosl(-1);
const int LEN = 505;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
#define si lo
#define theta hi
#define LO x
#define HI y
#define LINE 1
#define CIRCLE 2
#define STRONG 0
#define WEAK 1

int N;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(const ld& the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return norm(atan2(y, x)); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d3) / (d1 - d2).mag(); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo, const ld& hi) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool collinear(const Seg& p, const Seg& q) { return collinear(p.s, p.e, q.s, q.e); }
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator < (const Circle& q) const { return c == q.c ? r < q.r : c < q.c; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool outside(const Circle& q) const { return sign((c - q.c).Euc() - sq((ll)r + q.r)) >= 0; }
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
	inline friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
ld intersection(const Seg& s1, const Seg& s2) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Vld circle_line_intersections(const Seg& l, const Circle& q, const int& t = LINE) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos s = l.s, e = l.e;
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	if (t == LINE) {
		if (0 < lo && lo < 1) ret.push_back(lo);
		if (zero(det)) return ret;
		if (0 < hi && hi < 1) ret.push_back(hi);
	}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		if (zero(det)) return ret;
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
	}
	return ret;
}
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
struct Arc {
	ld lo, hi;
	Arc(ld l_ = 0, ld h_ = 0) : lo(l_), hi(h_) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline friend std::istream& operator >> (std::istream& is, Arc& a) { is >> a.lo >> a.hi; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Arc& a) { os << a.lo << " " << a.hi; return os; }
};
typedef std::vector<Arc> Arcs;
struct Sector {
	Circle c;
	Arc a;
	Seg s1, s2;
	Arcs va;
	Polygon v1, v2;
	int val = -1;
	void init() {
		a.hi = norm(a.si + a.theta);
		std::swap(a.lo, a.hi);
		Pos lo = c.p(a.lo), hi = c.p(a.hi);
		s1 = Seg(lo, c.c); s2 = Seg(c.c, hi);//ccw
		va.clear(); v1.clear(); v2.clear();
		val = 1;
	}
	bool outer_check(const Pos& p, const int& f = STRONG) const { return inside(s2.e, c.c, s1.s, p, f); }
	bool inner_check(const Pos& p, const int& f = STRONG) const { return (c < p) ? 0 : !outer_check(p, (f + 1) % 2); }
	inline friend std::istream& operator >> (std::istream& is, Sector& s) { is >> s.c >> s.a; return is; }
} S[LEN];
void init() {
	std::sort(S, S + N, [&](const Sector& p, const Sector& q) -> bool { return p.c < q.c; });
	for (int i = 0; i < N; i++) {
		const Arc& ai = S[i].a;
		Arcs uva;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			const Circle& ci = S[i].c, & cj = S[j].c;
			if (ci.outside(cj)) continue;
			const Seg& is1 = S[i].s1, & is2 = S[i].s2;
			const Seg& js1 = S[j].s1, & js2 = S[j].s2;
			if (ci == cj) {
				if (j < i) continue;
				while (j < N && ci == S[j].c) {
					S[j].val = 0;
					uva.push_back(S[j].a);
					if (!S[i].outer_check(js1.s, WEAK))
						S[j].v1.push_back(Pos(0, 1));
					if (!S[i].outer_check(js2.e, WEAK))
						S[j].v2.push_back(Pos(0, 1));
					if (!S[j].outer_check(is1.s, STRONG) || is1.s == js2.e)
						S[i].v1.push_back(Pos(0, 1));
					if (!S[j].outer_check(is2.e, STRONG) || is2.e == js1.s)
						S[i].v2.push_back(Pos(0, 1));
					j++;
				}
				j--;
				if (S[i].val) {
					S[i].val = 2;
					uva.push_back(S[i].a);
				}
				continue;
			}
			Vld tmp = { (ld)0, 2 * PI };
			if (S[i].val) {
				Vld cc = intersections(ci, cj);
				Vld cs1 = circle_line_intersections(js1, ci, CIRCLE);
				Vld cs2 = circle_line_intersections(js2, ci, CIRCLE);
				tmp.insert(tmp.end(), cc.begin(), cc.end());
				tmp.insert(tmp.end(), cs1.begin(), cs1.end());
				tmp.insert(tmp.end(), cs2.begin(), cs2.end());
				std::sort(tmp.begin(), tmp.end());
				tmp.erase(unique(tmp.begin(), tmp.end(), eq), tmp.end());
				int sz = tmp.size();
				for (int k = 0; k < sz - 1; k++) {
					ld l = tmp[k], h = tmp[k + 1];
					ld m = (l + h) * .5;
					Pos mid = ci.p(m);
					if (S[j].inner_check(mid)) S[i].va.push_back(Arc(l, h));
				}
			}
			if (i < j) {
				ld r1, r2, r3, r4;
				Seg IS[2] = { is1, is2 };
				Seg JS[2] = { js1, js2 };
				for (int k = 1; k <= 2; k++) {
					for (int l = 1; l <= 2; l++) {
						const Seg& s1 = IS[k - 1];
						const Seg& s2 = JS[l - 1];
						if (collinear(s1, s2)) {
							r1 = projection(s1.s, s1.e, s2.s); r1 = fit(r1, 0, 1);
							r2 = projection(s1.s, s1.e, s2.e); r2 = fit(r2, 0, 1);
							r3 = projection(s2.s, s2.e, s1.s); r3 = fit(r3, 0, 1);
							r4 = projection(s2.s, s2.e, s1.e); r4 = fit(r4, 0, 1);
							if (l == 1) S[j].v1.push_back(Pos(r3, r4));
							if (l == 2) S[j].v2.push_back(Pos(r3, r4));
							if (dot(s1, s2) < 0) {
								if (k == 1) S[i].v1.push_back(Pos(r1, r2));
								if (k == 2) S[i].v2.push_back(Pos(r1, r2));
							}
						}
					}
				}
			}
			for (int t = 1; t <= 2; t++) {
				tmp = { (ld)0, (ld)1 };
				Seg s = t == 1 ? is1 : is2;
				if (on_seg_weak(s.s, s.e, cj.c)) tmp.push_back(projection(s.s, s.e, cj.c));
				if (on_seg_weak(s.s, s.e, S[j].s1.s)) tmp.push_back(projection(s.s, s.e, S[j].s1.s));
				if (on_seg_weak(s.s, s.e, S[j].s2.e)) tmp.push_back(projection(s.s, s.e, S[j].s2.e));
				Vld ix1 = circle_line_intersections(s, cj, LINE);
				ld ix2 = intersection(s, js1);
				ld ix3 = intersection(s, js2);
				tmp.insert(tmp.end(), ix1.begin(), ix1.end());
				if (ix2 > -TOL) tmp.push_back(ix2);
				if (ix3 > -TOL) tmp.push_back(ix3);
				std::sort(tmp.begin(), tmp.end());
				tmp.erase(unique(tmp.begin(), tmp.end(), eq), tmp.end());
				int sz = tmp.size();
				for (int k = 0; k < sz - 1; k++) {
					ld l = tmp[k], h = tmp[k + 1];
					ld m = (l + h) * .5;
					Pos mid = s.p(m);
					if (t == 1 && S[j].inner_check(mid, WEAK)) S[i].v1.push_back(Pos(l, h));
					if (t == 2 && S[j].inner_check(mid, WEAK)) S[i].v2.push_back(Pos(l, h));
				}
			}
		}
		if (S[i].val == 1) {
			ld lo = ai.lo, hi = ai.hi;
			if (lo > hi) {
				S[i].va.push_back(Arc(lo, 2 * PI));
				S[i].va.push_back(Arc(0, hi));
			}
			else S[i].va.push_back(Arc(lo, hi));
		}
		else if (S[i].val == 2) {
			Arcs va;
			for (const Arc& a : uva) {
				ld lo = a.lo, hi = a.hi;
				if (lo < hi) {
					va.push_back(Arc(hi, 2 * PI));
					va.push_back(Arc(0, lo));
				}
				else va.push_back(Arc(hi, lo));
			}
			std::sort(va.begin(), va.end());
			va.push_back(Arc(2 * PI, 2 * PI));
			ld cur = 0;
			for (const Arc& a : va) {
				if (a.lo > cur) S[i].va.push_back(Arc(cur, a.lo)), cur = a.hi;
				else cur = std::max(cur, a.hi);
			}
		}
		std::sort(S[i].va.begin(), S[i].va.end()); S[i].va.push_back(Arc(2 * PI, 2 * PI));
		std::sort(S[i].v1.begin(), S[i].v1.end()); S[i].v1.push_back(Pos(1, 1));
		std::sort(S[i].v2.begin(), S[i].v2.end()); S[i].v2.push_back(Pos(1, 1));
	}
	return;
}
ld union_area() {
	ld A = 0, hi = 0;
	for (int i = 0; i < N; i++) {
		if (S[i].val) {
			hi = 0;
			for (const Arc& a : S[i].va) {
				if (a.lo > hi) A += S[i].c.green(hi, a.lo), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
		if (!eq(S[i].s1.s.x, S[i].s1.e.x)) {
			hi = 0;
			for (const Pos& a : S[i].v1) {
				if (a.LO > hi) A += S[i].s1.green(hi, a.LO), hi = a.HI;
				else hi = std::max(hi, a.HI);
			}
		}
		if (!eq(S[i].s2.s.x, S[i].s2.e.x)) {
			hi = 0;
			for (const Pos& a : S[i].v2) {
				if (a.LO > hi) A += S[i].s2.green(hi, a.LO), hi = a.HI;
				else hi = std::max(hi, a.HI);
			}
		}
	}
	return A;
}
void query() {
	for (int i = 0; i < N; i++) { std::cin >> S[i]; S[i].init(); }
	init();
	std::cout << union_area() << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (std::cin >> N) query();
	return;
}
int main() { solve(); return 0; }//boj19368 Circular Sectors
//Petrozavodsk Programming Camp > Winter 2017 > Day 2: Xiaoxu Guo Contest J

/*

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acosl(-1);
const int LEN = 505;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline ll sq(ll x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(ld x, ld lo, ld hi) { return std::min(hi, std::max(lo, x)); }

#define si lo
#define theta hi

#define LINE 1
#define CIRCLE 2

#define STRONG 0
#define WEAK 1

int N;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return norm(atan2(y, x)); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d3) / (d1 - d2).mag(); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
struct Seg {
	Pos s, e;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo, const ld& hi) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool collinear(const Seg& p, const Seg& q) { return collinear(p.s, p.e, q.s, q.e); }
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool outside(const Circle& q) const { return sign((c - q.c).Euc() - sq((ll)r + q.r)) >= 0; }
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
	inline friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
bool cmpcr(const Circle& p, const Circle& q) { return p.c == q.c ? p.r < q.r : p.c < q.c; }
ld intersection(const Seg& s1, const Seg& s2) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Vld circle_line_intersections(const Seg& l, const Circle& q, const int& t = LINE) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos s = l.s, e = l.e;
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	if (t == LINE) {
		if (0 < lo && lo < 1) ret.push_back(lo);
		if (zero(det)) return ret;//remove dupl
		if (0 < hi && hi < 1) ret.push_back(hi);
	}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		if (zero(det)) return ret;//remove dupl
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
	}
	return ret;
}
Vld intersection(const Circle& a, const Circle& b) {
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
	if (zero(h)) return ret;//remove dupl
	ret.push_back(norm(rd + h));
	return ret;
}
struct Arc {
	ld lo, hi;
	Arc(ld LO = 0, ld HI = 0) : lo(LO), hi(HI) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline friend std::istream& operator >> (std::istream& is, Arc& a) { is >> a.lo >> a.hi; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Arc& a) { os << a.lo << " " << a.hi; return os; }
};
typedef std::vector<Arc> Arcs;
struct Sector {
	Circle c;
	Arc a;
	Seg s1, s2;
	Arcs va, v1, v2;
	int val = -1;
	inline friend std::istream& operator >> (std::istream& is, Sector& s) { is >> s.c >> s.a; return is; }
	void init() {
		a.hi = norm(a.si + a.theta);
		std::swap(a.lo, a.hi);
		Pos lo = c.p(a.lo);
		Pos hi = c.p(a.hi);
		s1 = Seg(lo, c.c); s2 = Seg(c.c, hi);
		va.clear(); v1.clear(); v2.clear();
		val = 1;
	}
	bool outer_check(const Pos& q, const int& f = STRONG) const { return inside(s2.e, c.c, s1.s, q, f); }
} S[LEN];
bool cmpc(const Sector& p, const Sector& q) { return cmpcr(p.c, q.c); }
bool inner_check(const Sector& s, const Pos& p, const int& f = STRONG) {
	if (s.c < p) return 0;
	return !s.outer_check(p, f == STRONG ? WEAK : STRONG);
}
void init() {
	std::sort(S, S + N, cmpc);
	for (int i = 0; i < N; i++) {
		const Arc& ai = S[i].a;
		Arcs uva;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			const Circle& ci = S[i].c, cj = S[j].c;
			if (ci.outside(cj)) continue;
			const Seg& is1 = S[i].s1, is2 = S[i].s2;
			const Seg& js1 = S[j].s1, js2 = S[j].s2;
			if (ci == cj) {
				if (j < i) continue;
				while (j < N && ci == S[j].c) {
					S[j].val = 0;
					uva.push_back(S[j].a);
					if (!S[i].outer_check(js1.s, WEAK))
						S[j].v1.push_back(Arc(0, 1));
					if (!S[i].outer_check(js2.e, WEAK))
						S[j].v2.push_back(Arc(0, 1));
					if (!S[j].outer_check(is1.s, STRONG) || is1.s == js2.e)
						S[i].v1.push_back(Arc(0, 1));
					if (!S[j].outer_check(is2.e, STRONG) || is2.e == js1.s)
						S[i].v2.push_back(Arc(0, 1));
					j++;
				}
				j--;
				if (S[i].val) {
					S[i].val = 2;
					uva.push_back(S[i].a);
				}
				continue;
			}
			Vld tmp = { 0, 2 * PI };
			if (S[i].val) {
				Vld cc = intersection(ci, cj);
				Vld cs1 = circle_line_intersections(js1, ci, CIRCLE);
				Vld cs2 = circle_line_intersections(js2, ci, CIRCLE);
				tmp.insert(tmp.end(), cc.begin(), cc.end());
				tmp.insert(tmp.end(), cs1.begin(), cs1.end());
				tmp.insert(tmp.end(), cs2.begin(), cs2.end());
				std::sort(tmp.begin(), tmp.end());
				tmp.erase(unique(tmp.begin(), tmp.end(), eq), tmp.end());
				int sz = tmp.size();
				for (int k = 0; k < sz - 1; k++) {
					ld l = tmp[k], h = tmp[k + 1];
					ld m = (l + h) * .5;
					Pos mid = ci.p(m);
					if (inner_check(S[j], mid)) S[i].va.push_back(Arc(l, h));
				}
			}
			if (i < j) {
				ld r1, r2, r3, r4;
				std::vector<Seg> IS = { is1, is2 };
				std::vector<Seg> JS = { js1, js2 };
				for (int k = 1; k <= 2; k++) {
					for (int l = 1; l <= 2; l++) {
						const Seg& s1 = IS[k - 1];
						const Seg& s2 = JS[l - 1];
						if (collinear(s1, s2)) {
							r1 = projection(s1.s, s1.e, s2.s); r1 = fit(r1, 0, 1);
							r2 = projection(s1.s, s1.e, s2.e); r2 = fit(r2, 0, 1);
							r3 = projection(s2.s, s2.e, s1.s); r3 = fit(r3, 0, 1);
							r4 = projection(s2.s, s2.e, s1.e); r4 = fit(r4, 0, 1);
							if (l == 1) S[j].v1.push_back(Arc(r3, r4));
							if (l == 2) S[j].v2.push_back(Arc(r3, r4));
							if (dot(s1, s2) < 0) {
								if (k == 1) S[i].v1.push_back(Arc(r1, r2));
								if (k == 2) S[i].v2.push_back(Arc(r1, r2));
							}
						}
					}
				}
			}
			for (int t = 1; t <= 2; t++) {
				tmp = { (ld)0, (ld)1 };
				Vld ix1;
				Seg s = t == 1 ? is1 : is2;
				if (on_seg_weak(s.s, s.e, cj.c)) tmp.push_back(projection(s.s, s.e, cj.c));
				if (on_seg_weak(s.s, s.e, S[j].s1.s)) tmp.push_back(projection(s.s, s.e, S[j].s1.s));
				if (on_seg_weak(s.s, s.e, S[j].s2.e)) tmp.push_back(projection(s.s, s.e, S[j].s2.e));
				ix1 = circle_line_intersections(s, cj, LINE);
				ld ix2 = intersection(s, js1);
				ld ix3 = intersection(s, js2);
				tmp.insert(tmp.end(), ix1.begin(), ix1.end());
				if (ix2 > -TOL) tmp.push_back(ix2);
				if (ix3 > -TOL) tmp.push_back(ix3);
				std::sort(tmp.begin(), tmp.end());
				tmp.erase(unique(tmp.begin(), tmp.end(), eq), tmp.end());
				int sz = tmp.size();
				for (int k = 0; k < sz - 1; k++) {
					ld l = tmp[k], h = tmp[k + 1];
					ld m = (l + h) * .5;
					Pos mid = s.p(m);
					if (t == 1 && inner_check(S[j], mid, WEAK)) S[i].v1.push_back(Arc(l, h));
					if (t == 2 && inner_check(S[j], mid, WEAK)) S[i].v2.push_back(Arc(l, h));
				}
			}
		}
		if (S[i].val == 1) {
			ld lo = ai.lo, hi = ai.hi;
			if (lo > hi) {
				S[i].va.push_back(Arc(lo, 2 * PI));
				S[i].va.push_back(Arc(0, hi));
			}
			else S[i].va.push_back(Arc(lo, hi));
		}
		else if (S[i].val == 2) {
			Arcs va;
			for (const Arc& a : uva) {
				ld lo = a.lo, hi = a.hi;
				if (lo < hi) {
					va.push_back(Arc(hi, 2 * PI));
					va.push_back(Arc(0, lo));
				}
				else va.push_back(Arc(hi, lo));
			}
			std::sort(va.begin(), va.end());
			va.push_back(Arc(2 * PI, 2 * PI));
			ld cur = 0;
			for (const Arc& a : va) {
				if (a.lo > cur) S[i].va.push_back(Arc(cur, a.lo)), cur = a.hi;
				else cur = std::max(cur, a.hi);
			}
		}
		std::sort(S[i].va.begin(), S[i].va.end()); S[i].va.push_back(Arc(2 * PI, 2 * PI));
		std::sort(S[i].v1.begin(), S[i].v1.end()); S[i].v1.push_back(Arc(1, 1));
		std::sort(S[i].v2.begin(), S[i].v2.end()); S[i].v2.push_back(Arc(1, 1));
	}
	return;
}
ld union_area() {
	ld A = 0, hi = 0;
	for (int i = 0; i < N; i++) {
		if (S[i].val) {
			hi = 0;
			for (const Arc& a : S[i].va) {
				if (a.lo > hi) A += S[i].c.green(hi, a.lo), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
		if (!eq(S[i].s1.s.x, S[i].s1.e.x)) {
			hi = 0;
			for (const Arc& a : S[i].v1) {
				if (a.lo > hi) A += S[i].s1.green(hi, a.lo), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
		if (!eq(S[i].s2.s.x, S[i].s2.e.x)) {
			hi = 0;
			for (const Arc& a : S[i].v2) {
				if (a.lo > hi) A += S[i].s2.green(hi, a.lo), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
	}
	return A;
}
void query() {
	for (int i = 0; i < N; i++) { std::cin >> S[i]; S[i].init(); }
	init();
	std::cout << union_area() << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (std::cin >> N) query();
	return;
}
int main() { solve(); return 0; }//boj19368 Circular Sectors
//Petrozavodsk Programming Camp > Winter 2017 > Day 2: Xiaoxu Guo Contest J


*/