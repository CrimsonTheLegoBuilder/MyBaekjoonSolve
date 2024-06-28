#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e3 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
const ll DEN = 1LL << 32;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int N, M, T, Q;
char sc;
struct BigInt {
	ll hi, lo;
	BigInt(ll HI = 0, ll LO = 0) : hi(HI), lo(LO) {}
	BigInt operator - () const { return { -hi, -lo }; }
	bool operator < (const BigInt& p) const { return hi == p.hi ? lo < p.lo : hi < p.hi; }
	bool operator == (const BigInt& p) const { return hi == p.hi && lo == p.lo; }
};
BigInt mul(ll num, ll den) {
	assert(0 < den && den < DEN);
	int m = num < 0 ? -1 : 1;
	if (m < 0) num *= -1;
	ll hi = (num / DEN) * den;
	ll lo = (num % DEN) * den;
	hi += lo / DEN;
	lo %= DEN;
	return BigInt(m * hi, m * lo);
}
struct Pos {
	ll x, y;
	ll den;
	bool dark;
	Pos(ll X = 0, ll Y = 0, ll d = 1, bool S = 0) : x(X), y(Y), den(d), dark(S) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ll operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y, den }; }
	Pos operator ~ () const { return { -y, x, den }; }
	Pos operator ! () const { return { y, x }; }
	Pos s() const { return { y, -x }; }
	ld xy() const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Pos H[LEN];
struct Line {//ax + by = c
	Pos s;
	ll c;
	Line(Pos ps = Pos(0, 0), Pos pe = Pos(0, 0)) {
		s = (pe - ps).s();
		c = s * ps;
	}
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s << " " << l.c; return os; }
};
Pos intersection(const Line& l1, const Line& l2) {
	ll den = l1.s * l2.s;
	Pos s = l1.s * l2.c - l2.s * l1.c;
	s.den = den;
	return ~s;
}
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return !ret ? 0 : ret > 0 ? 1 : -1; }
ll inner_check(const Pos& p1, const Pos& p2, const Pos& p3, const Pos& q) {
	return ccw(p1, p2, q) < 0 && ccw(p2, p3, q) < 0 && ccw(p3, p1, q) < 0;
}
BigInt dot(const Pos& vec, const Pos& inx) { return BigInt(vec * inx, inx.den); }
inline bool cmp_frac(BigInt p, BigInt q) {
	if (p.lo < 0) p = -p;//p.den < 0
	if (q.lo < 0) q = -q;//q.den < 0
	return mul(p.hi, q.lo) < mul(q.hi, p.lo);
}
inline bool frac_eq(BigInt p, BigInt q) {
	if (p.lo < 0) p = -p;//p.den < 0
	if (q.lo < 0) q = -q;//q.den < 0
	return mul(p.hi, q.lo) == mul(q.hi, p.lo);
}
bool above(const Line& l, const Pos& inx, bool rvs = 0) {
	BigInt dist = dot(l.s, inx);
	if (rvs) return cmp_frac(dist, BigInt(l.c, 1));
	return cmp_frac(BigInt(l.c, 1), dist);
}
bool on_line(const Line& l, const Pos& inx) {
	BigInt dist = dot(l.s, inx);
	return frac_eq(dist, BigInt(l.c, 1));
}
bool closer(const Pos& p1, const Pos& p2, const Pos& q, const Pos& candi) {
	ll ccwq = cross(p1, p2, q);
	ll numq = ccwq * ccwq;
	if (ccwq < 0) numq *= -1;
	ll denq = (q - p1).Euc();
	ll ccwx = cross(p1, p2, q);
	ll numx = ccwx * ccwx;
	if (ccwx < 0) numx *= -1;
	ll denx = (candi - p1).Euc();
	return cmp_frac(BigInt(numq, denq), BigInt(numx, denx));
}
//std::string all_light_check() {
//	for (int i = 0; i < N; i++) {//half_plane_intersection must include i1-i2
//		Pos& i1 = H[i], &i2 = H[(i + 1) % N];
//		Pos vec = i2 - i1;
//		bool rvs = 1;
//		BigInt hi = { INF, 1 }, lo = { -INF, 1 };
//		for (int j = 0; j < N; j++) {
//			if (j == i) continue;
//			Pos& j1 = H[j], &j2 = H[(j + 1) % N];
//			ll det = cross(i1, i2, j1, j2);
//			if (!det) {
//				if (ccw(j1, j2, i1) >= 0) { rvs = 0; break; }
//				continue;
//			}
//			BigInt inx = dot(vec, intersection(Line(i1, i2), Line(j1, j2)));
//			if (det < 0 && cmp_frac(inx, hi)) hi = inx;
//			else if (det > 0 && cmp_frac(lo, inx)) lo = inx;
//		}
//		if (cmp_frac(lo, hi) && rvs) return "TAK";
//	}
//	return "NIE";
//}
std::string half_plane_intersection_except_x(int x = -1) {
	int sz = (x == -1 ? N : x + 1);
	int i = (x == -1 ? 0 : x);
	for (i; i < sz; i++) {//half_plane_intersection must include i1-i2
		Pos& i1 = H[i], & i2 = H[(i + 1) % N];
		Pos vec = i2 - i1;
		bool rvs = 0;
		BigInt hi = { INF, 1 }, lo = { -INF, 1 };
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			Pos& j1 = H[j], & j2 = H[(j + 1) % N];
			ll det = cross(i1, i2, j1, j2);
			if (!det) {
				if (ccw(j1, j2, i1) >= 0) { rvs = 1; break; }
				continue;
			}
			BigInt inx = dot(vec, intersection(Line(i1, i2), Line(j1, j2)));
			if (det < 0 && cmp_frac(inx, hi)) hi = inx;
			else if (det > 0 && cmp_frac(lo, inx)) lo = inx;
		}
		if (cmp_frac(lo, hi) && !rvs) return "TAK";
	}
	return "NIE";
}
std::string query() {
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> H[i];
	bool all_light = 1, all_dark = 1;
	for (int i = 0; i < N; i++) {
		std::cin >> sc;
		H[i].dark = sc == 'C';
		if (!H[i].dark) all_dark = 0;
		if (H[i].dark) all_light = 0;
	}

	if (all_dark) return "NIE";
	if (all_light) return half_plane_intersection_except_x();

	int i1 = 0, i2 = 0, i3 = 0, i4 = 0;
	while (1) {//merge dark walls
		if (N <= 3) return "NIE";//at least one must be dark.

		i1 = 0; i2 = 0; i3 = 0;
		for (i1 = 0; i1 < N; i1++) {
			i2 = (i1 + 1) % N;
			i3 = (i2 + 1) % N;
			if (H[i1].dark && H[i2].dark && ccw(H[i1], H[i2], H[i3]) < 0) break;
		}

		if (i1 == N) break;//no continuous dark walls

		int c = -1;
		for (int i = 0; i < N; i++) {
			if (i == i1 || i == i2 || i == i3) continue;
			if (inner_check(H[i1], H[i2], H[i3], H[i])) {
				if (c == -1 || closer(H[i1], H[i2], H[c], H[i]))
					c = i;
			}
		}

		if (c == -1) {//merge two dark wall
			for (int i = i2 + 1; i < N; i++) H[i - 1] = H[i];
			N--;
			continue;
		}

		int c1 = i2, c2 = c;
		if (c2 < c1) std::swap(c1, c2);

		bool dark_hi = 1, dark_lo = 1;
		for (int i = 0; i < N; i++) {
			if (!H[i].dark) {
				if (c1 <= i && i < c2) dark_hi = 0;
				else dark_lo = 0;
			}
		}
		
		if (!dark_hi && !dark_lo) return "NIE";//both sides of a dark passage must be bright.

		if (dark_hi) {//merge dark walls
			for (int i = c2; i < N; i++) H[i - (c2 - c1 - 1)] = H[i];
			//H[c1].dark = 1;
			N -= (c2 - c1 - 1);
		}
		else {//merge dark walls
			for (int i = c1; i <= c2; i++) H[i - c1] = H[i];
			N = (c2 - c1 - 1);
			//H[N - 1].dark = 1;
		}
	}

	//for (int i = 0; i < N; i++) if (H[i].dark && H[(i + 1) % N].dark) return "NIE";

	for (int i = 0; i < N; i++) {
		/*
			If there is a light wall in the same direction
			on either side of a dark wall, a half-plane intersection is not formed.
		*/
		Pos& prev = H[i], & w1 = H[(i + 1) % N], & w2 = H[(i + 2) % N], & nxt = H[(i + 3) % N];
		if (w1.dark && ccw(prev, w1, w2) == ccw(w1, w2, nxt)) return "NIE";
	}

	i1 = 0, i2 = 0;
	while (!H[i1].dark) i1++;
	i2 = i1 + 1;
	while (i2 < N && !H[i2].dark) i2++;

	if (i2 < N) {//two or more dark walls
		/*
			All dark walls must intersect at a point
			and all light walls must contain that point.
		*/
		Pos& w1 = H[i1], & w2 = H[(i1 + 1) % N];
		Pos& w3 = H[i2], & w4 = H[(i2 + 1) % N];
		if (!ccw(w1, w2, w3, w4)) return "NIE";
		Pos inx = intersection(Line(w1, w2), Line(w3, w4));
		for (int i = 0; i < N; i++) {
			Pos& prev = H[(i - 1 + N) % N], & nxt = H[(i + 2) % N];
			Pos& p1 = H[i], & p2 = H[(i + 1) % N];
			if (p1.dark && p2.dark) return "NIE";
			if (p1.dark && ccw(prev, p1, p2) == ccw(p1, p2, nxt)) return "NIE";
			Line l(p1, p2);
			BigInt cur = dot(l.s, inx);
			if (!H[i].dark && !above(l, inx)) return "NIE";
			if (H[i].dark && !on_line(l, inx)) return "NIE";
		}
	}
	else {//only one dark wall
		/*
			There is a possible point somewhere
			along the straight line extending the dark wall.
		*/
		Pos& prev = H[(i1 - 1 + N) % N], & nxt = H[(i1 + 2) % N];
		Pos& p1 = H[i1], & p2 = H[(i1 + 1) % N];
		if (p1.dark && ccw(prev, p1, p2) == ccw(p1, p2, nxt)) return "NIE";
		return half_plane_intersection_except_x(i1);
		//Pos vec = w2 - w1;
		//BigInt hi = { INF, 1 }, lo = { -INF, 1 };
		//for (int i = 0; i < N; i++) {
		//	if (i == i1) continue;
		//	Pos& p1 = H[i], & p2 = H[(i + 1) % N];
		//	ll det = cross(w1, w2, p1, p2);
		//	if (!det) {
		//		if (ccw(p1, p2, w1) >= 0) return "NIE";
		//		continue;
		//	}
		//	BigInt inx = dot(vec, intersection(Line(w1, w2), Line(p1, p2)));
		//	if (det < 0 && cmp_frac(inx, hi)) hi = inx;
		//	else if (det > 0 && cmp_frac(lo, inx)) lo = inx;
		//}
		//if (!cmp_frac(lo, hi)) return "NIE";
	}
	return "TAK";
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) std::cout << query() << "\n";
	return;
}
int main() { solve(); return 0; }//boj8237 Tapestries refer to model_code (oj.uz)