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
	Pos norm() const { assert(x != y); if (y < x) return Pos(y, x); return Pos(x, y); }
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
Polygon conquer(const Polygon& L, const Polygon& R, Polygon& all, const Pos& v1 = Pos(), const Pos& v2 = Pos(), const bool& f = 1) {
	int szl = L.size(), szr = R.size();
	if (!szl) return R;
	if (!szr) return L;
	int il = 0, ir = 0;
	if (f) {
		for (int i = 0; i < szl; i++) if (L[il].x < L[i].x) il = i;
		for (int i = 0; i < szr; i++) if (R[ir].x > R[i].x) ir = i;
	}
	else {
		auto dist = [&](const Pos& p) -> ll { return std::abs(cross(v1, v2, p)); };
		for (int i = 0; i < szl; i++) if (dist(L[il]) > dist(L[i])) il = i;
		for (int i = 0; i < szr; i++) if (dist(R[ir]) > dist(R[i])) ir = i;
	}
	all.push_back(Pos(L[il].i, R[ir].i).norm());

	int jl, jr, lhi, llo, rhi, rlo;

	jl = il, jr = ir;
	while (1) {
		lhi = (jl + 1) % szl;
		if (ccw(R[jr], L[jl], L[lhi]) < 0) {
			all.push_back(Pos(L[lhi].i, R[jr].i).norm());
			jl = lhi;
			continue;
		}
		rlo = (jr - 1 + szr) % szr;
		if (ccw(L[jl], R[jr], R[rlo]) > 0) {
			all.push_back(Pos(L[jl].i, R[rlo].i).norm());
			jr = rlo;
			continue;
		}
		break;
	}
	lhi = jl; rlo = jr;
	all.push_back(Pos(L[lhi].i, R[rlo].i).norm());

	jl = il, jr = ir;
	while (1) {
		llo = (jl - 1 + szl) % szl;
		if (ccw(R[jr], L[jl], L[llo]) > 0) {
			all.push_back(Pos(L[llo].i, R[jr].i).norm());
			jl = llo;
			continue;
		}
		rhi = (jr + 1) % szr;
		if (ccw(L[jl], R[jr], R[rhi]) < 0) {
			all.push_back(Pos(L[jl].i, R[rhi].i).norm());
			jr = rhi;
			continue;
		}
		break;
	}
	llo = jl; rhi = jr;
	all.push_back(Pos(L[llo].i, R[rhi].i).norm());

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
Polygon divide(Polygon P, Polygon& all) {
	if (P.size() <= 1) return P;
	if (P.size() == 2) {
		all.push_back(Pos(P[0].i, P[1].i).norm());
		return P;
	}
	if (P.size() == 3) {
		for (int i = 0; i < 3; i++)
			all.push_back(Pos(P[i].i, P[(i + 1) % 3].i).norm());
		if (ccw(P[0], P[1], P[2]) < 0) std::swap(P[1], P[2]);
		return P;
	}
	int sz = P.size();
	int m = sz >> 1;
	if (sz == 4) m++;//avoid 2 | 2
	Polygon L(P.begin(), P.begin() + m);
	Polygon R(P.begin() + m, P.end());
	return conquer(divide(L, all), divide(R, all), all);
}
Polygon convex_hull_dnc(const Polygon& P, Pos p1, Pos p2, Pos q1, Pos q2) {
	assert(!collinear(p1, p2, q1, q2));
	Polygon C1, C2, C3, all;
	Seg fst, snd;
	int sz = P.size();
	if (!cross(p1, p2, q1) || !cross(p1, p2, q2)) {
		if (cross(p1, p2, q1) < 0 || cross(p1, p2, q2) < 0) std::swap(p1, p2);
		if (cross(q1, q2, p1) < 0 || cross(q1, q2, p2) < 0) std::swap(q1, q2);
		fst = Seg(p1, p2), snd = Seg(q1, q2);
		for (int i = 0; i < sz; i++) {
			if (cross(p1, p2, P[i]) >= 0 && cross(q1, q2, P[i]) >= 0) C1.push_back(P[i]);
			else if (cross(p1, p2, P[i]) > 0) C2.push_back(P[i]);
			else C3.push_back(P[i]);
		}
	}
	else {
		if (ccw(p1, p2, q1) == ccw(p1, p2, q2)) fst = Seg(p1, p2), snd = Seg(q1, q2);
		else fst = Seg(q1, q2), snd = Seg(p1, p2);
		int ccw2 = ccw(fst, snd.s);
		for (int i = 0; i < sz; i++) {
			if (ccw(fst, P[i]) == ccw2) {
				if (cross(snd, P[i]) > 0) C1.push_back(P[i]);
				else C2.push_back(P[i]);
			}
			else C3.push_back(P[i]);
		}
	}
	std::sort(C1.begin(), C1.end());
	std::sort(C2.begin(), C2.end());
	std::sort(C3.begin(), C3.end());
	Polygon H1, H2, H3, H4;
	H1 = divide(C1, all);
	H2 = divide(C2, all);
	H3 = divide(C3, all);
	H4 = conquer(H1, H2, all, snd.s, snd.e, 0);
	conquer(H3, H4, all, fst.s, fst.e, 0);
	std::sort(all.begin(), all.end());
	all.erase(unique(all.begin(), all.end()), all.end());
	return all;
}
void solve(const int& t) {
	std::cin >> N;
	Polygon P(N);
	int i = 1;
	for (Pos& p : P) std::cin >> p, p.i = i++;
	int a1, a2, b1, b2;
	std::cin >> a1 >> a2 >> b1 >> b2;
	Pos s1 = Pos(a1, a2).norm();
	Pos s2 = Pos(b1, b2).norm();
	a1--, a2--, b1--, b2--;
	Pos p1 = P[a1], p2 = P[a2], q1 = P[b1], q2 = P[b2];
	Polygon ans = convex_hull_dnc(P, p1, p2, q1, q2);
	std::cout << "Case #" << t << ": " << ans.size() - 2 << "\n";
	for (Pos& p : ans) if (p != s1 && p != s2) std::cout << p << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	for (int t = 1; t <= T; t++) solve(t);
	return;
}
int main() { solve(); return 0; }//boj22915 Fence Design
