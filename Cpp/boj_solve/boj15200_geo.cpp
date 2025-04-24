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
const int LEN = 20;
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

int N, M;
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
Polygon P[LEN];
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
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
bool parallel(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3, d4); }
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
	if (f == WEAK) return a1;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Vld intersections(const Seg& l, const Polygon& H) {
	int sz = H.size();
	Vld ret;
	const Pos& s = l.s, & e = l.e;
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = H[i], & p1 = H[(i + 1) % sz];
		if (parallel(s, e, p0, p1)) continue;
		if (ccw(s, e, p0) * ccw(s, e, p1) <= 0) {
			ld x = intersection(Seg(s, e), Seg(p0, p1));
			ret.push_back(x);
		}
	}
	std::sort(ret.begin(), ret.end());
	ret.erase(unique(ret.begin(), ret.end(), eq), ret.end());
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	ld A = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> M;
		P[i].resize(M);
		for (Pos& p : P[i]) std::cin >> p;
	}
	for (int i = 0; i < N; i++) {
		int sz = P[i].size();
		for (int j = 0; j < sz; j++) {
			const Pos& s = P[i][j], & e = P[i][(j + 1) % sz];
			Seg l = Seg(s, e);
			ld len = l.mag();
			Vld X = { 0, 1 };
			for (int k = i + 1; k < N; k++) {
				Vld inxs = intersections(l, P[k]);
				for (const ld& x : inxs) X.push_back(fit(x, 0, 1));
			}
			std::sort(X.begin(), X.end());
			X.erase(unique(X.begin(), X.end(), eq), X.end());
			int szx = X.size();
			for (int x = 0; x < szx - 1; x++) {
				const ld& lo = X[x], & hi = X[x + 1];
				ld m = (lo + hi) * .5;
				Pos mid = l.p(m);
				int t = 0;
				for (int k = i + 1; k < N; k++) if (inner_check(P[k], mid)) t++;
				A += len * (hi - lo) / (t + 1);
			}
		}
	}
	std::cout << A << "\n";
	return;
}
int main() { solve(); return 0; }//boj15200 Presjek
