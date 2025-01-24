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
const ld TOL = 1e-10;
const int LEN = 1005;
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

#define XX 0
#define SX 1
#define XE 2
#define SE 3

int B, N, M, T;
ld RET[LEN];
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
typedef std::vector<Pos> Polygon;
Polygon ROT[LEN];
bool cmpt(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	return p / q > 0;
}
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
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
	bool inside(const Circle& q) const { return sign((c - q.c).Euc() - sq((ll)r - q.r)) <= 0; }
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
	if (hi < lo) std::swap(hi, lo);
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
Pos VX[LEN]; int vp;
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
Vint G[LEN];
bool V[LEN];
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
			}
		}
	}
	return;
}
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const int& r, const Pos& q) {
	Circle c = Circle(p1, r * 2);
	bool f1 = c > q;
	bool f2 = inside(p0, p1, p2, q, WEAK);
	return f1 && f2;
}
struct Node {
	int i;
	ld t;
	Node(int i_ = 0, ld t_ = 0) : i(i_), t(t_) {}
	bool operator < (const Node& x) const { return t < x.t; }
};
Arcs BLK[LEN];//block
typedef std::vector<Node> Vnode;
Vnode ND[LEN];
bool cmpti(const Node& p, const Node& q) { return eq(p.t, q.t) ? p.i < q.i : p.t < q.t; }
bool eqti(const Node& p, const Node& q) { return eq(p.t, q.t) && p.i == q.i; }
int cross_check(const Seg& b, const Circle& c, ld& s, ld& e) {//SEXSEX
	Vld inxs = circle_line_intersections(c, b, CIRCLE);
	int sz = inxs.size();
	if (!sz) return XX;
	if (sz == 2) {
		s = inxs[0];
		e = inxs[1];
		if (c == b.e) e = norm(e + TOL);
		if (c == b.s) s = norm(s - TOL);
		return SE;
	}
	ld x = inxs[0];
	bool fs = c >= b.s;
	bool fe = c >= b.e;
	if (fs && fe) {
		if (c == b.e) {
			x = norm(x - TOL);
			s = x;
			return SX;
		}
		else if (c == b.s) {
			x = norm(x + TOL);
			e = x;
			return XE;
		}
		assert(0);
	}
	if (fe) {
		s = x;
		if (c == b.e) s = norm(s - TOL);
		return SX;
	}
	else if (fs) {
		e = x;
		if (c == b.s) e = norm(e + TOL);
		return XE;
	}
	assert(0);
}
bool valid_check(const Polygon& H, const Circles& C, const Pos& p, const int& k) {
	if (inner_check(H, p) < 0) return 0;
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		if (i == k) continue;
		if (C[i] > p) return 0;
	}
	return 1;
}
bool query() {
	std::cin >> B; Polygon H(B); for (Pos& p : H) std::cin >> p;
	std::cin >> N; Polygon I(N); for (Pos& p : I) std::cin >> p;
	std::cin >> M; Circles R(M); for (Circle& r : R) std::cin >> r;
	//inner circle remove
	std::sort(R.rbegin(), R.rend());
	Vbool F(M, 0);
	for (int i = 0; i < M; i++) {
		for (int j = i + 1; j < M; j++) {
			if (R[j].inside(R[i]) || R[j] == R[i]) F[j] = 1;
		}
	}
	Circles R_;
	for (int i = 0; i < M; i++) if (!F[i]) R_.push_back(R[i]);
	R = R_; M = R.size();
	//inner informer remove
	F.resize(N);
	for (int i = 0; i < N; i++) {
		F[i] = 0;
		for (int j = 0; j < M; j++) {
			if (R[j] >= I[i]) { F[i] = 1; break; }
		}
	}
	Polygon I_;
	for (int i = 0; i < N; i++) if (!F[i]) I_.push_back(I[i]);
	I = I_; N = I.size();
	//circle - circle intersecions : block and connect
	//for (int i = 0; i < M; i++) {
	//	for (int j = 0; j < M; j++) {
	//		if (i == j) continue;
	//		Vld inxs = intersections(R[i], R[j]);
	//		int sz = inxs.size();
	//		if (!sz) continue;
	//		ld hi, lo;
	//		if (sz == 1) {
	//			ld x = inxs[0];
	//			hi = norm(x + TOL);
	//			lo = norm(x - TOL);
	//		}
	//		else {
	//			lo = norm(inxs[0]);
	//			hi = norm(inxs[1]);
	//		}
	//		ND[i].push_back(Node(j, lo));
	//		ND[i].push_back(Node(j, hi));
	//		if (lo < hi) BLK[i].push_back(Arc(lo, hi));
	//		else {
	//			BLK[i].push_back(Arc(lo, 2 * PI));
	//			BLK[i].push_back(Arc(0, hi));
	//		}
	//	}
	//}
	//circle - hull intersections : block and connect
	//for (int i = 0; i < M; i++) {
	//	Vnode V;
	//	for (int j = 0; j < B; j++) {
	//		const Pos& p0 = H[j], & p1 = H[(j + 1) % B];
	//		Seg b = Seg(p0, p1);
	//		ld s, e;
	//		int f = cross_check(b, R[i], s, e);
	//		if (f == XX) continue;
	//		if (f == SE) V.push_back(Node(1, s)), V.push_back(Node(0, e));
	//		if (f == SX) V.push_back(Node(1, s));
	//		if (f == XE) V.push_back(Node(0, e));
	//		Vld inxs = circle_line_intersections(R[i], b, CIRCLE);
	//		for (const ld& x : inxs) ND[i].push_back(Node(0, x));
	//	}
	//	std::sort(V.begin(), V.end());
	//	V.erase(unique(V.begin(), V.end(), eqti), V.end());
	//}

	//circle - circle && circle - hull intersections - block && get node
	for (int i = 0; i < M; i++) {
		Vld X = { 0, 2 * PI };
		Arcs VA;
		for (int j = 0; j < M; j++) {
			if (i == j) continue;
			Vld inxs = intersections(R[i], R[j]);
			int sz = inxs.size();
			if (!sz) continue;
			for (const ld& x : inxs) X.push_back(x);
		}
		for (int j = 0; j < B; j++) {
			const Pos& p0 = H[j], & p1 = H[(j + 1) % B];
			Seg b = Seg(p0, p1);
			Vld inxs = circle_line_intersections(R[i], b, CIRCLE);
			int sz = inxs.size();
			if (!sz) continue;
			for (const ld& x : inxs) X.push_back(x);
		}
		std::sort(X.begin(), X.end());
		X.erase(unique(X.begin(), X.end(), eqti), X.end());
		int sz = X.size();
		for (int j = 0; j < sz - 1; j++) {
			const ld& s = X[j], e = X[j + 1];
			ld m = (s + e) * .5;
			Pos mid = R[i].p(m);
			if (!valid_check(H, R, mid, i)) VA.push_back(Arc(s, e));
		}

	}
	//informer - circle intersection : connect
	//informer - hull intersection : connect
	//informer - hull dist check
	//bfs
	//get optimal solution
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	while (query());
	return;
}
int main() { solve(); return 0; }//boj22530 Intelligent Circular Perfect Cleaner
