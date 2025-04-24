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
inline ld power(ld a, ll b) {
	ld ret = 1;
	while (b) {
		if (b & 1) ret *= a;
		a *= a;
		b >>= 1;
	}
	return ret;
}

int N, K, R, X[LEN], Y[LEN];
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
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
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
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
} C[LEN];
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
struct Event {
	ld x;
	int f;
	Event(ld x_ = 0, int f_ = 0) : x(x_), f(f_) {}
	bool operator < (const Event& o) const { return eq(x, o.x) ? f < o.f : sign(x - o.x) < 0; }
};
typedef std::vector<Event> Ve;
Ve V[LEN];
Vld P[LEN];
ld A[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> K >> R;
	for (int i = 0; i < N; i++) {
		std::cin >> X[i] >> Y[i];
		C[i] = Circle(Pos(X[i], Y[i]), R);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			ll d1 = sq((ll)X[i] - X[j]) + sq((ll)Y[i] - Y[j]);
			ll d2 = sq(R * 2ll);
			if (d1 > d2) continue;
			Vld inxs = intersections(C[i], C[j]);
			if (inxs.size() < 2) continue;
			ld lo = inxs[0], hi = inxs[1];
			if (lo < hi) {
				V[i].push_back({ lo, 1 });
				V[i].push_back({ hi, -1 });
			}
			else {
				V[i].push_back({ 0, 1 });
				V[i].push_back({ hi, -1 });
				V[i].push_back({ lo, 1 });
				V[i].push_back({ PI * 2, -1 });
			}
			P[i].push_back(hi);
			P[i].push_back(lo);
		}
		P[i].push_back(0);
		P[i].push_back(PI * 2);
		std::sort(P[i].begin(), P[i].end());
		P[i].erase(unique(P[i].begin(), P[i].end()), P[i].end());
		V[i].push_back({ 0, 1 });
		V[i].push_back({ PI * 2, -1 });
		std::sort(V[i].begin(), V[i].end());
	}
	memset(A, 0, sizeof A);
	for (int i = 0; i < N; i++) {
		int szp = P[i].size();
		int szv = V[i].size();
		int cnt = 0;
		for (int j = 0, k = 0; j < szp - 1; j++) {
			const ld& s = P[i][j], e = P[i][j + 1];
			while (k < szv && eq(V[i][k].x, s)) { cnt += V[i][k].f; k++; }
			ld a = C[i].green(s, e);
			if (cnt > 0) A[cnt] += a;
			if (cnt > 1) A[cnt - 1] -= a;
		}
	}
	ld t = 0;
	for (int i = 1; i <= N; i++) {
		ld r = (ld)(N - i) / N;
		t += A[i] * (1 - power(r, K));
	}
	std::cout << t << "\n";
	return;
}
int main() { solve(); return 0; }//boj8303 The Goat

/*

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
inline ld power(ld a, ll b) {
	ld ret = 1;
	while (b) {
		if (b & 1) ret *= a;
		a *= a;
		b >>= 1;
	}
	return ret;
}

int N, K, R, X[LEN], Y[LEN];
ld A[LEN];
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
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
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
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
} C[LEN];
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
struct Event {
	ld x;
	int f;
	bool operator < (const Event& o) const { return eq(x, o.x) ? f < o.f : sign(x - o.x) < 0; }
};
typedef std::vector<Event> Ve;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> N >> K >> R;
	for (int i = 0; i < N; i++) {
		std::cin >> X[i] >> Y[i];
		C[i] = Circle(Pos(X[i], Y[i]), R);
	}
	memset(A, 0, sizeof A);
	for (int i = 0; i < N; i++) {
		Ve V = { { 0, 1 }, { PI * 2, -1 } };
		Vld P = { 0, PI * 2 };
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			ll d1 = sq((ll)X[i] - X[j]) + sq((ll)Y[i] - Y[j]);
			ll d2 = sq(R * 2ll);
			if (d1 > d2) continue;
			Vld inxs = intersections(C[i], C[j]);
			if (inxs.size() < 2) continue;
			ld lo = inxs[0], hi = inxs[1];
			if (lo < hi) {
				V.push_back({ lo, 1 });
				V.push_back({ hi, -1 });
			}
			else {
				V.push_back({ 0, 1 });
				V.push_back({ hi, -1 });
				V.push_back({ lo, 1 });
				V.push_back({ PI * 2, -1 });
			}
			P.push_back(hi); P.push_back(lo);
		}
		std::sort(V.begin(), V.end());
		std::sort(P.begin(), P.end());
		P.erase(unique(P.begin(), P.end()), P.end());
		int szp = P.size(), szv = V.size(), cnt = 0;
		for (int j = 0, k = 0; j < szp - 1; j++) {
			const ld& s = P[j], e = P[j + 1];
			while (k < szv && eq(V[k].x, s)) { cnt += V[k].f; k++; }
			ld a = C[i].green(s, e);
			if (cnt > 0) A[cnt] += a;
			if (cnt > 1) A[cnt - 1] -= a;
		}
	}
	ld t = 0;
	for (int i = 1; i <= N; i++) {
		ld r = (ld)(N - i) / N;
		t += A[i] * (1 - power(r, K));
	}
	std::cout << t << "\n";
	return;
}
int main() { solve(); return 0; }//boj8303 The Goat


*/