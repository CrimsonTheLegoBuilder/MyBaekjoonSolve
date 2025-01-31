//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//#include <queue>
//#include <deque>
//#include <random>
//#include <array>
//#include <tuple>
//#include <complex>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//typedef std::pair<int, int> pi;
//typedef std::vector<int> Vint;
//typedef std::vector<ld> Vld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const int LEN = 1e3;
//int N, M, T, Q;
//inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
//inline bool zero(const ld& x) { return !sign(x); }
//inline ll sq(int x) { return (ll)x * x; }
//inline ld norm(ld th) {
//	while (th < 0) th += 2 * PI;
//	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
//	return th;
//}
//ld flip(ld lat) {
//	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
//	if (zero(lat)) return PI * .5;
//	if (lat > 0) return PI * .5 - lat;
//	if (lat < 0) return -(PI * .5) - lat;
//	return INF;
//}
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//
//struct Pos {
//	ld x, y;
//	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
//	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ld& n) const { return { x * n, y * n }; }
//	Pos operator / (const ld& n) const { return { x / n, y / n }; }
//	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
//	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
//	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
//	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
//	Pos operator - () const { return { -x, -y }; }
//	Pos operator ~ () const { return { -y, x }; }
//	Pos operator ! () const { return { y, x }; }
//	ld xy() const { return x * y; }
//	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return sqrt(Euc()); }
//	Pos unit() const { return *this / mag(); }
//	ld rad() const { return atan2(y, x); }
//	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
//	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
//	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
//	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = { 0, 0 };
//typedef std::vector<Pos> Polygon;
//bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
//bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
////bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
//struct Vec {
//	ld vy, vx;
//	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
//	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
//	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
//	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
//	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
//	Vec operator ~ () const { return { -vx, vy }; }
//	Vec& operator *= (const ld& n) { vy *= n; vx *= n; return *this; }
//	Vec& operator /= (const ld& n) { vy /= n; vx /= n; return *this; }
//	ld mag() const { return hypot(vy, vx); }
//}; const Vec Zero = { 0, 0 };
//struct Line {//ax + by = c
//	Vec s;
//	ld c;
//	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
//	bool operator < (const Line& l) const {
//		bool f1 = Zero < s;
//		bool f2 = Zero < l.s;
//		if (f1 != f2) return f1;
//		ld CCW = s / l.s;
//		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
//	}
//	ld operator * (const Line& l) const { return s * l.s; }
//	ld operator / (const Line& l) const { return s / l.s; }
//	Line operator + (const ld& n) const { return Line(s, c + hypot(s.vy, s.vx) * n); }
//	Line operator - (const ld& n) const { return Line(s, c - hypot(s.vy, s.vx) * n); }
//	Line operator * (const ld& n) const { return Line({ s.vy * n, s.vx * n }, c * n); }
//	Line& operator += (const ld& n) { c += hypot(s.vy, s.vx) * n; return *this; }
//	Line& operator -= (const ld& n) { c -= hypot(s.vy, s.vx) * n; return *this; }
//	Line& operator *= (const ld& n) { s *= n, c *= n; return *this; }
//	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
//	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
//	ld mag() const { return s.mag(); }
//	friend inline ld rad(const Line& b, const Line& l) { return atan2(b / l, b * l); }
//	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
//};
//Pos intersection(const Line& l1, const Line& l2) {
//	Vec v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.vx - l2.c * v1.vx) / det,
//		(l2.c * v1.vy - l1.c * v2.vy) / det,
//	};
//}

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const int LEN = 1 << 17;
inline int sign(const ld& x) { return x < TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }

int n, t;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return eq(x, p.x) && eq(y, p.y); }
};
struct Line {
	int a, b, c;//ax + by + c = 0
	Line(int a_ = 0, int b_ = 0, int c_ = 0) : a(a_), b(b_), c(c_) {}
	ll operator / (const Line& l) const { return (ll)a * l.b - (ll)b * l.a; }
	ll operator * (const Line& l) const { return (ll)a * l.a + (ll)b * l.b; }
	Line& operator *= (const int& n) { a *= n, b *= n, c *= n; return *this; }
	ld dist(const Pos& p) const { return a * p.x + b * p.y + c; }
} l, L[LEN], T[25];
Pos intersection(const Line& u, const Line& v) {
	ld det = u / v;
	return {
		(v.c * u.b - u.c * v.b) / det,
		(u.c * v.a - v.c * u.a) / det,
	};
}
bool cmp(const Line& u, const Line& v) {
	Pos pu = intersection(l, u);
	Pos pv = intersection(l, v);
	if (pu == pv) return u / v < 0;
	return v.dist(pu) > 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(12);
	std::cin >> n >> t;
	for (int i = 0; i < n; i++) std::cin >> L[i].a >> L[i].b >> L[i].c;
	for (int i = 0; i < t; i++) std::cin >> T[i].a >> T[i].b >> T[i].c;
	ld ret = INF;
	for (int i = 0; i < t; i++) {
		std::vector<Line> V, S;
		ld d = INF;
		for (int j = 0; j < n; j++) {
			if (T[t] / L[j] < 0 || (T[t] / L[j] == 0 && T[t] * L[j] < 0)) L[j] *= -1;
			if (T[t] / L[j] != 0) V.push_back(L[j]);
			else d = std::min(d, std::abs((T[t].c / hypot(T[t].a, T[t].b) - L[j].c / hypot(L[j].a, L[j].b))));
		}
		int sz = V.size();
		for (int z = 0; z < 2; z++) {
			l = T[t]; std::sort(V.begin(), V.end(), cmp);
			S.clear();
			for (int j = 0; j < sz - 1; j++) {
				//while (S.size() && )
			}
			for (int j = 0; j < sz; j++) L[j] *= -1;
			T[t] *= -1;
		}
	}
}