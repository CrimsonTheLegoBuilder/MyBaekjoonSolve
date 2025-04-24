#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 3e3 + 5;
int N, M, T, Q;
bool V[LEN];
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

///========================================================///
//half plane intersection - refer to bulijiojiodibuliduo
//O(N^2logN + 6QN) power-diagram query
///========================================================///

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
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct Linear {//ps[0] -> ps[1] :: refer to bulijiojiodibuliduo
	Pos ps[2];
	Pos dir_;
	Pos& operator[](int i) { return ps[i]; }
	Pos dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	Linear push() const {//push eps outward
		const double eps = 1e-8;
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator + (const double eps) const {//push eps outward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator - (const double eps) const {//pull eps inward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] - delta, ps[1] - delta);
	}
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return sign(ret);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
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
std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
	ld d = dist(p1, p2, o);
	if (std::abs(d) > r) return {};
	Pos vec = p2 - p1;
	Pos m = intersection(p1, p2, o, o + ~vec);
	ld distance = vec.mag();
	ld ratio = sqrt(r * r - d * d);
	Pos m1 = m - vec * ratio / distance;
	Pos m2 = m + vec * ratio / distance;
	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
	return { m1, m2 };//p1->p2
}
std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {//refer to bulijiojiodibuliduo
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Pos> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
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
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
std::vector<Pos> pd[LEN];//power diagram (Laguerre-Voronoi diagram)
std::vector<Circle> disks;
ld circle_cut(const Circle& c, const Pos& p1, const Pos& p2) {
	Pos v1 = p1 - c.c, v2 = p2 - c.c;
	ld r = c.r;
	std::vector<Pos> inx = circle_line_intersection(O, r, v1, v2);
	if (inx.empty()) return r * r * rad(v1, v2) * .5;
	Pos m1 = inx[0], m2 = inx[1];
	bool d1 = dot(m1, v1, m2) > -TOL, d2 = dot(m1, v2, m2) > -TOL;
	if (d1 && d2) return (v1 / v2) * .5;
	else if (d1) return (v1 / m2 + r * r * rad(m2, v2)) * .5;
	else if (d2) return (r * r * rad(v1, m1) + m1 / v2) * .5;
	else if (dot(v1, m1, v2) > 0 && dot(v1, m2, v2) > 0)
		return (r * r * (rad(v1, m1) + rad(m2, v2)) + m1 / m2) * .5;
	else return (r * r * rad(v1, v2)) * .5;
}
void query() {
	ll x, y, w, h;
	std::vector<Pos> box;
	std::cin >> x >> y >> w >> h;
	box = { Pos(x, y), Pos(x + w, y), Pos(x + w, y + h), Pos(x, y + h) };
	ld ret = 0;
	for (int i = 0; i < N; i++) {
		if (pd[i].empty()) continue;
		std::vector<Pos> rem = sutherland_hodgman(pd[i], box);
		int sz = rem.size();
		if (sz < 3) continue;
		for (int j = 0; j < sz; j++)
			ret += circle_cut(disks[i], rem[j], rem[(j + 1) % sz]);
	}
	std::cout << ret * 100 / w / h << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> Q;
	std::vector<Circle> tmp(N);
	for (Circle& c : tmp) std::cin >> c;
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++) {//remove
		if (V[i]) continue;
		for (int j = 0; j < N; j++) {
			if (i < j && tmp[i] == tmp[j]) V[i] = 1;
			if (tmp[i] < tmp[j]) V[i] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
		}
	}
	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
	N = disks.size();
	int bnd = 3e6;
	for (int i = 0; i < N; i++) {//compose power diagram
		std::vector<Linear> HP;
		HP.push_back(Linear(Pos(bnd, bnd), Pos(-bnd, bnd)));
		HP.push_back(Linear(Pos(-bnd, bnd), Pos(-bnd, -bnd)));
		HP.push_back(Linear(Pos(-bnd, -bnd), Pos(bnd, -bnd)));
		HP.push_back(Linear(Pos(bnd, -bnd), Pos(bnd, bnd)));
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ll ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			Pos m = ca + vec * X / distance;
			HP.push_back(Linear(m, m + ~vec));
		}
		pd[i] = half_plane_intersection(HP);
	}
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//NAC 2021 B Apple Orchard
//half plane intersection - refer to bulijiojiodibuliduo




/*

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
const ld TOL = 1e-9;
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

const ll MOD = 1e9 + 7;
ll powmod(ll a, ll b) {
	ll res = 1; a %= MOD;
	assert(b >= 0);
	for (; b; b >>= 1) {
		if (b & 1) res = res * a % MOD;
		a = a * a % MOD;
	}
	return res;
}

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
	//ld mag() const { return hypot(x, y); }
	ld mag() const { return sqrt(Euc()); }
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
ld ang(const Pos& b, const Pos& l) {
	ld x = (b * l) / b.mag();
	ld y = (b / l) / b.mag();
	return atan2l(y, x);
}
ld ang(const Line& b, const Line& l) {
	ld x = b * l;
	ld y = b / l;
	return atan2l(y, x);
}
//ld ang(const Pos& b, const Pos& l) { return atan2(b / l, b * l); }
//ld ang(const Line& b, const Line& l) { return atan2(b / l, b * l); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
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
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[(sz - 1) % sz], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[(sz - 1) % sz], p)) return 0;
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
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
		if (l1.s / l2.s < TOL) return 0;
		return target.above(intersection(l1, l2)) > -TOL;
		};
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
std::vector<Pos> convex_cut(const std::vector<Pos>& C, const Pos& b1, const Pos& b2) {
	std::vector<Pos> ret;
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = C[i], nxt = C[(i + 1) % sz];
		int c1 = ccw(b1, b2, cur), c2 = ccw(b1, b2, nxt);
		if (c1 >= 0) ret.push_back(cur);
		if (c1 * c2 < 0) ret.push_back(intersection(L(b1, b2), L(cur, nxt)));
	}
	return ret;
}
std::vector<Pos> sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
	}
	return ret;
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r <= q.r && dist + r < q.r + TOL;
		//return r <= q.r && dist + r <= q.r;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
std::vector<Pos> pd[LEN];//power diagram (Laguerre-Voronoi diagram)
std::vector<Circle> disks;
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> Q;
	std::vector<Circle> tmp(N);
	for (Circle& c : tmp) std::cin >> c;
	std::sort(tmp.begin(), tmp.end(), cmpr);
	memset(V, 0, sizeof V);
	for (int i = 0; i < N; i++) {//remove duplicates
		if (V[i]) continue;
		for (int j = i + 1; j < N; j++) {
			//if (i == j) continue;
			if (tmp[j] == tmp[i]) V[j] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
			//if (tmp[j] == tmp[i]) V[i] = 1;
			//if (tmp[i] < tmp[j]) V[i] = 1;
			//if (tmp[i].r <= tmp[j].r && (tmp[j].r - tmp[i].r >= (tmp[i].c - tmp[j].c).mag())) V[i] = 1;
			//if (std::make_pair(tmp[i].r, i) <= std::make_pair(tmp[j].r, j)) {
			//	if (tmp[j].r - tmp[i].r >= (tmp[i].c - tmp[j].c).mag()) V[i] = 1;
			//}
		}
	}
	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
	N = disks.size();
	for (int i = 0; i < N; i++) {//compose power diagram
		std::vector<Line> HP;
		HP.push_back(Line(Vec(1, 0), 3e6));
		HP.push_back(Line(Vec(-1, 0), 3e6));
		HP.push_back(Line(Vec(0, 1), 3e6));
		HP.push_back(Line(Vec(0, -1), 3e6));
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ld ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			//ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
			Pos m = ca + vec * X / distance;
			HP.push_back(L(m, m + ~vec));
		}
		if (!half_plane_intersection(HP, pd[i])) continue;

		//for (int j = 0; j < N; j++) {
		//	if (i == j) continue;
		//	Pos ca = disks[i].c, cb = disks[j].c;
		//	ld ra = disks[i].r, rb = disks[j].r;
		//	Pos vec = (cb - ca) * 2;
		//	ld X = (cb * cb - rb * rb) - (ca * ca - ra * ra);
		//	Pos m(0, 0);
		//	if (abs(vec.x) > abs(vec.y)) m = Pos(X / vec.x, 0);
		//	else m = Pos(0, X / vec.y);
		//	HP.push_back(L(m, m + ~vec));
		//}
		//if (!half_plane_intersection(HP, pd[i])) continue;
	}
	return;
}
ld valid_area(const Circle& disk, const std::vector<Pos>& HPI) {
	int sz = HPI.size();
	ld r = disk.r;
	Pos c = disk.c;
	ld a = 0;
	for (int i = 0; i < sz; i++) {
		Pos p1 = HPI[i], p2 = HPI[(i + 1) % sz];
		Pos v1 = p1 - c, v2 = p2 - c, vec = p2 - p1;
		ld d = dist(v1, v2, O);
		if (zero(d)) continue;
		if (std::abs(d) > r - TOL) { a += (r * r * ang(v1, v2)) * .5; continue; }
		//if (std::abs(d) > r) { a += r * r * ang(v1, v2); continue; }
		Pos m = intersection(L(v1, v2), L(O, ~vec + O));
		ld distance = vec.mag();
		ld ratio = sqrt(r * r - d * d);
		Pos m1 = m - vec * ratio / distance;
		Pos m2 = m + vec * ratio / distance;
		if (dot(v1, v2, m1, m2) < 0) std::swap(m1, m2);

		//ld X = dot(O, v1, v2), Y = (v2 - v1).Euc(), D = X * X - Y * ((v1 - O).Euc() - r * r);
		//D = std::max(D, 0.);
		//Pos m = v1 - (v2 - v1) * (X / Y), dr = (v2 - v1) * (sqrt(D) / Y);
		//Pos m1 = m - dr, m2 = m + dr;

		//bool f1 = v1.Euc() > r * r, f2 = v2.Euc() > r * r;
		//if (f1 && f2) {
		//	//if (on_seg_weak(v1, v2, m1)) a += r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2;
		//	if (dot(v1, m1, v2) > 0) a += r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2;
		//	else a += r * r * ang(v1, v2);
		//}
		//else if (f1) a += r * r * ang(v1, m1) + m1 / v2;
		//else if (f2) a += v1 / m2 + r * r * ang(m2, v2);
		//else a += v1 / v2;

		ld d1 = dot(m1, v1, m2), d2 = dot(m1, v2, m2);
		if (d1 >= 0 && d2 >= 0) a += (v1 / v2) * .5;
		else if (d1 >= 0) a += (v1 / m2 + r * r * ang(m2, v2)) * .5;
		else if (d2 >= 0) a += (r * r * ang(v1, m1) + m1 / v2) * .5;
		else if (dot(v1, m1, v2) > 0 && dot(v1, m2, v2) > 0) a += (r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2) * .5;
		else a += (r * r * ang(v1, v2)) * .5;
	}
	return a;
}
ld query(int w, int h, const std::vector<Pos>& box) {
	ld a = 0;
	for (int i = 0; i < N; i++) {
		//int sz = pd[i].size();
		//if (!sz) continue;
		//std::vector<Line> HP;
		//std::vector<Pos> HPI;
		//for (int j = 0; j < 4; j++) HP.push_back(L(box[j], box[(j + 1) % 4]));
		//for (int j = 0; j < sz; j++) HP.push_back(L(pd[i][j], pd[i][(j + 1) % sz]));
		//if (!half_plane_intersection(HP, HPI)) continue;
		//a += valid_area(disks[i], HPI);

		std::vector<Pos> HPI = sutherland_hodgman(pd[i], box);
		if (HPI.size() < 3) continue;
		a += valid_area(disks[i], HPI);
	}
	//a = std::min(std::max(a, 0.), 100.);
	return a * 100 / w / h;
}
void solve() {
	init();
	int x, y, w, h;
	std::vector<Pos> box;
	while (Q--) {
		std::cin >> x >> y >> w >> h;
		box = { Pos(x, y), Pos(x + w, y), Pos(x + w, y + h), Pos(x, y + h) };
		std::cout << query(w, h, box) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj22923 NAC 2021 B Apple Orchard

//void init() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(9);
//	std::cin >> N >> Q;
//	std::vector<Circle> tmp(N);
//	for (Circle& c : tmp) std::cin >> c;
//	memset(V, 0, sizeof V);
//	for (int i = 0; i < N; i++) {
//		if (V[i]) continue;
//		for (int j = i + 1; j < N; j++) {
//			if (tmp[i] == tmp[j]) V[j] = 1;
//			if (tmp[i] < tmp[j]) V[i] = 1;
//			if (tmp[j] < tmp[i]) V[j] = 1;
//		}
//	}
//	for (int i = 0; i < N; i++) if (!V[i]) disks.push_back(tmp[i]);
//	memset(V, 0, sizeof V);
//	N = disks.size();
//	for (int i = 0; i < N; i++) {
//		for (int j = i + 1; j < N; j++) {
//			if (intersection(disks, i, j)) V[i] = 1, V[j] = 1;
//		}
//		if (!V[i]) arc[i].push_back(Arc(-PI, PI, disks[i]));
//	}
//	return;
//}
//if (on_seg_strong(v1, v2, m1)) a += r * r * (ang(v1, m1) + ang(m2, v2)) + m1 / m2;


*/