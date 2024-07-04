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
typedef std::unordered_set<int> Sint;
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
typedef std::unordered_set<Pos> Seg_set;
std::vector<Pos> C, H;
struct Seg {
	Pos s, e;
	Seg(Pos S = 0, Pos E = 0) { if (E < S) std::swap(S, E); s = S, e = E; }
	bool operator == (const Seg& p) const { return s == p.s && e == p.e; }
	bool operator < (const Seg& p) const { return s == p.s ? e < p.e : s < p.s; }
	friend std::ostream& operator << (std::ostream& os, const Seg& S) { os << S.s << " " << S.e; return os; }
};
typedef std::vector<Seg> Segs;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return !ret ? 0 : ret > 0 ? 1 : -1; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
int ccw(const Seg& S, const Pos& p) { return ccw(S.s, S.e, p); }
std::vector<Pos> monotone_chain(std::vector<Pos>& C, int rvs = 1) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) * rvs <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) * rvs <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
std::vector<Pos> monotone_chain(std::vector<Pos>& C, const Pos& v1, const Pos& v2, int rvs = 1) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end(), [&](const Pos& p, const Pos& q) -> bool {
		ll d1 = dot(v1, v2, p);
		ll d2 = dot(v1, v2, q);
		if (d1 == d2) return ((p - v1).Euc() < (q - v1).Euc());
		return d1 < d2;
		});
	if (C.size() <= 2) { for (const Pos& pos : C) H.push_back(pos); }
	else {
		for (int i = 0; i < C.size(); i++) {
			while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) * rvs <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
		int s = H.size() + 1;
		for (int i = C.size() - 1; i >= 0; i--) {
			while (H.size() > s && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) * rvs <= 0)
				H.pop_back();
			H.push_back(C[i]);
		}
		H.pop_back();
	}
	return H;
}
Polygon conquer(Polygon L, Polygon R, Seg_set& all) {
	Polygon L = monotone_chain(L, 1);
	Polygon R = monotone_chain(R, -1);
	return {};
}
Polygon divide(Polygon P, Seg_set& all) {
	if (P.size() == 1) return P;
	if (P.size() == 2) {
		all.insert(Pos(P[0].i, P[1].i).norm());
		return P;
	}
	if (P.size() == 3) {
		all.insert(Pos(P[0].i, P[1].i).norm());
		all.insert(Pos(P[1].i, P[2].i).norm());
		all.insert(Pos(P[2].i, P[0].i).norm());
		if (ccw(P[0], P[1], P[2]) < 0) std::swap(P[1], P[2]);
		return P;
	}
	Polygon L, R;
	//std::sort(P.begin(), P.end());
	int sz = P.size();
	int m = sz >> 1;
	for (int i = 0; i < m; i++) L.push_back(P[i]);
	for (int i = m; i < sz; i++) R.push_back(P[i]);
	return conquer(divide(L, all), divide(R, all), all);
}
Seg_set triangulation_include_done(const Polygon& P, Pos p1, Pos p2, Pos q1, Pos q2) { 
	if (!ccw(p1, p2, q1, q2)) assert(1);
	Polygon C1, C2, C3;
	Seg_set all;
	if (on_seg_strong(p1, p2, q1) || on_seg_strong(p1, p2, q2)) {
		Pos p3 = on_seg_strong(p1, p2, q1) ? q2 : q1;
		if (ccw(p1, p2, p3) < 0) std::swap(p2, p3);
		Polygon C1, C2, C3;
		C1 = { p1, p2, p3 };
		int sz = P.size();
		for (int i = 0; i < sz; i++) {
			if (ccw(p1, p2, P[i]) > 0 && ccw(p2, p3, P[i]) > 0) C1.push_back(P[i]);
			else if (ccw(p1, p2, P[i]) > 0) C2.push_back(P[i]);
			else C3.push_back(P[i]);
		}
		std::sort(C1.begin(), C1.end());
		std::sort(C2.begin(), C2.end());
		std::sort(C3.begin(), C3.end());
		Polygon H1 = divide(C1, all);
		Polygon H2 = divide(C2, all);
		Polygon H3 = divide(C3, all);
		H1 = monotone_chain(H1, p2, p3, 1);
		H2 = monotone_chain(H2, p2, p3, 1);
		Polygon H4 = conquer(H1, H2, all);
		H3 = monotone_chain(H3, p1, p2, 1);
		H4 = monotone_chain(H4, p1, p2, 1);
		conquer(H3, H4, all);
	}
	else {
		Seg fst, snd;
		if (ccw(p1, p2, q1) == ccw(p1, p2, q2)) fst = Seg(q1, q2), snd = Seg(p1, p2);
		else fst = Seg(p1, p2), snd = Seg(q1, q2);
		Polygon C1, C2, C3;
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
		std::sort(C1.begin(), C1.end());
		std::sort(C2.begin(), C2.end());
		std::sort(C3.begin(), C3.end());
		Polygon H1 = divide(C1, all);
		Polygon H2 = divide(C2, all);
		H1 = monotone_chain(H1, snd.s, snd.e, 1);
		H2 = monotone_chain(H2, snd.s, snd.e, 1);
		Polygon H3 = divide(C3, all);
		Polygon H4 = conquer(H1, H2, all);
		H3 = monotone_chain(H3, fst.s, fst.e, 1);
		H4 = monotone_chain(H4, fst.s, fst.e, 1);
		conquer(H3, H4, all);
	}
	return all;
}
void solve(const int& t) {
	std::cin >> N;
	Polygon P(N);
	for (Pos& p : P) std::cin >> p;
	int a1, a2, b1, b2;
	std::cin >> a1 >> a2 >> b1 >> b2;
	a1--, a2--, b1--, b2--;
	Pos p1 = P[a1], p2 = P[a2], q1 = P[b1], q2 = P[b2];
	std::cout << "Case #" << t << " :";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	for (int t = 1; t <= T; t++) solve(t);
	return;
}