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
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }
ll sq(int x) { return (ll)x * x; }

int N, M;
ll V[LEN];
struct Pos {
	int x, y;
	int q;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) { q = 0; }
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
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, const bool f = 1) {
	//f : include end of seg, !f : ignore end of seg
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (!f) return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ll area(const std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		const Pos& pre = H[(i - 1 + sz) % sz], & cur = H[i], & nxt = H[(i + 1) % sz];
		assert(ccw(pre, cur, nxt) == 1);
		ret += H[i] / H[(i + 1) % sz];
	}
	assert(ret > 0);
	return ret;
}
bool inner_check(const Pos& d0, const Pos& d1, const Pos& d2, const Pos& t) {
	if (on_seg_weak(d1, d2, t)) return 1;
	return ccw(d0, d1, t) > 0 && ccw(d1, d2, t) > 0 && ccw(d2, d0, t) > 0;
}
int inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	//if (!sz) return -1;
	//if (sz == 1) return p == H[0] ? 0 : -1;
	//if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	assert(sz >= 3);
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	if (inner_check(H[0], H[1], H[sz - 1], p)) { V[0] += p.q; }
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) < 0) return -1;
	if (on_seg_strong(H[s], H[e], p)) return 0;
	if (s == 2 && on_seg_weak(H[2], H[0], p)) { V[(s - 1 + sz) % sz] += p.q; }
	if (inner_check(H[s], H[e], H[(s - 1 + sz) % sz], p)) { V[s] += p.q; }
	if (inner_check(H[e], H[(e + 1) % sz], H[s], p)) { V[e] += p.q; }
	if (e == sz - 2 && on_seg_weak(H[sz - 2], H[0], p)) { V[(e + 1) % sz] += p.q; }
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	Polygon H(N), C(M);
	for (Pos& p : H) std::cin >> p;
	for (Pos& p : C) std::cin >> p >> p.q;
	area(H);
	memset(V, 0, sizeof V);
	ll all = 0;
	for (Pos& p : C) if (inner_check_bi_search(H, p) == 1) all += p.q;
	//for (Pos& p : C) std::cout << "in:: " << inner_check_bi_search(H, p) << "\n";

	//ll ret = -1;
	ll ret = std::abs(all);

	if (N == 3) { std::cout << ret << "\n"; return; }//no answer
	if (N == 4) {//brute
		for (int i = 0; i < 4; i++) {
			ll tmp = 0;
			Polygon tri = { H[i], H[(i + 1) % 4], H[(i + 2) % 4] };
			for (const Pos& p : C) if (inner_check_bi_search(tri, p) == 1) tmp += p.q;
			ret = std::max(std::abs(tmp), ret);
		}
		std::cout << ret << "\n";
		return;
	}

	for (int i = 0; i < N; i++) ret = std::max(std::abs(all - V[i]), ret);
	assert(ret >= 0);
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj31808 gauss's law

/*

4 4
-10 10
-10 -10
10 -10
10 10

5 5 1
5 -5 3
5 0 3
6 0 3

*/