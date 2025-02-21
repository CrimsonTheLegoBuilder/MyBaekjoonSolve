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
const int LEN = 105;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }

#define STRONG 0
#define WEAK 1

#define LO x
#define HI y

#define BLACK   0
#define RED     (1 << 0)
#define GREEN   (1 << 1)
#define BLUE    (1 << 2)
#define YELLOW  (RED | GREEN)
#define MAGENTA (RED | BLUE)
#define CYAN    (GREEN | BLUE)
#define WHITE   (RED | GREEN | BLUE)

int N, M, K, Q;
ld A[1 << 3];
int I[1 << 3];
int C[LEN];
struct Pos {
	ld x, y;
	Pos(ld X_ = 0, ld y_ = 0) : x(X_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} L[1 << 3]; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon P[LEN];
std::vector<Polygon> T[1 << 3];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ld area(const Polygon& H) {
	int sz = H.size();
	ld a = 0;
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
void norm(Polygon& H) {
	ld a = area(H);
	assert(!zero(a));
	if (a < 0) std::reverse(H.begin(), H.end());
	return;
}
void prune(Polygon& H) {
	int sz = H.size();
	Vbool V(sz, 0);
	Polygon tmp;
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = H[(i - 1 + sz) % sz], & p1 = H[i], & p2 = H[(i + 1) % sz];
		if (!ccw(p0, p1, p2)) V[i] = 1;
	}
	for (int i = 0; i < sz; i++) if (!V[i]) tmp.push_back(H[i]);
	H = tmp;
	return;
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
};
Seg S[1 << 3][LEN];
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
bool inner_check(const Polygon& H, const Pos& q) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], q) < 0) return 0;
	return 1;
}
Pos get_pos(const Pos& l, const Seg& p, const Seg& q) {
	Pos p1 = p.s, p2 = p.e, q1 = q.s, q2 = q.e;
	if (!inside(p2, l, p1, q1, WEAK) && !inside(p2, l, p1, q2, WEAK)) {
		if (intersect(l, p1, q1, q2) && intersect(l, p2, q1, q2)) return Pos(0, 1);
		else return Pos(0, 0);
	}
	Polygon tri = { p1, p2, l };
	bool in1 = inner_check(tri, q1), in2 = inner_check(tri, q2);
	if (!in1 && !in2) return Pos(0, 0);
	ld r1 = 0, r2 = 1;
	if (in1 && in2) {
		r1 = intersection(p, Seg(l, q1), WEAK);
		r2 = intersection(p, Seg(l, q2), WEAK);
	}
	else if (in1) r1 = intersection(p, Seg(l, q1), WEAK);
	else if (in2) r2 = intersection(p, Seg(l, q2), WEAK);
	else r1 = r2 = 0;
	if (r2 < r1) std::swap(r1, r2);
	return Pos(r1, r2);
}
ld intersection(const Polygon& a, const Polygon& b) {
	Polygon ret = sutherland_hodgman(a, b);
	return area(ret);
}
ld intersection(const Polygon& a, const Polygon& b, const Polygon& c) {
	Polygon d = sutherland_hodgman(a, b); prune(d);
	Polygon ret = sutherland_hodgman(d, c);
	return area(ret);
}
void query(const int& q) {
	std::cin >> L[RED] >> L[GREEN] >> L[BLUE];
	memset(A, 0, sizeof A);
	memset(I, -1, sizeof I);
	memset(C, 0, sizeof C);
	for (int c = 1; c < (1 << 3); c <<= 1) T[c].clear();
	for (int c = 1; c < (1 << 3); c <<= 1) {
		int f0 = inner_check(P[0], L[c]), fk = 0;
		if (f0) { C[0] |= c; I[c] = 0; }
		for (int k = 1; k <= K; k++) {
			fk = inner_check(P[k], L[c]);
			if (fk) {
				C[0] ^= c;
				C[k] |= c;
				I[c] = k;
				break;
			}
		}
		if (fk) continue;
		int sz;
		for (int k = 1; k <= K; k++) {
			const Polygon& H = P[k];
			sz = H.size();
			Pos pl = H[0], pr = H[0];
			for (int j = 0; j < sz; j++) {
				if (ccw(L[c], pl, H[j]) > 0) pl = H[j];
				if (ccw(L[c], pr, H[j]) < 0) pr = H[j];
			}
			S[c][k] = Seg(pl, pr);
		}
		sz = P[0].size();
		for (int i = 0; i < sz; i++) {
			const Pos& s = P[0][i], & e = P[0][(i + 1) % sz];
			Seg w = Seg(s, e);
			Polygon VP = { Pos(0, 0) };
			for (int j = 0; j < N; j++) {
				if (i == j) continue;
				Pos se = get_pos(L[c], w, S[c][j]);
				if (!eq(se.x, se.y)) VP.push_back(se);
			}
			VP.push_back(Pos(1, 1));
			std::sort(VP.begin(), VP.end());
			ld hi = 0;
			for (const Pos& p : VP) {
				if (hi < p.LO) {
					Pos s = w.p(hi);
					Pos e = w.p(p.LO);
					Polygon tri = { L[c], s, e };
					T[c].push_back(tri);
					hi = p.HI;
				}
				else hi = std::max(hi, p.HI);
			}
		}
	}
	if (!I[RED] && !I[GREEN] && !I[BLUE]) {
		for (const Polygon& R : T[RED]) {
			for (const Polygon& G : T[GREEN]) {
				for (const Polygon& B : T[BLUE]) {
					ld a = intersection(R, G, B);
					A[WHITE] += a;
				}
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		int c1 = (1 << ((i + 1) % 3));
		int c2 = (1 << ((i + 2) % 3));
		if (!I[c1] && !I[c2]) {
			int c = c1 | c2;
			for (const Polygon& T1 : T[c1]) {
				for (const Polygon& T2 : T[c2]) {
					ld a = intersection(T1, T2);
					A[c] += a;
				}
			}
		}
	}
	for (int c = 1; c < (1 << 3); c <<= 1) {
		if (!I[c]) {
			for (const Polygon& T0 : T[c]) A[c] += area(T0);
		}
	}
	for (int k = 1; k <= K; k++) {
		ld a = area(P[k]);
		int c = C[k];
		A[c] += a;
	}
	A[YELLOW] -= A[WHITE];
	A[MAGENTA] -= A[WHITE];
	A[CYAN] -= A[WHITE];
	A[RED] -= A[YELLOW] + A[MAGENTA] + A[WHITE];
	A[GREEN] -= A[YELLOW] + A[CYAN] + A[WHITE];
	A[BLUE] -= A[MAGENTA] + A[CYAN] + A[WHITE];
	A[BLACK] = area(P[0]);
	for (int c = 1; c < (1 << 3); c++) A[BLACK] -= A[c];
	std::cout << "Case #" << q << ":\n";
	std::cout << "R: " << A[RED] << "\n";
	std::cout << "G: " << A[GREEN] << "\n";
	std::cout << "B: " << A[BLUE] << "\n";
	std::cout << "Y: " << A[YELLOW] << "\n";
	std::cout << "M: " << A[MAGENTA] << "\n";
	std::cout << "C: " << A[CYAN] << "\n";
	std::cout << "W: " << A[WHITE] << "\n";
	std::cout << "L: " << A[BLACK] << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	//std::cout << std::scientific;
	std::cout.precision(15);
	std::cin >> N;
	P[0].resize(N);
	for (Pos& p : P[0]) std::cin >> p;
	norm(P[0]);
	std::cin >> K;
	for (int i = 1; i <= K; i++) {
		std::cin >> M;
		P[i].resize(M);
		for (Pos& p : P[i]) std::cin >> p;
		norm(P[i]);
	}
	std::cin >> Q;
	for (int q = 1; q <= Q; q++) query(q);
	return;
}
int main() { solve(); return 0; }//boj12620 