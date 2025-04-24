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
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1 << 17;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }

#define FULL 0
#define HALF 1
#define OUTER 0
#define INNER 1
#define LIMIT (1 << 6 | 1 << 5 | 1 << 2)//WHAT THE FUCKING IMBEDDED

int N, M, K;
bool V[LEN];
struct Pos {
	int x, y, i;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) { i = 0; }
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
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
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
typedef std::vector<Pos> Polygon;
Polygon OUT[LEN], IN[LEN];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool inner_check(const Pos& d0, const Pos& d1, const Pos& d2, const Pos& q) {
	return ccw(d0, d1, q) >= 0 && ccw(d1, d2, q) >= 0 && ccw(d2, d0, q) >= 0;
}
bool inner_check(const Pos& d0, const Pos& d1, const Pos& d2, const Pos& d3, const Pos& q) {
	return ccw(d0, d1, q) >= 0 && ccw(d1, d2, q) >= 0 && ccw(d2, d3, q) >= 0 && ccw(d3, d0, q) >= 0;
}
Polygon graham_scan(Polygon& C) {
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
ld round(const Polygon& H, const bool& half = FULL) {
	ld R = 0;
	int sz = H.size();
	for (int i = 0; i < sz - half; i++) R += (H[i] - H[(i + 1) % sz]).mag();
	return R;
}
ld brute(Polygon& P) {//only use when N <= 100
	int sz = P.size();
	Polygon H = graham_scan(P);
	ld R = round(H);
	ld ret = INF;
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			Polygon C;
			for (int k = 0; k < sz; k++) {//O(100 * 100 * 100 * 8)
				if (k == i || k == j) continue;
				C.push_back(P[k]);
			}
			ret = std::min(ret, round(graham_scan(C)));
		}
	}
	return R - ret;
}
int inner_check_bi_search(const Polygon& H, const Pos& p, const int& n = 1, const bool& in = OUTER) {//convex
	int sz = H.size();
	assert(sz > 2);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p)) {
		if (in) { IN[0].push_back(p); IN[1].push_back(p); }
		else {
			OUT[0].push_back(p); OUT[1].push_back(p);
			if (n == 2) OUT[sz - 1].push_back(p);
		}
		return 0;
	}
	if (on_seg_strong(H[0], H[sz - 1], p)) {
		if (in) { IN[0].push_back(p); IN[sz - 1].push_back(p); }
		else {
			OUT[0].push_back(p); OUT[sz - 1].push_back(p);
			if (n == 2) OUT[sz - 2].push_back(p);
		}
		return 0;
	}
	if (n == 1) {
		if (inner_check(H[sz - 2], H[sz - 1], H[0], p)) {
			if (in) IN[sz - 1].push_back(p);
			else OUT[sz - 1].push_back(p);
		}
		if (inner_check(H[sz - 1], H[0], H[1], p)) {
			if (in) IN[0].push_back(p);
			else OUT[0].push_back(p);
		}
		if (inner_check(H[0], H[1], H[2], p)) {
			if (in) IN[1].push_back(p);
			else OUT[1].push_back(p);
		}
	}
	else if (n == 2) {
		if (inner_check(H[sz - 3], H[sz - 2], H[sz - 1], H[0], p)) OUT[sz - 2].push_back(p);
		if (inner_check(H[sz - 2], H[sz - 1], H[0], H[1], p)) OUT[sz - 1].push_back(p);
		if (inner_check(H[sz - 1], H[0], H[1], H[2], p)) OUT[0].push_back(p);
		if (inner_check(H[0], H[1], H[2], H[3], p)) OUT[1].push_back(p);
	}
	//bi-search
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	//bi-search
	if (cross(H[s], H[e], p) < 0) return -1;
	if (in) {
		if (on_seg_strong(H[s], H[e], p)) { IN[s].push_back(p); IN[e].push_back(p); return 0; }
		if (s == 2 && on_seg_weak(H[2], H[0], p)) { IN[1].push_back(p); }
		if (inner_check(H[s], H[e], H[(s - 1 + sz) % sz], p)) { IN[s].push_back(p); }
		if (inner_check(H[e], H[(e + 1) % sz], H[s], p)) { IN[e].push_back(p); }
		if (e == sz - 2 && on_seg_weak(H[sz - 2], H[0], p)) { IN[sz - 1].push_back(p); }
		return 1;
	}
	else {
		if (n == 1) {
			if (on_seg_strong(H[s], H[e], p)) { OUT[s].push_back(p); OUT[e].push_back(p); return 0; }
			if (s == 2 && on_seg_weak(H[2], H[0], p)) { OUT[1].push_back(p); }
			if (inner_check(H[s], H[e], H[(s - 1 + sz) % sz], p)) { OUT[s].push_back(p); }
			if (inner_check(H[e], H[(e + 1) % sz], H[s], p)) { OUT[e].push_back(p); }
			if (e == sz - 2 && on_seg_weak(H[sz - 2], H[0], p)) { OUT[sz - 1].push_back(p); }
			return 1;
		}
		else if (n == 2) {
			if (on_seg_strong(H[s], H[e], p)) {
				OUT[(s - 1 + sz) % sz].push_back(p);
				OUT[s].push_back(p);
				OUT[e].push_back(p);
				return 0;
			}
			int i0 = (s - 2 + sz) % sz;
			int i1 = (s - 1 + sz) % sz;
			int i2 = s;
			int i3 = e;
			int i4 = (e + 1) % sz;
			int i5 = (e + 2) % sz;
			if (inner_check(H[i0], H[i1], H[i2], H[i3], p)) OUT[i1].push_back(p);
			if (inner_check(H[i1], H[i2], H[i3], H[i4], p)) OUT[i2].push_back(p);
			if (inner_check(H[i2], H[i3], H[i4], H[i5], p)) OUT[i3].push_back(p);
		}
	}
	return 1;
}
Polygon lower_monotone_chain(const Polygon& H, const int& x, const int& n = 1, const bool& in = OUTER) {
	Polygon& C = in ? IN[x] : OUT[x];
	int sz = H.size();
	const Pos& s = H[(x - 1 + sz) % sz], & e = H[(x + n) % sz];
	//C.push_back(s); C.push_back(e);
	//std::sort(C.begin(), C.end(), [&](const Pos& p, const Pos& q) -> bool {
	//	ll fp = dot(s, e, p), fq = dot(s, e, q);
	//	if (fp == fq) {
	//		ll tp = cross(s, e, p), tq = cross(s, e, q);
	//		return tp < tq;
	//	}
	//	return fp < fq;
	//	});
	std::sort(C.begin(), C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(s, p, q);
		if (!ret) return (s - p).Euc() < (s - q).Euc();
		return ret > 0;
		}
	);
	Polygon S;
	sz = C.size();
	if (sz == 2) return C;
	for (int i = 0; i < sz; i++) {
		while (S.size() > 1 && cross(S[S.size() - 2], S.back(), C[i]) <= 0)
			S.pop_back();
		S.push_back(C[i]);
	}
	return S;
}
struct E {
	ld r;
	int i;
	E(ld r_ = 0, int i_ = 0) : r(r_), i(i_) {}
	bool operator < (const E& e) const { return r > e.r; }
};
typedef std::vector<E> VE;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	//freopen("../../../input_data/MakingPerimeter/002.in", "r", stdin);
	//freopen("../../../input_data/MakingPerimeter/out.txt", "w", stdout);
	std::cin >> N;
	assert(N >= 5); assert(N <= LEN);
	Polygon C(N);
	for (int i = 0; i < N; i++) std::cin >> C[i], C[i].i = i;
	//if (N <= LIMIT) { std::cout << brute(C) << "\n"; return; }
	Polygon P = graham_scan(C), I;
	assert(P.size() >= 3);
	ld R = round(P), ret = R;
	VE del;
	memset(V, 0, sizeof V);
	for (const Pos& p : P) V[p.i] = 1;
	for (const Pos& p : C) if (!V[p.i]) I.push_back(p);
	for (const Pos& p : I) inner_check_bi_search(P, p, 1, OUTER);
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		const Pos& p0 = P[(i - 1 + sz) % sz], & p1 = P[i], & p2 = P[(i + 1) % sz];
		ld o1 = (p0 - p1).mag() + (p1 - p2).mag();
		OUT[i].push_back(p0); OUT[i].push_back(p2);
		K = OUT[i].size();
		for (int k = 0; k < K; k++) OUT[i][k].i = k;
		Polygon Q = lower_monotone_chain(P, i, 1, OUTER), J;
		M = Q.size();
		ld r = round(Q, HALF);
		del.push_back(E(o1 - r, p1.i));
		if (M <= 2) continue;
		std::vector<bool> F(K, 0);
		for (int j = 0; j < M; j++) Polygon().swap(IN[j]);
		for (const Pos& q : Q) F[q.i] = 1;
		for (const Pos& q : OUT[i]) if (!F[q.i]) J.push_back(q);
		for (const Pos& q : J) inner_check_bi_search(Q, q, 1, INNER);
		for (int j = 1; j < M - 1; j++) {
			const Pos& q0 = Q[(j - 1 + M) % M], & q1 = Q[j], & q2 = Q[(j + 1) % M];
			ld o2 = (q0 - q1).mag() + (q1 - q2).mag();
			IN[j].push_back(q0); IN[j].push_back(q2);
			Polygon H = lower_monotone_chain(Q, j, 1, INNER);
			ld rr = round(H, HALF);
			ret = std::min(ret, R - o1 + r - o2 + rr);
		}
	}
	if (sz >= 4) {
		std::sort(del.begin(), del.end());
		int t0 = del[0].i, t1 = del[1].i, t2 = del[2].i, t3 = del[3].i;
		for (int i = 0; i < sz; i++) {
			if (P[i].i == t0) {
				int tnxt = P[(i + 1) % sz].i, tpre = P[(i - 1 + sz) % sz].i;
				if (t1 != tnxt && t1 != tpre) ret = std::min(ret, R - del[0].r - del[1].r);
				else if (t2 != tnxt && t2 != tpre) ret = std::min(ret, R - del[0].r - del[2].r);
				else {
					assert(t3 != tnxt && t3 != tpre);
					ret = std::min(ret, R - del[0].r - del[3].r);
					ret = std::min(ret, R - del[1].r - del[2].r);
				}
				break;
			}
		}
	}
	if (sz == 3) {
		for (int i = 0; i < sz; i++) {
			Polygon tmp = I;
			tmp.push_back(P[i]);
			ret = std::min(ret, round(graham_scan(tmp)));
		}
	}
	else {
		for (int i = 0; i < sz; i++) Polygon().swap(OUT[i]);
		for (const Pos& p : I) inner_check_bi_search(P, p, 2, OUTER);
		for (int i = 0; i < sz; i++) {
			const Pos& p0 = P[(i - 1 + sz) % sz], & p1 = P[i], & p2 = P[(i + 1) % sz], & p3 = P[(i + 2) % sz];
			ld o3 = (p0 - p1).mag() + (p1 - p2).mag() + (p2 - p3).mag();
			OUT[i].push_back(p0); OUT[i].push_back(p3);
			Polygon Q = lower_monotone_chain(P, i, 2, OUTER);
			ld rr = round(Q, HALF);
			ret = std::min(ret, R - o3 + rr);
		}
	}
	std::cout << R - ret << "\n";
	return; 
}
int main() { solve(); return 0; }//boj15332 Making Perimeter of the Convex Hull Shortest
