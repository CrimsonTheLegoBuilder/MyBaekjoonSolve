#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
#include <unordered_set>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
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
int V[LEN];
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
std::vector<Pos> C, H;
struct Seg {
	Pos s, e;
	Seg(Pos S = 0, Pos E = 0) : s(S), e (E) {}
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
Polygon conquer(Polygon L, Polygon R, Polygon& all) {
	int il = 0, ir = 0;
	int szl = L.size(), szr = R.size();
	if (!szl) return R;
	if (!szr) return L;
	for (int i = 1; i < szl; i++) 
		if ((L[i] - R[0]).Euc() < (L[il] - R[0]).Euc())
			il = i;
	for (int i = 1; i < szr; i++)
		if ((R[i] - L[il]).Euc() < (R[ir] - L[il]).Euc())
			ir = i;
	all.push_back(Pos(L[il].i, R[ir].i).norm());
	int jl = il, jr = ir, lhi, llo, rhi, rlo;
	//std::cout << "DEBUG:: l: " << L[il].i << " r: " << R[ir].i << "\n";
	while (1) {
		//std::cout << "DEBUG:: conq 1\n";
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
	jl = il, jr = ir;
	while (1) {
		//std::cout << "DEBUG: conq 2\n";
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
	Polygon H;
	if (lhi <= llo) {
		for (int i = lhi; i <= llo; i++) H.push_back(L[i]);
	}
	else {
		for (int i = 0; i <= llo; i++) H.push_back(L[i]);
		for (int i = lhi; i < szl; i++) H.push_back(L[i]);
	}
	if (rhi <= rlo) {
		for (int i = rhi; i <= rlo; i++) H.push_back(R[i]);
	}
	else {
		for (int i = 0; i <= rlo; i++) H.push_back(R[i]);
		for (int i = rhi; i < szr; i++) H.push_back(R[i]);
	}
	//std::cout << "DEBUG::all sz:: " << all.size() << "\n";
	return H;
}
Polygon divide(Polygon P, Polygon& all) {
	//std::cout << "DEBUG::divide:: " << P.size() << "\n";
	//std::cout << "DEBUG::all sz:: " << all.size() << "\n";
	//std::cout << "DEBUG::\n";
	//for (Pos& p : P) std::cout << p << "\n";
	//std::cout << "DEBUG::\n";
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
	for (int i = 0; i < m; i++) L.push_back(P[i]);
	for (int i = m; i < sz; i++) R.push_back(P[i]);
	return conquer(divide(L, all), divide(R, all), all);
}
Polygon convex_hull_dnc(const Polygon& P, Pos p1, Pos p2, Pos q1, Pos q2) { 
	if (!collinear(p1, p2, q1, q2)) assert(1);
	Polygon C1, C2, C3, all;
	if (!ccw(p1, p2, q1) || !ccw(p1, p2, q2)) {
		//std::cout << "DEBUG hull 1\n";
		if (ccw(p1, p2, q1) < 0 || ccw(p1, p2, q2) < 0) std::swap(p1, p2);
		if (ccw(q1, q2, p1) < 0 || ccw(q1, q2, p2) < 0) std::swap(q1, q2);
		Pos p3 = !ccw(p1, p2, q1) ? q2 : q1;
		C1 = { p1, p2, p3 };
		int sz = P.size();
		for (int i = 0; i < sz; i++) {
			if (ccw(p1, p2, P[i]) > 0 && ccw(q1, q2, P[i]) > 0) C1.push_back(P[i]);
			else if (ccw(p1, p2, P[i]) > 0) C2.push_back(P[i]);
			else C3.push_back(P[i]);
		}
	}
	else {
		//std::cout << "DEBUG hull 2\n";
		Seg fst, snd;
		if (ccw(p1, p2, q1) == ccw(p1, p2, q2)) fst = Seg(q1, q2), snd = Seg(p1, p2);
		else fst = Seg(p1, p2), snd = Seg(q1, q2);
		int sz = P.size();
		bool r = 0;
		int ccw2 = ccw(fst, snd.s);
		for (int i = 0; i < sz; i++) {
			if (ccw(fst, P[i]) == ccw2) {
				if (ccw(snd, P[i]) > 0) C1.push_back(P[i]);
				else C2.push_back(P[i]);
			}
			else C3.push_back(P[i]);
		}
	}
	//std::cout << "DEBUG::sz C1 " << C1.size() << "\n";
	//std::cout << "DEBUG::sz C2 " << C2.size() << "\n";
	//std::cout << "DEBUG::sz C3 " << C3.size() << "\n";
	//std::cout << "DEBUG hull pos init\n";
	std::sort(C1.begin(), C1.end());
	std::sort(C2.begin(), C2.end());
	std::sort(C3.begin(), C3.end());
	Polygon H1 = divide(C1, all);
	//std::cout << "DEBUG hull divide1\n";
	Polygon H2 = divide(C2, all);
	//std::cout << "DEBUG hull divide2\n";
	Polygon H3 = divide(C3, all);
	//std::cout << "DEBUG hull divide3\n";
	Polygon H4 = conquer(H1, H2, all);
	conquer(H3, H4, all);
	//std::cout << "DEBUG:: before sort " << all.size() << "\n";
	std::sort(all.begin(), all.end());
	all.erase(unique(all.begin(), all.end()), all.end());
	//std::cout << "DEBUG::fin::all sz " << all.size() << " ";
	std::cout << all[0] << "\n";
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
	//std::cout << "DEBUG hull dnc\n";
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