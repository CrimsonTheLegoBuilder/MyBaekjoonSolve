#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
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
	inline BigInt(ll HI = 0, ll LO = 0) : hi(HI), lo(LO) {}
	inline BigInt operator - () const { return { -hi, -lo }; }
	inline BigInt norm() const { if (lo < 0) return -*this; return *this; }//den < 0
	inline bool operator < (const BigInt& p) const { return hi == p.hi ? lo < p.lo : hi < p.hi; }
	inline bool operator == (const BigInt& p) const { return hi == p.hi && lo == p.lo; }
};
inline BigInt mul(ll a, ll b) {
	assert(0 < b && b < DEN);
	int m = a < 0 ? -1 : 1;
	if (m < 0) a *= -1;
	ll hi = (a / DEN) * b;
	ll lo = (a % DEN) * b;
	hi += lo / DEN;
	lo %= DEN;
	return BigInt(m * hi, m * lo);
}
inline bool cmp_frac(BigInt p, BigInt q) { p = p.norm(); q = q.norm(); return mul(p.hi, q.lo) < mul(q.hi, p.lo); }
inline bool eq_frac(BigInt p, BigInt q) { p = p.norm(); q = q.norm(); return mul(p.hi, q.lo) == mul(q.hi, p.lo); }
struct Pos {
	ll x, y;
	ll den;
	bool dark;
	inline Pos(ll X = 0, ll Y = 0, ll d = 1, bool S = 0) : x(X), y(Y), den(d), dark(S) {}
	inline bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	inline bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ll& n) const { return { x * n, y * n }; }
	inline Pos operator / (const ll& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ll operator / (const Pos& p) const { return x * p.y - y * p.x; }
	inline Pos operator - () const { return { -x, -y, den }; }
	inline Pos operator ~ () const { return { -y, x, den }; }
	inline ll Euc() const { return x * x + y * y; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} H[LEN]; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct Line {//ax + by = c
	Pos s;
	ll c;
	inline Line(Pos ps = Pos(0, 0), Pos pe = Pos(0, 0)) { s = ~(pe - ps); c = s * ps; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s << " " << l.c; return os; }
};
inline Pos intersection(const Line& l1, const Line& l2) {
	Pos s = l1.s * l2.c - l2.s * l1.c;
	s.den = l1.s / l2.s;
	return ~s;
}
//Pos intersection(const Line& l1, const Line& l2) {
//	Pos v1 = l1.s, v2 = l2.s;
//	ld det = v1 / v2;
//	return {
//		(l1.c * v2.y - l2.c * v1.y) / det,
//		(l2.c * v1.x - l1.c * v2.x) / det,
//	};
//}
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
inline bool concave(const int& i) {
	Pos& p1 = H[(i - 1 + N) % N], & p2 = H[i];
	Pos& p3 = H[(i + 1) % N], & p4 = H[(i + 2) % N];
	return cross(p1, p2, p3) > 0 && cross(p2, p3, p4) > 0;
}
inline bool not_zig_zag(const int& i) {
	Pos& p1 = H[(i - 1 + N) % N], & p2 = H[i];
	Pos& p3 = H[(i + 1) % N], & p4 = H[(i + 2) % N];
	return ccw(p1, p2, p3) == ccw(p2, p3, p4);
}
inline bool inner_check(const Pos& p1, const Pos& p2, const Pos& p3, const Pos& q) { 
	return cross(p1, p2, q) < 0 && cross(p2, p3, q) < 0 && cross(p3, p1, q) < 0;
}
inline bool closer(const Pos& p1, const Pos& p2, const Pos& cur, const Pos& candi) {
	return std::abs(cross(p1, p2, cur)) > std::abs(cross(p1, p2, candi));
}
inline BigInt dot(const Pos& vec, const Pos& inx) { return BigInt(vec * inx, inx.den); }
inline bool include(const Line& l, const Pos& inx) { return cmp_frac(dot(l.s, inx), BigInt(l.c, 1)); }
inline bool on_line(const Line& l, const Pos& inx) { return eq_frac(dot(l.s, inx), BigInt(l.c, 1)); }
inline bool half_plane_intersection_include_x(const int& x = -1) {
	//x == -1 : no dark wall
	//x != -1 : only one dark wall
	 
	//the two walls next to the dark wall
	//must have a zigzag shape to be bright.
	if (x != -1 && not_zig_zag(x)) return 0;

	int sz = (x == -1 ? N : x + 1);
	int i = (x == -1 ? 0 : x);
	for (i; i < sz; i++) {//half_plane_intersection must include i1-i2
		if (x == -1 && concave(i)) continue;
		Pos& i1 = H[i], & i2 = H[(i + 1) % N];
		Pos vec = i2 - i1;
		bool rvs = 0;
		BigInt hi = { INF, 1 }, lo = { -INF, 1 };
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			Pos& j1 = H[j], & j2 = H[(j + 1) % N];
			ll det = cross(i1, i2, j1, j2);
			if (!det) {
				if (cross(j1, j2, i1) >= 0) { rvs = 1; break; }
				continue;
			}
			BigInt inx = dot(vec, intersection(Line(i1, i2), Line(j1, j2)));
			if (det < 0 && cmp_frac(inx, hi)) hi = inx;
			else if (det > 0 && cmp_frac(lo, inx)) lo = inx;
		}
		if (cmp_frac(lo, hi) && !rvs) return 1;
	}
	return 0;
}
inline bool query() {
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> H[i];
	int illum = 0, dark = -1;
	for (int i = 0; i < N; i++) {
		std::cin >> sc;
		H[i].dark = sc == 'C';
		if (!H[i].dark) illum++;
		if (H[i].dark) dark = i;
	}

	if (illum < 3) return 0;
	if (illum >= N - 1) return half_plane_intersection_include_x(dark);

	int i1 = 0, i2 = 0, i3 = 0;

	while (1) {//merge all dark walls
		//a room with only dark walls, leaving only a door.

		if (N <= 3) return 0;//at least one must be dark.

		i1 = 0; i2 = 0; i3 = 0;
		for (i1 = 0; i1 < N; i1++) {
			i2 = (i1 + 1) % N;
			i3 = (i2 + 1) % N;
			if (H[i1].dark && H[i2].dark && cross(H[i1], H[i2], H[i3]) < 0) break;
		}

		if (i1 == N) break;//all dark walls are unattached.

		int c = -1;//closest point to wall i1-i2
		for (int i = 0; i < N; i++) {
			if (i == i1 || i == i2 || i == i3) continue;
			if (inner_check(H[i1], H[i2], H[i3], H[i])) {
				if (c == -1 || closer(H[i1], H[i2], H[c], H[i]))
					c = i;
			}
		}

		if (c == -1) {//merge two dark walls
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
		
		if (!dark_hi && !dark_lo) return 0;//both sides of a dark passage must be bright.

		if (dark_hi) {//merge dark walls
			int len = c2 - c1 - 1;
			for (int i = c2; i < N; i++) H[i - len] = H[i];
			N -= len;
		}
		if (dark_lo) {//merge dark walls
			for (int i = c1; i <= c2; i++) H[i - c1] = H[i];
			N = c2 - c1 + 1;
		}
	}

	i1 = 0;
	while (!H[i1].dark) i1++;
	i2 = i1 + 1;
	while (i2 < N && !H[i2].dark) i2++;

	//cnly one dark wall
	if (i2 == N) return half_plane_intersection_include_x(i1);

	//two or more dark walls
	//all dark walls must intersect at a point
	//and all left half-plane of light wall must include that point.
	Pos& w1 = H[i1], & w2 = H[(i1 + 1) % N];
	Pos& w3 = H[i2], & w4 = H[(i2 + 1) % N];
	if (!cross(w1, w2, w3, w4)) return 0;
	Pos inx = intersection(Line(w1, w2), Line(w3, w4));
	for (int i = 0; i < N; i++) {
		Pos& p1 = H[i], & p2 = H[(i + 1) % N];
		if (p1.dark) {
			if (p2.dark) return 0;
			if (not_zig_zag(i)) return 0;
			if (!on_line(Line(p1, p2), inx)) return 0;
		}
		if (!p1.dark && !include(Line(p1, p2), inx)) return 0;
	}
	return 1;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) std::cout << (query() ? "TAK\n" : "NIE\n");
	return;
}
int main() { solve(); return 0; }//boj8237 Tapestries refer to model_code (oj.uz)
