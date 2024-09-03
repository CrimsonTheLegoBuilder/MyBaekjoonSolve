#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 505;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N, K;
struct Pos {
	ld x, y;
	int f;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { f = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
bool cmpr(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	int f3 = sign(q.Euc() - p.Euc());
	int CCW = sign(p / q);
	if (!CCW) {
		if (!f3) return p.f > q.f;
		return f3 > 0;
	}
	return CCW > 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
struct Line {
	Pos s, e;
	Line(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	Pos v() const { return e - s; }
	ld operator / (const Line& l) const { return v() / l.v(); }
};
Line rot90(const Line& l, const Pos& p) { return Line(p, p + ~l.v()); }
Pos intersection(const Line& l1, const Line& l2) { return intersection(l1.s, l1.e, l2.s, l2.e); }
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && zero(r - C.r); }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator > (const Pos& p) const { return sign(r - (c - p).mag()) > 0; }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool operator <= (const Pos& p) const { return sign(r - (c - p).mag()) <= 0; }
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.r << "\n" << c.c.x << " " << c.c.y; return os; }
} INVAL = Circle(O, -1);
typedef std::vector<Circle> Disks;
Circle enclose_circle(const Pos& u, const Pos& v) { Pos c = (u + v) * .5; return Circle(c, (c - u).mag()); }
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Line l1 = rot90(Line(u, v), (u + v) * .5);
	Line l2 = rot90(Line(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return INVAL;
	Pos c = intersection(l1, l2);
	ld r = (c - u).mag();
	return Circle(c, r);
}
Disks enclose_circle(const Pos& u, const Pos& v, const ld& r) {
	ld h = (u - v).mag() * .5;
	if (sign(h - r) > 0) return {};
	Pos m = (u + v) * .5;
	if (zero(h - r)) return { Circle(m, r) };
	Pos vec = ~(u - v).unit();
	ld w = sqrt(r * r - h * h);
	return { Circle(m + vec * w, r), Circle(m - vec * w, r) };
}
Circle minimum_enclose_circle(Polygon& P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	Circle mec = INVAL;
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		if (mec.r < 0 || mec < P[i]) {
			mec = Circle(P[i], 0);
			for (int j = 0; j <= i; j++) {
				if (mec < P[j]) {
					Circle ans = enclose_circle(P[i], P[j]);
					if (zero(mec.r)) { mec = ans; continue; }
					Circle l = INVAL, r = INVAL;
					for (int k = 0; k <= j; k++) {
						if (ans < P[k]) {
							int CCW = ccw(P[i], P[j], P[k]);
							Circle c = enclose_circle(P[i], P[j], P[k]);
							if (c.r < 0) continue;
							else if (CCW > 0 && (l.r < 0 || cross(P[i], P[j], c.c) > cross(P[i], P[j], l.c))) l = c;
							else if (CCW < 0 && (r.r < 0 || cross(P[i], P[j], c.c) < cross(P[i], P[j], r.c))) r = c;
						}
					}
					if (l.r < 0 && r.r < 0) mec = ans;
					else if (l.r < 0) mec = r;
					else if (r.r < 0) mec = l;
					else mec = l.r < r.r ? l : r;
				}
			}
		}
	}
	return mec;
}
bool brute_check(const Polygon& H, Circle& ec, const ld& r, const int& in) {//O(N^3)
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			const Pos& I = H[i], & J = H[j];
			Disks ecs = enclose_circle(I, J, r);
			int esz = ecs.size();
			if (esz) {
				for (int k = 0; k < esz; k++) {
					int cnt = 0;
					for (int l = 0; l < sz; l++) if (ecs[k] >= H[l]) cnt++;
					if (cnt >= in) { ec = ecs[k]; return 1; }
				}
			}
		}
	}
	return 0;
}
bool check(const Polygon& H, Circle& ec, const ld& r, const int& in) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Polygon EV;
		for (int j = 0; j < sz; j++) {
			if (i == j) continue;
			Pos tmp = H[j] - H[i];
			Disks ecs = enclose_circle(O, tmp, r);
			int esz = ecs.size();
			if (!esz) continue;
			Pos s = ecs[0].c, e = ecs.back().c;
			if (sign(ccw(O, s, e)) < 0) std::swap(s, e);
			s.f = 1; EV.push_back(s);
			e.f = -1; EV.push_back(e);
		}
		std::sort(EV.begin(), EV.end(), cmpr);
		int esz = EV.size();
		if (!esz) continue;
		Pos fst = EV[0];
		Circle F = Circle(fst, r);
		int cnt = 0;
		for (int j = 0; j < sz; j++) if (F >= (H[j] - H[i])) cnt++;
		if (cnt >= in) { ec = Circle(fst + H[i], r); return 1; }
		int p = 0;
		while (p < esz && EV[p] == fst && EV[p].f == 1) p++;
		for (p; p < esz; p++) {
			cnt += EV[p].f;
			if (cnt >= in) { ec = Circle(EV[p] + H[i], r); return 1; }
		}
	}
	return 0;
}
Circle bi_search(Polygon& H, const int& in) {//O(N^2log(N) * log(MEC.R))
	int sz = H.size();
	assert(in > 1);
	assert(in <= sz);
	if (sz == 2) return enclose_circle(H[0], H[1]);
	Circle mec = minimum_enclose_circle(H);
	if (sz == in) return mec;
	ld s = 0, e = mec.r + TOL, m = 0;
	Circle ec = mec;
	while (!zero(e - s)) {
	//int cnt = 30; while (cnt--) {
		m = (e + s) * .5;
		//if (brute_check(H, ec, m, in)) e = m;
		if (check(H, ec, m, in)) e = m;
		else s = m;
	}
	return ec;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> K;
	Polygon H(N);
	for (int i = 0; i < N; i++) std::cin >> H[i];
	std::cout << bi_search(H, K) << "\n";
	return;
}
int main() { solve(); return 0; }//boj3333 Antenna