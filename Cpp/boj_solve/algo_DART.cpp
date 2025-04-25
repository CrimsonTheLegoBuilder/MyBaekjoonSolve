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
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ll sq(const ll& x) { return x * x; }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int Q, N;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(const ld& the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Circle {
	Pos c;
	int r, s;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) { s = 0; }
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
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
typedef std::vector<Circle> Disks;
bool cmpcr(const Circle& p, const Circle& q) { return p.c == q.c ? p.r < q.r : p.c < q.c; }
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
void query() {
	ld A = 0;
	Circle b;
	std::cin >> b >> N;
	Disks C(N);
	for (Circle& c : C) std::cin >> c >> c.s;
	for (int i = 0; i < N; i++) {
		Vld V = { 0 };
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			Vld inxs = intersections(C[i], C[j]);
			for (const ld& x : inxs) V.push_back(x);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end(), eq), V.end());
		V.push_back(PI * 2);
		int sz = V.size();
		for (int k = 0; k < sz - 1; k++) {
			const ld& s = V[k], e = V[k + 1];
			if (eq(s, e)) continue;
			ld a = C[i].green(s, e);
			Pos mid = C[i].p((s + e) * .5);
			int p = 0, m = 0;
			for (int k = 0; k < N; k++) {
				if (C[k] >= mid) p = std::max(p, C[k].s);
				if (k == i) continue;
				if (C[k] >= mid) m = std::max(m, C[k].s);
			}
			A += a * p; 
			A -= a * m; 
		}
	}
	A /= b.area(0, PI * 2);
	std::cout << A << "\n";
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//algospot DART