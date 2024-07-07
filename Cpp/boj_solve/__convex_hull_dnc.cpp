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
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int N, T;
struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos norm() const { if (y < x) return Pos(y, x); return Pos(x, y); }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Seg {
	Pos s, e;
	Seg(Pos S = 0, Pos E = 0) : s(S), e(E) {}
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Seg& S, const Pos& p) { return cross(S.s, S.e, p); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Seg& S, const Pos& p) { return ccw(S.s, S.e, p); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Polygon conquer(const Polygon& L, const Polygon& R, const Seg& v = Seg(), const bool& f = 1) {
	int szl = L.size(), szr = R.size();
	if (!szl) return R;
	if (!szr) return L;
	int il = 0, ir = 0;
	if (f) {
		for (int i = 0; i < szl; i++) if (L[il].x < L[i].x) il = i;
		for (int i = 0; i < szr; i++) if (R[ir].x > R[i].x) ir = i;
	}
	else {
		auto dist = [&](const Pos& p) -> ll { return std::abs(cross(v, p)); };
		for (int i = 0; i < szl; i++) if (dist(L[il]) > dist(L[i])) il = i;
		for (int i = 0; i < szr; i++) if (dist(R[ir]) > dist(R[i])) ir = i;
	}

	int jl, jr, lhi, llo, rhi, rlo;

	jl = il, jr = ir;
	while (1) {
		lhi = (jl + 1) % szl;
		if (ccw(R[jr], L[jl], L[lhi]) < 0) {
			jl = lhi;
			continue;
		}
		rlo = (jr - 1 + szr) % szr;
		if (ccw(L[jl], R[jr], R[rlo]) > 0) {
			jr = rlo;
			continue;
		}
		break;
	}
	lhi = jl; rlo = jr;

	jl = il, jr = ir;
	while (1) {
		llo = (jl - 1 + szl) % szl;
		if (ccw(R[jr], L[jl], L[llo]) > 0) {
			jl = llo;
			continue;
		}
		rhi = (jr + 1) % szr;
		if (ccw(L[jl], R[jr], R[rhi]) < 0) {
			jr = rhi;
			continue;
		}
		break;
	}
	llo = jl; rhi = jr;

	Polygon H;
	H.push_back(L[lhi]);
	while (lhi != llo) {
		lhi = (lhi + 1) % szl;
		H.push_back(L[lhi]);
	}
	H.push_back(R[rhi]);
	while (rhi != rlo) {
		rhi = (rhi + 1) % szr;
		H.push_back(R[rhi]);
	}
	return H;
}
Polygon divide(const Polygon& P, const int& s, const int& e) {
	if (e - s == 0) return { P[s] };
	if (e - s == 1) return { P[s], P[e] };
	if (e - s == 2) {
		Pos p0 = P[s], p1 = P[s + 1], p2 = P[e];
		if (ccw(p0, p1, p2) < 0) std::swap(p1, p2);
		return { p0, p1, p2 };
	}
	int m = (s + e) >> 1;
	return conquer(divide(P, s, m), divide(P, m + 1, e));
}
Polygon convex_hull_dnc(const Polygon& P) {
	int sz = P.size();
	assert(sz);
	if (sz <= 2) return P;
	if (sz == 3) {
		Pos p0 = P[0], p1 = P[1], p2 = P[2];
		if (ccw(p0, p1, p2) < 0) std::swap(p1, p2);
		return { p0, p1, p2 };
	}
	return divide(P, 0, sz - 1);
}
void solve(const int& t) {
	std::cin >> N;
	Polygon P(N);
	int i = 1;
	for (Pos& p : P) std::cin >> p, p.i = i++;
	Polygon ans = convex_hull_dnc(P);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	for (int t = 1; t <= T; t++) solve(t);
	return;
}
int main() { solve(); return 0; }
