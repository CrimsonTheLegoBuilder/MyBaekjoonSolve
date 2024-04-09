#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
int N, M, T, Q;
bool V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ld flip(ld lat) {
	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
	if (zero(lat)) return PI * .5;
	if (lat > 0) return PI * .5 - lat;
	if (lat < 0) return -(PI * .5) - lat;
	return INF;
}
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	ld ang() const { return atan2(y, x); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
Line L(const Vec& s, const Pos& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
Pos rotate(Pos c, ld R, ld the) {
	Pos p = Pos(R, 0);
	ld vx = p.x * cos(the) - p.y * sin(the);
	ld vy = p.x * sin(the) + p.y * cos(the);
	return c + Pos(vx, vy);
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld get_theta(const Line& b, const Line& l) {
	ld x = (b * l) / b.mag();//dot
	ld y = (b / l) / b.mag();//cross
	return atan2(y, x);
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
ld area(const std::vector<Pos>& H) {
	Pos P = { 0, 0 };
	ld ret = 0;
	int h = H.size();
	for (int i = 0; i < h; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % h];
		ret += cross(P, cur, nxt);
	}
	//return ret / 2;
	return ret;
}
struct Seg {
	Pos s, e;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
};
std::vector<Seg> seg;
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
std::vector<Circle> disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc
	Circle c; // c.r - radius of arc
	Arc(ld LO = 0, ld HI = 0, Circle C = Circle(O, 0)) : lo(LO), hi(HI), c(C) {}
	bool operator < (const Arc& a) const { return lo == a.lo ? hi < a.hi : lo < a.lo; }
};
std::vector<Arc> arc[LEN];
bool intersection(const std::vector<Circle>& C, int i, int j) {
	Circle a = C[i], b = C[j];
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;//vec a -> b
	ld ra = a.r, rb = b.r;
	ld distance = vec.mag();

	if (distance > ra + rb + TOL) return 0;
	if (distance < std::abs(ra - rb) - TOL) return 0;

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
	ld Y = sqrt(ra * ra - X * X);
	Pos m = ca + vec * (X / distance);
	if (Y > 0) {
		ld ratio = Y / distance;
		Pos l = m + ~vec * ratio, h = m - ~vec * ratio;
		ld lo, hi;
		lo = (l - ca).ang();
		hi = (h - ca).ang();
		if (lo > hi) {
			arc[i].push_back(Arc(lo, PI, ca));
			arc[i].push_back(Arc(-PI, hi, ca));
		}
		else arc[i].push_back(Arc(lo, hi, ca));
		lo = (h - cb).ang();
		hi = (l - cb).ang();
		if (lo > hi) {
			arc[j].push_back(Arc(lo, PI, cb));
			arc[j].push_back(Arc(-PI, hi, cb));
		}
		else arc[j].push_back(Arc(lo, hi, cb));
		return 1;
	}
	return 0;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> Q;
	std::vector<Circle> tmp(N);
	for (Circle& c : tmp) std::cin >> c;
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++) {
		if (V[i]) continue;
		for (int j = i + 1; j < N; j++) {
			if (tmp[i] == tmp[j]) V[j] = 1;
			if (tmp[i] < tmp[j]) V[i] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
		}
	}
	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
	memset(V, 0, sizeof V);
	N = disks.size();
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			if (intersection(disks, i, j)) V[i] = 1, V[j] = 1;
		}
		if (!V[i]) arc[i].push_back(Arc(-PI, PI, disks[i]));
	}
	return;
}
void sweep() {
	for (int i = 0; i < N; i++) {
		if (arc[i].size() == 1) continue;
		std::sort(arc[i].begin(), arc[i].end());
		std::vector<Arc> tmp;
		int sz = arc[i].size();
		Circle c = arc[i][0].c;
		for (int j = 0; j < sz - 1; j++) {
			if (arc[i][j].hi > arc[i][j + 1].lo - TOL) {
			//if (arc[i][j].hi >= arc[i][j + 1].lo) {
				if (arc[i][j].hi > arc[i][j + 1].hi)
					arc[i][j + 1] = Arc(arc[i][j].lo, arc[i][j].hi, c);
				else
					arc[i][j + 1] = Arc(arc[i][j].lo, arc[i][j + 1].hi, c);
				arc[i][j] = Arc(INF, INF);
			}
		}
		for (const Arc& a : arc[i]) if (a.lo < INF - 1) tmp.push_back(a);
		tmp.swap(arc[i]);
		if (arc[i].size() == 1) {
			if (zero(norm(arc[i][0].lo) - norm(arc[i].back().hi)) && V[i])
				arc[i].pop_back();
			continue;
		}
		if (zero(norm(arc[i][0].lo) - norm(arc[i].back().hi))) {
			arc[i][0].lo = arc[i].back().lo;
			arc[i].pop_back();
		}
	}
	return;
}
void seg_init() {
	for (int i = 0; i < N; i++) {
		int sz = arc[i].size();
		Circle c = arc[i][0].c;
		for (int j = 0; j < sz; j++) {
			ld hi = arc[i][j].hi, lo = arc[i][(j + 1) % sz].lo;
			Pos s = rotate(c.c, c.r, lo);
			Pos e = rotate(c.c, c.r, hi);
			seg.push_back(Seg(s, e));
		}
	}
	return;
}
ld query(const std::vector<Pos> box) {
	ld A = area(box);
	ld a = 0;
	for (const Seg& se : seg) {
		for (int i = 0; i < 4; i++) {

		}
	}
	return a / A;
}
void solve() {
	init();
	sweep();
	ld X, Y, XW, YH;
	std::vector<Pos> box;
	while (Q--) {
		std::cin >> X >> Y >> XW >> YH;
		box = { Pos(X, Y), Pos(X + XW, Y), Pos(X + XW, Y + YH), Pos(X, Y + YH) };
		std::cout << query(box) << "\n";
	}
	return;
}