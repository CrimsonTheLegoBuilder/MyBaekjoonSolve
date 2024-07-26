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
typedef std::vector<ld> Vld;
#define right x
#define left y
const ld INF = 1e30;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
bool zero(const ld& x) { return !sign(x); }

int N, T;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
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
};
const Pos O = Pos(0, 0);
const Pos INVAL = Pos(-1, -1);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
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
bool inner_check(Pos p0, Pos p1, Pos p2, const Pos& t) {
	if (ccw(p0, p1, p2) < 0) std::swap(p1, p2);
	return ccw(p0, p1, t) >= 0 && ccw(p1, p2, t) >= 0 && ccw(p2, p0, t) >= 0;
}
Pos inner_check_bi_search(const std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return Pos(-1, -1);
	if (sz == 1) return p == H[0] ? Pos(0, 0) : Pos(-1, -1);
	if (sz == 2) {
		int i1 = -1, i2 = -1;
		if (H[0] == p) i1 = 0;
		if (H[1] == p) i2 = 1;
		return Pos(i1, i2);
	}
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return Pos(-1, -1);
	if (H[0] == p) return Pos(sz - 1, 1);
	if (H[1] == p) return Pos(0, 2 % sz);
	if (H[sz - 1] == p) return Pos(sz - 2, 0);
	if (on_seg_weak(H[0], H[1], p)) return Pos(0, 1);
	if (on_seg_weak(H[0], H[sz - 1], p)) return Pos(sz - 1, 0);
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) >= 0) s = m;
		else e = m;
	}
	//std::cout << "DEBUG:: " << s << " " << e << "\n";
	if (cross(H[s], H[e], p) < 0) return Pos(-1, -1);
	if (H[s] == p) return Pos((s - 1 + sz) % sz, e);
	if (H[e] == p) return Pos(s, (e + 1) % sz);
	if (on_seg_weak(H[s], H[e], p)) return Pos(s, e);
	//std::cout << "DEBUG:: fuck\n";
	return Pos(sz + 1, sz + 1);
}
Pos find_tangent_bi_search(const Polygon& H, const Pos& p) {
	int sz = H.size();
	Pos IN = Pos(sz + 1, sz + 1);
	Pos F = inner_check_bi_search(H, p);
	//std::cout << "inner_bi:: " << F << "\n";
	if (F == IN) return INVAL;
	if (F != INVAL) return F;
	int i1{ 0 }, i2{ 0 };
	int ccw1 = ccw(p, H[0], H[1]), ccwN = ccw(p, H[0], H[sz - 1]);
	if (ccw1 * ccwN >= 0) {
		i1 = 0;
		if (!ccw1 && dot(p, H[1], H[0]) > 0) i1 = 1;
		if (!ccwN && dot(p, H[sz - 1], H[0]) > 0) i1 = sz - 1;
		int s = 0, e = sz - 1, m;
		if (!ccw1) s += 1;
		if (!ccwN) e -= 1;
		bool f = ccw(p, H[s], H[s + 1]) >= 0;
		while (s < e) {
			m = s + e >> 1;
			const Pos& p1 = p, &cur = H[m], &nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = m + 1;
			else e = m;
		}
		i2 = s;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	else {
		//divide hull
		int s = 0, e = sz - 1, k, m;
		bool f = ccw1 > 0 && ccwN < 0;//if H[k] is between H[0] && p
		while (s + 1 < e) {
			k = s + e >> 1;
			int CCW = ccw(H[0], H[k], p);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s = k;
			else e = k;
		}

		//search lower hull
		int s1 = 0, e1 = s;
		while (s1 < e1) {
			m = s1 + e1 >> 1;
			const Pos& p1 = p, &cur = H[m], &nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW > 0) s1 = m + 1;
			else e1 = m;
		}
		i1 = s1;
		if (!ccw(p, H[i1], H[(i1 + 1) % sz]) && dot(p, H[(i1 + 1) % sz], H[i1]) > 0) i1 = (i1 + 1) % sz;

		//search upper hull
		int s2 = e, e2 = sz - 1;
		while (s2 < e2) {
			m = s2 + e2 >> 1;
			const Pos& p1 = p, &cur = H[m], &nxt = H[(m + 1) % sz];
			int CCW = ccw(p1, cur, nxt);
			if (!f) CCW *= -1;//normailze
			if (CCW < 0) s2 = m + 1;
			else e2 = m;
		}
		i2 = s2;
		if (!ccw(p, H[i2], H[(i2 + 1) % sz]) && dot(p, H[(i2 + 1) % sz], H[i2]) > 0) i2 = (i2 + 1) % sz;
	}
	if (ccw(p, H[i1], H[i2]) < 0) std::swap(i1, i2);
	return Pos(i2, i1);
}
struct Pdd {
	ld x, y;
	Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	Pdd operator + (const Pdd& p) const { return { x + p.x, y + p.y }; }
	Pdd operator - (const Pdd& p) const { return { x - p.x, y - p.y }; }
	Pdd operator * (const ld& n) const { return { x * n, y * n }; }
	Pdd operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pdd& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pdd& p) const { return x * p.y - y * p.x; }
	Pdd operator ^ (const Pdd& p) const { return { x * p.x, y * p.y }; }
	Pdd& operator += (const Pdd& p) { x += p.x; y += p.y; return *this; }
	Pdd& operator -= (const Pdd& p) { x -= p.x; y -= p.y; return *this; }
	Pdd& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pdd& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld mag() const { return hypot(x, y); }
};
Pdd P(const Pos& p) { return Pdd(p.x, p.y); }
ld cross(const Pdd& d1, const Pdd& d2, const Pdd& d3) { return (d2 - d1) / (d3 - d2); }
Pdd intersection(const Pdd& p1, const Pdd& p2, const Pdd& q1, const Pdd& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
void query(const Polygon& H) {
	ld dist = INF;
	Pos u, v;
	std::cin >> u >> v;
	if (u == v) { std::cout << "0.0000000\n"; return; }
	Pos tu = find_tangent_bi_search(H, u);
	Pos tv = find_tangent_bi_search(H, v);
	int ur = tu.right, ul = tu.left;
	int vr = tv.right, vl = tv.left;
	//std::cout << "DEBUG:: tangent:: " << ur << " " << ul << " " << vr << " " << vl << " fuck::\n";
	if (!ccw(H[ul], u, H[ur]) &&
		!ccw(H[vl], v, H[vr]) &&
		!collinear(H[ul], H[ur], H[vl], H[vr]) &&
		!ccw(H[ul], H[ur], H[vl], H[vr])) {
		std::cout << "-1\n";
		return;
	}
	if (!ccw(u, v, H[ur]) ||
		!ccw(u, v, H[ul]) ||
		ccw(u, v, H[ur]) == ccw(u, v, H[ul]) ||
		inner_check(u, H[ur], H[ul], v) ||
		inner_check(v, H[vr], H[vl], u)) {
		dist = std::min(dist, (u - v).mag());
	}
	else {
		Pdd U = P(u), V = P(v);
		Pdd UL = P(H[ul]), UR = P(H[ur]);
		Pdd VL = P(H[vl]), VR = P(H[vr]);
		Pdd inx1 = intersection(U, UL, V, VR);
		Pdd inx2 = intersection(U, UR, V, VL);
		dist = std::min({ dist, (U - inx1).mag() + (V - inx1).mag(), (U - inx2).mag() + (V - inx2).mag() });
	}
	std::cout << dist << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(8);
	std::cin >> N;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	std::cin >> T;
	while (T--) query(H);
	return;
}
int main() { solve(); return 0; }//boj32079

/*

6
1 5
-1 1
0 -1
2 1
4 5
3 7
1
3 7 0 -1

*/
