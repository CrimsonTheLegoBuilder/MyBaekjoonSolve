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
const int LEN = 1005;
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

int T, N, M;
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
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Polygon graham_scan(Polygon C) {
	Polygon H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) < 0) return 0;
	return 1;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos c_ = Pos(), ld r_ = 0) : c(c_), r(r_) {}
	bool operator > (const Pos& p) const { return sign(r - (c - p).mag()) > 0; }
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
};
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ld ra = a.r, rb = b.r;
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
ld green(const Pos& q, const Pos& p0, const Pos& p1, const Pos& p2) {
	Circle c0 = Circle(p0, (p0 - q).mag());
	Circle c1 = Circle(p1, (p1 - q).mag());
	Circle c2 = Circle(p2, (p2 - q).mag());
	ld a = 0;
	Vld V = { 0, 2 * PI };
	Vld i10 = intersections(c1, c0);
	Vld i12 = intersections(c1, c2);
	for (const ld& x : i10) V.push_back(x);
	for (const ld& x : i12) V.push_back(x);
	std::sort(V.begin(), V.end());
	int sz = V.size();
	for (int i = 0; i < sz - 1; i++) {
		const ld& s = V[i], & e = V[i + 1];
		ld m = (s + e) * .5;
		Pos mid = c1.p(m);
		if (c0 > mid && c2 > mid) a += c1.green(s, e);
	}
	return a;
}
ld A[LEN];
void query(const int& t) {
	std::cin >> N >> M;
	Polygon P(N); for (Pos& p : P) std::cin >> p;
	Polygon Q(M); for (Pos& p : Q) std::cin >> p;
	memset(A, 0, sizeof A);
	for (int j = 0; j < M; j++) {
		const Pos& q = Q[j];
		Polygon R = P;
		for (Pos& p : R) p -= q;
		Polygon H = graham_scan(R);
		if (inner_check(H, O)) { A[j] = 0; continue; }
		std::sort(R.begin(), R.end(), [&](const Pos& p, const Pos& q) -> bool {
			int tq = sign(p / q);
			return !tq ? p.Euc() < q.Euc() : tq > 0;
			});
		Polygon S;
		for (int i = 0; i < N; i++) {
			while (S.size() > 1 && ccw(S[S.size() - 2], S[S.size() - 1], R[i]) > 0)
				S.pop_back();
			S.push_back(R[i]);
		}
		int sz = S.size();
		for (int i = 0; i < sz; i++) 
			A[j] += green(O, S[(i - 1 + sz) % sz], S[i], S[(i + 1) % sz]);
	}
	std::cout << "Case #" << t << ": ";
	for (int j = 0; j < M; j++) std::cout << A[j] << " ";
	std::cout << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> T;
	for (int t = 1; t <= T; t++) query(t);
}
int main() { solve(); return 0; }//boj12570 Grazing Google Goats (Large)