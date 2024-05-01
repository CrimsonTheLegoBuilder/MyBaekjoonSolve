#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 55;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ld sqr(const ld& x) { return x * x; }

//#define TEST

int N, M, T, Q, c;
ld W, H;
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
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
Circle C[LEN];
std::vector<Circle> close_circle(const int& i, const int& j, const ld& r) {
	Pos& ca = C[i].c, cb = C[j].c;
	ld ra = C[i].r + r, rb = C[j].r + r;
	Pos vec = cb - ca;//vec a -> b
	ld distance = vec.mag();
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
	if (X > ra) return {};
	Pos w = ca + vec * X / distance;
	ld Y = sqrt(ra * ra - X * X);
	Pos h = ~vec.unit() * Y;
	return { Circle(w + h, r), Circle(w - h, r) };
}
bool check(const Circle& nc, const int& i = -1, const int& j = -1) {
	if (nc.c.x < nc.r) return 0;
	if (W - nc.c.x < nc.r) return 0;
	if (nc.c.y < nc.r) return 0;
	if (H - nc.c.y < nc.r) return 0;
	for (int k = 0; k < c; k++) {
		if (k == i || k == j) continue;
		if (C[k].r + nc.r > (C[k].c - nc.c).mag()) return 0;
	}
	return 1;
}
bool F(const ld& m) {
	if (check(Circle(Pos(m, m), m))) return 1;
	if (check(Circle(Pos(W - m, m), m))) return 1;
	if (check(Circle(Pos(m, H - m), m))) return 1;
	if (check(Circle(Pos(W - m, H - m), m))) return 1;
	for (int i = 0; i < c; i++) {
		ld x, y;
		Circle& p = C[i];
		ld r = m + p.r;
		if (p.c.x - p.r < m * 2 + TOL) {
			x = p.c.x - m;
			y = sqrt(r * r - x * x);
			if (check(Circle(Pos(m, p.c.y + y), m), i) ||
				check(Circle(Pos(m, p.c.y - y), m), i))
				return 1;
		}
		if ((W - p.c.x) - p.r < m * 2 + TOL) {
			x = (W - p.c.x) - m;
			y = sqrt(r * r - x * x);
			if (check(Circle(Pos(W - m, p.c.y + y), m), i) ||
				check(Circle(Pos(W - m, p.c.y - y), m), i))
				return 1;
		}
		if (p.c.y - p.r < m * 2 + TOL) {
			y = p.c.y - m;
			x = sqrt(r * r - y * y);
			if (check(Circle(Pos(p.c.x + x, m), m), i) ||
				check(Circle(Pos(p.c.x - x, m), m), i))
				return 1;
		}
		if ((H - p.c.y) - p.r < m * 2 + TOL) {
			y = (H - p.c.y) - m;
			x = sqrt(r * r - y * y);
			if (check(Circle(Pos(p.c.x + x, H - m), m), i) ||
				check(Circle(Pos(p.c.x - x, H - m), m), i))
				return 1;
		}
		for (int j = i + 1; j < c; j++) {
			auto nc = close_circle(i, j, m);
			if (!nc.size()) continue;
			if (check(nc[0], i, j) || check(nc[1], i, j)) return 1;
		}
	}
	return 0;
}
ld bi_search() {
	ld s = 0, e = std::min(W, H) * .5;
	int cnt = 50;
	//while (cnt--) {
	while (std::abs(e - s) > 1e-6) {
		ld m = (s + e) * .5;
		if (F(m)) s = m;
		else e = m;
	}
	return s;
}
#ifndef TEST
void query() {
	std::cin >> W >> H >> c;
	for (int i = 0; i < c; i++) std::cin >> C[i];
	std::cout << bi_search() << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//BAPC 2010 E clock boj5394
#else
ld ret[LEN];
int t = 0;
void query() {
	std::cin >> W >> H >> c;
	for (int i = 0; i < c; i++) std::cin >> C[i];
	ret[t++] = bi_search();
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	freopen("E.in", "r", stdin);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() {
	solve();
	ld ans;
	freopen("E.out", "r", stdin);
	for (int i = 0; i < t; i++) {
		std::cin >> ans;
		std::cout << (std::abs(ans - ret[i]) < 1e-6 ? "Accepted\n" : "Wrong answer\n");
		//std::cout << ret[i] << " " << ans << "\n";
	}
	return 0;
}//BAPC 2010 E clock boj5394
#endif