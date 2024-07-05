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
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int N, M, T, Q;
struct Pos {
	int x, y;
	int i;
	Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos s() const { return { x, -y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	//Pos norm() const { assert(x != y); if (y < x) return Pos(y, x); return Pos(x, y); }
	Pos norm() const { if (y < x) return Pos(y, x); return Pos(x, y); }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
typedef std::vector<Pos> Polygon;
struct Seg {
	Pos s, e;
	Seg(Pos S = 0, Pos E = 0) : s(S), e(E) {}
	bool operator == (const Seg& p) const { return s == p.s && e == p.e; }
	bool operator < (const Seg& p) const { return s == p.s ? e < p.e : s < p.s; }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << S.s << " " << S.e; return os; }
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return !ret ? 0 : ret > 0 ? 1 : -1; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
int ccw(const Seg& S, const Pos& p) { return ccw(S.s, S.e, p); }
bool inner_check(const Pos& p1, const Pos& p2, const Pos& p3, const Pos& q) { return cross(p1, p2, q) > 0 && cross(p2, p3, q) > 0 && cross(p3, p1, q) > 0; }
std::vector<Pos> graham_scan(std::vector<Pos> C) {
	std::vector<Pos> H;
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
Polygon conquer(Polygon L, Polygon R, Polygon& all, const Pos& v1 = Pos(), const Pos& v2 = Pos(), const bool& f = 1) {
	int il = 0, ir = 0;
	int szl = L.size(), szr = R.size();
	if (!szl) return R;
	if (!szr) return L;
	if (f) {
		for (int i = 0; i < szl; i++) if (L[il].x < L[i].x) il = i;
		for (int i = 0; i < szr; i++) if (R[ir].x > R[i].x) ir = i;
	}
	else {
		auto dist = [&](const Pos& p) -> ll { return std::abs(cross(v1, v2, p)); };
		for (int i = 0; i < szl; i++) if (dist(L[il]) > dist(L[i])) il = i;
		for (int i = 0; i < szr; i++) if (dist(R[ir]) > dist(R[i])) ir = i;
		//for (int i = 0; i < szl; i++) {
		//	std::cout << "L.dist:: p.i: " << L[i].i << " d: " << dist(L[i]) << "\n";
		//}
		//for (int i = 0; i < szr; i++) {
		//	std::cout << "R.dist:: p.i: " << R[i].i << " d: " << dist(R[i]) << "\n";
		//}
	}
	//std::cout << "DEBUG:: f: " << f << " l.i: " << L[il].i << " r.i: " << R[ir].i << "\n";
 	all.push_back(Pos(L[il].i, R[ir].i).norm());
	int jl = il, jr = ir, lhi, llo, rhi, rlo;
	//std::cout << "DEBUG:: l: " << L[il].i << " r: " << R[ir].i << "\n";
	while (1) {
		//std::cout << "DEBUG:: conq 1\n";
		lhi = (jl + 1) % szl;
		if (ccw(R[jr], L[jl], L[lhi]) < 0 &&
			!inner_check(R[jr], L[lhi], L[jl], R[(jr - 1 + szr) % szr])) {
			all.push_back(Pos(L[lhi].i, R[jr].i).norm());
			jl = lhi;
			continue;
		}
		rlo = (jr - 1 + szr) % szr;
		if (ccw(L[jl], R[jr], R[rlo]) > 0 &&
			!inner_check(L[jl], R[jr], R[rlo], L[(jl + 1) % szl])) {
			all.push_back(Pos(L[jl].i, R[rlo].i).norm());
			jr = rlo;
			continue;
		}
		break;
	}
	lhi = jl;
	rlo = jr;
	all.push_back(Pos(L[lhi].i, R[rlo].i).norm());
	jl = il, jr = ir;
	while (1) {
		//std::cout << "DEBUG: conq 2\n";
		llo = (jl - 1 + szl) % szl;
		if (ccw(R[jr], L[jl], L[llo]) > 0 &&
			!inner_check(R[jr], L[jl], L[llo], R[(jr + 1) % szr])) {
			all.push_back(Pos(L[llo].i, R[jr].i).norm());
			jl = llo;
			continue;
		}
		rhi = (jr + 1) % szr;
		if (ccw(L[jl], R[jr], R[rhi]) < 0 &&
			!inner_check(L[jl], R[rhi], R[jr], L[(jl - 1 + szl) % szl])) {
			all.push_back(Pos(L[jl].i, R[rhi].i).norm());
			jr = rhi;
			continue;
		}
		break;
	}
	llo = jl;
	rhi = jr;
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

	int sz = H.size();
	//assert(sz > 2);
	//for (int i = 0; i < sz; i++) assert(ccw(H[i], H[(i + 1) % sz], H[(i + 2) % sz]) > 0);
	
	//std::cout << "DEBUG::all sz:: " << all.size() << "\n";
	return H;
}
Polygon divide(Polygon P, Polygon& all) {
	if (P.size() <= 1) return P;
	if (P.size() == 2) {
		all.push_back(Pos(P[0].i, P[1].i).norm());
		return P;
	}
	if (P.size() == 3) {
		all.push_back(Pos(P[0].i, P[1].i).norm());
		all.push_back(Pos(P[1].i, P[2].i).norm());
		all.push_back(Pos(P[2].i, P[0].i).norm());
		if (ccw(P[0], P[1], P[2]) < 0) std::swap(P[1], P[2]);
		return P;
	}
	Polygon L, R;
	int sz = P.size();
	int m = sz >> 1;
	if (sz == 4) m++;//avoid 2 | 2
	for (int i = 0; i < m; i++) L.push_back(P[i]);
	for (int i = m; i < sz; i++) R.push_back(P[i]);
	return conquer(divide(L, all), divide(R, all), all);
}
Polygon convex_hull_dnc(const Polygon& P, Pos p1, Pos p2, Pos q1, Pos q2) {
	//assert(!collinear(p1, p2, q1, q2));
	Polygon C1, C2, C3, all;
	Seg fst, snd;
	int sz = P.size();
	if (!ccw(p1, p2, q1) || !ccw(p1, p2, q2)) {
		//std::cout << "DEBUG hull 1\n";
		if (ccw(p1, p2, q1) < 0 || ccw(p1, p2, q2) < 0) std::swap(p1, p2);
		if (ccw(q1, q2, p1) < 0 || ccw(q1, q2, p2) < 0) std::swap(q1, q2);
		fst = Seg(p1, p2), snd = Seg(q1, q2);
		for (int i = 0; i < sz; i++) {
			if (ccw(p1, p2, P[i]) >= 0 && ccw(q1, q2, P[i]) >= 0) C1.push_back(P[i]);
			else if (ccw(p1, p2, P[i]) > 0) C2.push_back(P[i]);
			else C3.push_back(P[i]);
		}
	}
	else {
		//std::cout << "DEBUG hull 2\n";
		if (ccw(p1, p2, q1) == ccw(p1, p2, q2)) fst = Seg(p1, p2), snd = Seg(q1, q2);
		else fst = Seg(q1, q2), snd = Seg(p1, p2);
		//assert(ccw(fst, snd.s) == ccw(fst, snd.e));
		int ccw2 = ccw(fst, snd.s);
		for (int i = 0; i < sz; i++) {
			if (ccw(fst, P[i]) == ccw2) {
				if (ccw(snd, P[i]) > 0) C1.push_back(P[i]);
				else C2.push_back(P[i]);
			}
			else C3.push_back(P[i]);
		}
	}
	//assert(P.size() == (C1.size() + C2.size() + C3.size()));
	//std::cout << "DEBUG::sz C1 " << C1.size() << "\n";
	//std::cout << "DEBUG::sz C2 " << C2.size() << "\n";
	//std::cout << "DEBUG::sz C3 " << C3.size() << "\n";
	//std::cout << "DEBUG hull pos init\n";
	Polygon GH;
	std::sort(C1.begin(), C1.end());
	std::sort(C2.begin(), C2.end());
	std::sort(C3.begin(), C3.end());
	Polygon H1 = divide(C1, all);
	GH = graham_scan(C1);
	assert(GH.size() == H1.size());
	//std::cout << "DEBUG hull divide1\n";
	Polygon H2 = divide(C2, all);
	GH = graham_scan(C2);
	assert(GH.size() == H2.size());
	//std::cout << "DEBUG hull divide2\n";
	Polygon H3 = divide(C3, all);
	GH = graham_scan(C3);
	assert(GH.size() == H3.size());
	//for (const Pos& p : H3) std::cout << "H3: " << p.i << " " << p << "\n";
	//for (const Pos& p : GH) std::cout << "GH: " << p.i << " " << p << "\n";
	//std::cout << "DEBUG:: 3\n";
	//std::cout << "DEBUG hull divide3\n";
	Polygon H4 = conquer(H1, H2, all, snd.s, snd.e, 0);
	Polygon C4;
	for (Pos& p : C1) C4.push_back(p);
	for (Pos& p : C2) C4.push_back(p);
	GH = graham_scan(C4);
	assert(GH.size() == H4.size());
	//for (const Pos& p : H4) std::cout << "H4: " << p.i << " " << p << "\n";
	//for (const Pos& p : GH) std::cout << "GH: " << p.i << " " << p << "\n";
	//std::cout << "DEBUG:: 4 \n";
	Polygon H5 = conquer(H3, H4, all, fst.s, fst.e, 0);
	GH = graham_scan(P);
	assert(GH.size() == H5.size());
	//for (const Pos& p : H5) std::cout << "H5: " << p.i << " " << p << "\n";
	//for (const Pos& p : GH) std::cout << "GH: " << p.i << " " << p << "\n";
	//std::cout << "DEBUG:: 5\n";
	//std::cout << "DEBUG:: GH.sz: " << GH.size() << " H5.sz: " << H5.size() << "\n";
	std::sort(all.begin(), all.end());
	all.erase(unique(all.begin(), all.end()), all.end());
	//std::cout << "DEBUG::fin::all sz " << all.size() << " ";
	//std::cout << all[0] << "\n";
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
	//std::cout << "DEBUG Seg init\n";
	Polygon ans = convex_hull_dnc(P, p1, p2, q1, q2);
	std::cout << "Case #" << t << ": " << ans.size() - 2 << "\n";
	//int cnt = 0;
	//for (Pos& p : ans) if (p == s1 || p == s2) cnt++;
	//assert(cnt == 2);
	for (Pos& p : ans) if (p != s1 && p != s2) std::cout << p << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("ts2_input.txt", "r", stdin);
	//freopen("ts2_out.txt", "w", stdout);
	std::cin >> T;
	for (int t = 1; t <= T; t++) solve(t);
	return;
}
int main() { solve(); return 0; }//boj22915 Fence Design

/*

1
13
828697006 828697142
828696973 828696992
828696695 828696708
828696461 828696948
828696473 828696841
828696315 828697173
828697073 828696418
828696309 828696498
828697265 828696733
828696764 828697009
828697052 828696529
828696609 828696980
828696576 828697265
5 6
10 12

Case #1: 28
13 1
13 10
10 1
13 12
13 4
4 12
4 10
4 2
2 10
2 1
4 3
3 2
3 9
9 2
9 1
3 11
11 9
3 7
7 11
7 9
13 6
6 4
5 4
5 3
5 7
6 8
8 5
8 7

*/
