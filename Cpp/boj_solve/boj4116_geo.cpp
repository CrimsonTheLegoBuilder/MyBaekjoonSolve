#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-4;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;

bool zero(const ld& x) { return std::abs(x) < TOL; }


//2D============================================================================//
//2D============================================================================//
//2D============================================================================//
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y > p.y : x > p.x; }
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
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y;
		return os;
	}
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
	ld operator / (const Line& l) const { return s / l.s; }
	ld operator * (const Line& l) const { return s * l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c;
		return os;
	}
};
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	freopen("levees.in", "r", stdin);
	freopen("levees.out", "w", stdout);

	Pos a, b, c, d;
	while (1) {
		std::cin >> a >> b >> c >> d;
		if (zero(a.mag()) && zero(b.mag()) && zero(c.mag()) && zero(d.mag())) return;
		Pos m = intersection(L(a, c), L(b, d));
		Pos p, q, r, s;
		p = { std::abs(cross(a, b, m)) * .5,
			(a - b).mag() + (a - m).mag() + (b - m).mag() };	
		q = { std::abs(cross(b, c, m)) * .5,
			(b - c).mag() + (b - m).mag() + (c - m).mag() };	
		r = { std::abs(cross(c, d, m)) * .5,
			(c - d).mag() + (c - m).mag() + (d - m).mag() };	
		s = { std::abs(cross(d, a, m)) * .5,
			(d - a).mag() + (d - m).mag() + (a - m).mag() };
		std::vector<Pos> tmp = { p, q, r, s };
		std::sort(tmp.begin(), tmp.end());
		for (const Pos& t : tmp) std::cout << t << " ";
		std::cout << "\n";
	}
}
int main() { solve(); return 0; }//boj4116
