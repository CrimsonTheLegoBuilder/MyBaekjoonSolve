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
#include <queue>
#include <iomanip>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<size_t> Vidx;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-7;
const int LEN = 50;
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
//#define LO x
//#define HI y

int N, M, T;
ld RET[1000];
struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = -1; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
bool cmpt(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	return p / q > 0;
}
typedef std::vector<Pos> Polygon;
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
inline ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, const int& f = STRONG) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (f == WEAK) return f1 && f2;
	bool f3 = on_seg_weak(s1, s2, d1) ||
		on_seg_weak(s1, s2, d2) ||
		on_seg_weak(d1, d2, s1) ||
		on_seg_weak(d1, d2, s2);
	return (f1 && f2) || f3;
}
ld area(const Polygon& H) {
	int sz = H.size();
	ld a = 0;
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) <= 0) return 0;
	return 1;
}
int inner_check_concave(const Polygon& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
Polygon convex_cut(const Polygon& ps, const Pos& b1, const Pos& b2) {
	Polygon qs;
	int n = ps.size();
	for (int i = 0; i < n; i++) {
		Pos p1 = ps[i], p2 = ps[(i + 1) % n];
		int d1 = ccw(b1, b2, p1), d2 = ccw(b1, b2, p2);
		if (d1 >= 0) qs.push_back(p1);
		if (d1 * d2 < 0) qs.push_back(intersection(p1, p2, b1, b2));
	}
	return qs;
}
Polygon sutherland_hodgman(const Polygon& C, const Polygon& clip) {
	int sz = clip.size();
	Polygon ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
	}
	return ret;
}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
	Seg operator - (const ld& n) const {
		Pos v = ~(e - s).unit();
		return { s + v * n, e + v * n };
	}
	Seg& operator -= (const ld& n) {
		Pos v = ~(e - s).unit();
		s += v * n; e += v * n;
		return *this;
	}
	ld mag() const { return (e - s).mag(); }
};
typedef std::vector<Seg> Segs;
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool intersect(const Seg& u, const Seg& v) { return intersect(u.s, u.e, v.s, v.e); }
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
} C0;
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
		if (zero(det)) return ret;
		if (0 < hi && hi < 1) ret.push_back(hi);
		//ret.push_back(lo);
		//ret.push_back(hi);
	}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		if (zero(det)) return ret;
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
		//ret.push_back(the(lo));
		//ret.push_back(the(hi));
	}
	return ret;
}
Pos VX[LEN * LEN * LEN]; int vp;
Polygon ROT[LEN];
struct Pii {
	int i, j;
	bool operator < (const Pii& o) const { return i == o.i ? j < o.j : i < o.i; }
	bool operator == (const Pii& o) const { return i == o.i && j == o.j; }
};
std::vector<Pii> VP;
struct Info {
	int i;
	int s1, e1, s2, e2;
	Pii p1() const { return { s1, e1 }; }
	Pii p2() const { return { s2, e2 }; }
};
Info ND[LEN * LEN * LEN]; int np;
Vint G[LEN * LEN * LEN];
bool V[LEN * LEN * LEN];
void bfs() {
	std::queue<int> Q;
	memset(V, 0, sizeof V);
	VP.clear();
	Q.push(0);
	V[0] = 1;
	while (Q.size()) {
		int p = Q.front(); Q.pop();
		for (const int& w : G[p]) {
			if (!V[w]) {
				V[w] = 1;
				Q.push(w);
				Pii se;
				se = ND[w].p1();
				if (~se.i) VP.push_back(se);
				se = ND[w].p2();
				if (~se.i) VP.push_back(se);
			}
		}
	}
	return;
}
bool cross_check(const Polygon& H, const Polygon& C) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		if (inner_check_concave(C, H[i]) > 1) return 0;
		const Pos& p1 = H[i], & p2 = H[(i + 1) % sz];
		for (int j = 0; j < 4; j++) {
			const Pos& q1 = C[j], & q2 = C[(j + 1) % 4];
			if (intersect(p1, p2, q1, q2, WEAK)) return 0;
			if (on_seg_weak(q1, q2, p1) && ccw(q1, q2, p2) > 0) return 0;
			if (on_seg_weak(q1, q2, p2) && ccw(q1, q2, p1) > 0) return 0;
			if (q2 == p1) {
				if (ccw(q1, q2, p2) > 0 && dot(q1, q2, p2) < 0) return 0;
			}
			if (q2 == p2) {
				if (ccw(q1, q2, p1) > 0 && dot(q1, q2, p1) < 0) return 0;
			}
		}
	}
	return 1;
}
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const int& r, const Pos& q) {
	Circle c = Circle(p1, r * 2);
	bool f1 = c > q;
	bool f2 = inside(p0, p1, p2, q, WEAK);
	return f1 && f2;
}
bool connectable(const Polygon& H, const Pos& s, const Pos& e, const int& r, const int& idx = -1, const int& f = LINE) {
	if (s == e) return 1;
	if (f == LINE) {
		Pos v = ~(e - s).unit() * r;
		Polygon clip = { s + v, s - v, e - v, e + v };
		return cross_check(H, clip);
		//Polygon cut = sutherland_hodgman(H, clip);
		//return eq(area(clip), area(cut));
	}
	else if (f == CIRCLE) {
		int sz = H.size();
		if (s.i == -1 || e.i == -1) return 0;
		const Pos& p0 = H[(idx - 1 + sz) % sz], & p1 = H[idx], & p2 = H[(idx + 1) % sz];
		if (inside(p2, p1, p0, s, WEAK) || inside(p2, p1, p0, e, WEAK)) return 0;
		Circle c = Circle(p1, r * 2);
		for (int i = 0; i < sz; i++) {
			if (i != idx && inside(e, p1, s, r, H[i])) return 0;
			if ((i - 1 + sz) % sz == idx || i == idx) continue;
			Vld inxs = circle_line_intersections(c, Seg(H[i], H[(i + 1) % N]), CIRCLE);
			if (inxs.size() < 2) continue;
			//return 0;
			Pos q1 = c.p(inxs[0]);
			Pos q2 = c.p(inxs[1]);
			if (inside(e, p1, s, r, q1) && inside(e, p1, s, r, q2)) return 0;
		}
		return 1;
	}
	return 1;
}
bool valid_check(const Polygon& H, const Circle& c) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz];
		if (c > p0) return 0;
		Vld inxs = circle_line_intersections(c, Seg(p0, p1), CIRCLE);
		if (inxs.size() > 1) return 0;
	}
	if (inner_check_concave(H, c.c) < 2) return 0;
	return 1;
}
Polygon get_node(const Polygon& H, const int& r, const int& i, int& tq) {
	const Pos& p0 = H[(i - 1 + N) % N], & p1 = H[i], & p2 = H[(i + 1) % N];
	tq = ccw(p0, p1, p2);
	if (tq > 0) return {};
	Polygon ret;
	Pos v;
	v = ~(p1 - p0).unit() * r;
	ret.push_back(p1 + v);
	v = ~(p2 - p1).unit() * r;
	ret.push_back(p1 + v);
	return ret;
}
Pos get_node(const Seg& s1, const Seg& s2, const int& r) {
	Seg e1 = s1 - r;
	Seg e2 = s2 - r;
	Pos m = intersection(e1.s, e1.e, e2.s, e2.e);
	Circle c = Circle(m, r);
	Vld x1 = circle_line_intersections(c, s1, LINE);
	Vld x2 = circle_line_intersections(c, s2, LINE);
	m.i = -1;
	if (x1.size() == 1 && x2.size() == 1) m.i = 0;
	return m;
}
Polygon get_node(const Pos& p1, const Pos& p2, const int& r) {
	Pos v = p2 - p1;
	ld h = v.mag() * .5;
	if (r < h) return {};
	if (eq(r, h)) { return { (p1 + p2) * .5 }; }
	ld w = sqrt(r * r - h * h);
	Pos m = (p1 + p2) * .5;
	v = ~v.unit() * w;
	return { m + v, m - v };
}
Polygon get_node(const Seg& s, const Pos& p, const int& r) {
	if (ccw(s.s, s.e, p) <= 0) return {};
	ld d = std::abs(cross(s.s, s.e, p)) / s.mag();
	if (d >= r * 2) return {};
	Pos v = s.e - s.s, c1, c2;
	Polygon ret;
	if (eq(d, r)) {
		c1 = p + v.unit() * r, c2 = p - v.unit() * r;
	}
	else {
		ld h = d - r;
		ld w = sqrt(r * r - h * h);
		Pos dir = ~v.unit() * h;
		c1 = p - dir + v.unit() * w, c2 = p - dir - v.unit() * w;
	}
	if (dot(s.s, s.e, c1) < 0 && dot(s.e, s.s, c1) < 0) ret.push_back(c1);
	if (dot(s.s, s.e, c2) < 0 && dot(s.e, s.s, c2) < 0) ret.push_back(c2);
	return ret;
}
void connect(const Polygon& H, const int& r) {
	assert(vp == np);
	for (int i = 0; i < np; i++) {
		for (int j = 0; j < np; j++) {
			if (i == j) continue;
			const Pos& s = VX[i], & e = VX[j];
			if (connectable(H, s, e, r)) {
				G[i].push_back(j);
				G[j].push_back(i);
			}
		}
	}
	for (int i = 0; i < N; i++) {
		Pos cen = H[i];
		if (ccw(H[(i - 1 + N) % N], H[i], H[(i + 1) % N]) > 0) continue;
		for (Pos& p : ROT[i]) p -= cen;
		std::sort(ROT[i].begin(), ROT[i].end(), cmpt);
		for (Pos& p : ROT[i]) p += cen;
		int sz = ROT[i].size();
		for (int j = 0; j < sz; j++) {
			const Pos& s = ROT[i][j], & e = ROT[i][(j + 1) % sz];
			if (connectable(H, s, e, r, i, CIRCLE)) {
				G[s.i].push_back(e.i);
				G[e.i].push_back(s.i);
			}
		}
	}
	bfs();
	return;
}
ld green(const Polygon& H, const int& r) {
	ld A = 0;
	std::sort(VP.begin(), VP.end());
	VP.erase(unique(VP.begin(), VP.end()), VP.end());
	int sz = VP.size();
	for (int i = 0; i < sz; i++) {
		const Pii& se0 = VP[(i - 1 + sz) % sz];
		const Pii& se1 = VP[i];
		const Pii& se2 = VP[(i + 1) % sz];
		//std::cout << "arc integral::\n";
		Circle c;
		ld lo = 0, hi = 2 * PI;
		if (~se0.j && ~se1.j) {//e - e
			const Pos& p1 = H[se0.i];
			const Pos& p2 = H[se0.j];
			const Pos& q1 = H[se1.i];
			const Pos& q2 = H[se1.j];
			Seg s1 = Seg(p1, p2) - r;
			Seg s2 = Seg(q1, q2) - r;
			Pos m = intersection(s1.s, s1.e, s2.s, s2.e);
			Pos v1 = -~(p2 - p1);
			Pos v2 = -~(q2 - q1);
			c = Circle(m, r);
			lo = v1.rad();
			hi = v2.rad();
		}
		else if (!~se0.j && ~se1.j) {//v - e
			const Pos& p1 = H[se0.i];
			const Pos& q1 = H[se1.i];
			const Pos& q2 = H[se1.j];
			Pos v = q2 - q1;
			Pos dir = ~v.unit();
			ld d = std::abs(cross(q1, q2, p1)) / v.mag();
			ld h = d - r;
			ld w = sqrt(r * r - h * h);
			Pos m = p1 - dir * h + v.unit() * w;
			c = Circle(m, r);
			lo = (p1 - m).rad();
			hi = (-~v).rad();
		}
		else if (~se0.j && !~se1.j) {//e - v
			const Pos& p1 = H[se0.i];
			const Pos& p2 = H[se0.j];
			const Pos& q1 = H[se1.i];
			Pos v = p2 - p1;
			Pos dir = ~v.unit();
			ld d = std::abs(cross(p1, p2, q1)) / v.mag();
			ld h = d - r;
			ld w = sqrt(r * r - h * h);
			Pos m = q1 - dir * h - v.unit() * w;
			c = Circle(m, r);
			lo = (-~v).rad();
			hi = (q1 - m).rad();
		}
		else if (!~se0.j && !~se1.j) {//v - v
			const Pos& p = H[se0.i];
			const Pos& q = H[se1.i];
			Pos v = q - p;
			ld w = v.mag() * .5;
			ld h = sqrt(r * r - w * w);
			Pos mid = (p + q) * .5;
			Pos m = mid + ~v.unit() * h;
			c = Circle(m, r);
			lo = (p - m).rad();
			hi = (q - m).rad();
		}
		if (lo > hi) A += c.green(lo, 2 * PI), A += c.green(0, hi);
		else A += c.green(lo, hi);
		//std::cout << "seg integral::\n";
		if (!~se1.j) continue;//ignore point
		lo = 0, hi = 1;
		Seg seg = Seg(H[se1.i], H[se1.j]);
		const Pos& q1 = H[se1.i];
		const Pos& q2 = H[se1.j];
		Pos m, v = q2 - q1;
		if (!~se0.j) {//p0 - se1
			const Pos& p1 = H[se0.i];
			ld d = std::abs(cross(q1, q2, p1)) / v.mag();
			ld h = d - r;
			ld w = sqrt(r * r - h * h);
			Pos dir = ~v.unit() * h;
			m = p1 - dir + v.unit() * w;
		}
		else {//se0 - se1
			const Pos& p1 = H[se0.i];
			const Pos& p2 = H[se0.j];
			Pos v1 = ~v.unit() * r;
			Pos v2 = ~(p2 - p1).unit() * r;
			m = intersection(q1 + v1, q2 + v1, p1 + v2, p2 + v2);
		}
		lo = intersection(Seg(q1, q2), Seg(m, m + ~v), WEAK);
		if (!~se2.j) {//se1 - p2
			const Pos& p1 = H[se2.i];
			ld d = std::abs(cross(q1, q2, p1)) / v.mag();
			ld h = d - r;
			ld w = sqrt(r * r - h * h);
			Pos dir = ~v.unit() * h;
			m = p1 - dir - v.unit() * w;
		}
		else {//se1 - se2
			const Pos& p1 = H[se2.i];
			const Pos& p2 = H[se2.j];
			Pos v1 = ~v.unit() * r;
			Pos v2 = ~(p2 - p1).unit() * r;
			m = intersection(q1 + v1, q2 + v1, p1 + v2, p2 + v2);
		}
		hi = intersection(Seg(q1, q2), Seg(m, m + ~v), WEAK);
		if (eq(lo, hi) || eq(q1.x, q2.x)) continue;
		assert(lo < hi);
		A += seg.green(lo, hi);
	}
	return A;
}
void init(Polygon& H, const int& x, const int& y, const int& r) {
	for (int i = 0; i < np; i++) G[i].clear();
	for (int i = 0; i < N; i++) ROT[i].clear();
	vp = 0; np = 0;
	C0 = Circle(Pos(x, y), r);
	VX[vp++] = Pos(x, y);
	ND[np++] = { 0, -1, -1, -1, -1 };
	Vbool F(N, 0);
	for (int i = 0; i < N; i++) {
		const Pos& p0 = H[(i - 1 + N) % N], & p1 = H[i], & p2 = H[(i + 1) % N];
		int tq = ccw(p0, p1, p2);
		if (!tq) F[i] = 1;
	}
	Polygon tmp;
	for (int i = 0; i < N; i++) if (!F[i]) tmp.push_back(H[i]);
	H = tmp;
	N = H.size();
	for (int i = 0; i < N; i++) {//node init
		Polygon inxs;
		Pos inx;
		const Pos& p0 = H[(i - 1 + N) % N], & p1 = H[i], & p2 = H[(i + 1) % N];
		ld lo = (p0 - p1).rad();
		ld hi = (p2 - p1).rad();
		if (hi < lo) hi += 2 * PI;
		Pos m = Circle(p1, r).p((lo + hi) * .5);
		m.i = -1; ROT[i].push_back(m);
		int tq;
		inxs = get_node(H, r, i, tq);
		if (tq < 0) {//v
			assert(inxs.size() == 2);
			Pos c1 = inxs[0];
			if (valid_check(H, Circle(c1, r))) {
				c1.i = np; ROT[i].push_back(c1);
				VX[vp++] = c1;
				ND[np] = { np, (i - 1 + N) % N, i, i, -1 }; np++;
			}
			Pos c2 = inxs[1];
			if (valid_check(H, Circle(c2, r))) {
				c2.i = np; ROT[i].push_back(c2);
				VX[vp++] = c2;
				ND[np] = { np, i, (i + 1) % N, i, -1 }; np++;
			}
		}
		Pos i1 = H[i], i2 = H[(i + 1) % N];
		Seg i12 = Seg(i1, i2);
		for (int j = i + 1; j < N; j++) {//v - v
			Pos j1 = H[j], j2 = H[(j + 1) % N];
			if ((i + 1) % N != j && (j + 1) % N != i) {
				inxs = get_node(i1, j1, r);
				if (inxs.size() == 1) {
					Pos c1 = inxs[0];
					if (valid_check(H, Circle(c1, r))) {
						c1.i = np;
						ROT[i].push_back(c1);
						ROT[j].push_back(c1);
						VX[vp++] = c1;
						ND[np] = { np, i, -1, j, -1 }; np++;
					}
					continue;
				}
				else if (inxs.size() == 2) {
					Pos c1 = inxs[0];
					if (valid_check(H, Circle(c1, r))) {
						c1.i = np;
						ROT[i].push_back(c1);
						ROT[j].push_back(c1);
						VX[vp++] = c1;
						ND[np] = { np, i, -1, j, -1 }; np++;
					}
					Pos c2 = inxs[1];
					if (valid_check(H, Circle(c2, r))) {
						c2.i = np;
						ROT[i].push_back(c2);
						ROT[j].push_back(c2);
						VX[vp++] = c2;
						ND[np] = { np, i, -1, j, -1 }; np++;
					}
				}
			}
			if (zero(ccw(i1, i2, j1, j2))) continue;
			Seg j12 = Seg(j1, j2);
			inx = get_node(i12, j12, r);
			if (valid_check(H, Circle(inx, r)) && !inx.i) {//e - e
				VX[vp++] = inx;
				ND[np] = { np, i, (i + 1) % N, j, (j + 1) % N }; np++;
			}
		}
		for (int j = 0; j < N; j++) {//v - e
			if (j == i || j == (i + 1) % N) continue;
			Pos j1 = H[j];
			inxs = get_node(i12, j1, r);
			for (Pos& p : inxs) {
				if (valid_check(H, Circle(p, r))) {
					p.i = np; ROT[j].push_back(p);
					VX[vp++] = p;
					ND[np] = { np, i, (i + 1) % N, j, -1 }; np++;
				}
			}
		}
	}
	return;
}
bool query() {
	int x, y, r;
	std::cin >> N >> x >> y >> r;
	if (!N && !x && !y && !r) return 0;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	init(H, x, y, r);
	connect(H, r);
	ld A = green(H, r);
	RET[T++] = A;
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	T = 0; while (query());
	for (int i = 0; i < T; i++) std::cout << RET[i] << "\n";
	return;
}
int main() { solve(); return 0; }//boj22530 Intelligent Circular Perfect Cleaner
