#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define LOWER 1
#define UPPER -1
#define LEFT 0
#define RIGHT 1

int N, K, Q;
int X[LEN][2];
ll W[LEN];
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon L[LEN], R[LEN];
bool cmpyx(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
Polygon half_monotone_chain(Polygon& C, int f = RIGHT) {
	Polygon H;
	std::sort(C.begin(), C.end()), cmpyx;
	if (f == LEFT) std::reverse(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		//assert(0);
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	int sz = H.size();
	if (H[sz - 2].x == H[sz - 1].x) H.pop_back();
	//if (f == RIGHT) std::reverse(H.begin(), H.end());
	return H;
}
Pos idx_bi_search(const Polygon& H, const int& y) {
	int s = 0, e = H.size() - 1;
	assert(y >= 0);
	if (y == 0) return Pos(0, -1);
	if (y < H[1].y) return Pos(0, 1);
	while (s < e) {
		int m = s + e >> 1;
		if (H[m].y == y) return Pos(m, -1);
		if (H[m].y < y) s = m + 1;
		else e = m;
	}
	return Pos(s - 1, s);
}
ld get_x(const Pos& p, const Pos& q, const ld& y) {
	assert(p.y <= y && y <= q.y);
	int dy = q.y - p.y;
	int dx = q.x - p.x;
	ld x = p.x + (y * (1. * dx / dy));
	return x;
}
int check(const int& ri, const Polygon& r, const Pos& ir, const int& li, const Polygon& l, const Pos& il, ld& t) {
	int szr = r.size(), szl = l.size();
	if (ir.y == -1 && il.y == -1) {
		ll xr = W[ri] - (X[ri][RIGHT] - r[ir.x].x);
		ll xl = W[li] - (l[il.x].x - X[li][LEFT]);
		t = xr + xl;
		if (ir.x == 0) {
			assert(il.x == 0);
			return ccw(r[0], r[1], l[0], l[1]);
		}
		if (ir.x == szr - 1) {
			assert(il.x > 0);
			return ccw(r[ir.x - 1], r[ir.x], l[il.x - 1], l[il.x]) * -1;
		}
		if (il.x == szl - 1) {
			assert(ir.x > 0);
			return ccw(r[ir.x - 1], r[ir.x], l[il.x - 1], l[il.x]) * -1;
		}
		Pos r0 = r[ir.x - 1], r1 = r[ir.x], r2 = r[ir.x + 1];
		Pos l0 = l[il.x - 1], l1 = l[il.x], l2 = l[il.x + 1];
		assert(r1.y == l1.y);
		assert(ccw(r0, r1, r2) > 0);
		assert(ccw(l0, l1, l2) < 0);
		Pos v = l1 - r1;
		l0 += v; l1 += v; l2 += v;
		if (ccw(r1, r2, l1, l2) == 0 || ccw(r0, r1, l0, l1) < 0) return 0;
		if (ccw(r1, r2, l1, l2) > 0) return 1;
		if (ccw(r0, r1, l0, l1) < 0) return -1;
		return 0;
	}
	if (ir.y == -1) {
		assert(ir.x > 0);
		Pos l0 = l[il.x], l1 = l[il.y];
		ll xr = W[ri] - (X[ri][RIGHT] - r[ir.x].x);
		ld x_ = get_x(l0, l1, r[ir.x].y);
		ld xl = W[li] - (x_ - X[li][LEFT]);
		t = xr + xl;
		if (ir.x == szr - 1) {
			return ccw(r[ir.x - 1], r[ir.x], l0, l1) * -1;
		}
		Pos r0 = r[ir.x - 1], r1 = r[ir.x], r2 = r[ir.x + 1];
		if (ccw(l0, l1, r1, r2) >= 0 && ccw(l0, l1, r1, r0) >= 0) return 0;
		if (ccw(l0, l1, r1, r2) < 0) return 1;
		if (ccw(l0, l1, r1, r0) < 0) return -1;
		assert(0);
		return 0;
	}
	if (il.y == -1) {
		assert(il.x > 0);
		Pos r0 = r[ir.x], r1 = r[ir.y];
		ll xl = W[li] - (l[il.x].x - X[li][LEFT]);
		ld x_ = get_x(r0, r1, l[il.x].y);
		ld xr = W[ri] - (X[ri][RIGHT] - x_);
		t = xr + xl;
		if (il.x == szl - 1) {
			return ccw(r0, r1, l[il.x - 1], l[il.x]) * -1;
		}
		Pos l0 = l[il.x - 1], l1 = l[il.x], l2 = l[il.x + 1];
		if (ccw(r0, r1, l1, l2) <= 0 && ccw(r0, r1, l1, l0) <= 0) return 0;
		if (ccw(r0, r1, l1, l2) > 0) return 1;
		if (ccw(r0, r1, l1, l0) > 0) return -1;
		assert(0);
		return 0;
	}
	assert(0);
	return 0;
}
ld bi_search(const int& ri, const Polygon& r, const int& li, const Polygon& l) {
	int szr = r.size(), szl = l.size();
	int yr = r[szr - 1].y, yl = l[szl - 1].y;
	ld t = -1;
	if (yr >= yl) {
		int s = 0, e = szr - 1;
		while (s < e) {
			int m = s + e >> 1;
			Pos il = idx_bi_search(l, m);
			Pos ir = Pos(m, -1);
			int f = check(ri, r, ir, li, l, il, t);
			if (!f) return t;
			else if (f > 0) e = m;
			else s = m + 1;
		}
	}
	else {
		int s = 0, e = szl - 1;
		while (s < e) {
			int m = s + e >> 1;
			Pos ir = idx_bi_search(r, m);
			Pos il = Pos(m, -1);
			int f = check(ri, r, ir, li, l, il, t);
			if (!f) return t;
			else if (f > 0) s = m + 1;
			else e = m;
		}
	}
	std::cout << "err:: " << t << " ::what the fuck!!!\n";
	//assert(0);
	return -1;
}
void query() {
	int ic, jc;
	std::cin >> ic >> jc;
	ic--; jc--;
	ld d1 = bi_search(ic, R[ic], jc, L[jc]);
	ld d2 = bi_search(jc, R[jc], ic, L[ic]);
	std::cout << std::min(d1, d2) << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> K;
		Polygon C(K);
		int y = INF;
		int x1 = INF, x2 = -INF;
		for (Pos& p : C) std::cin >> p, y = std::min(y, p.y);
		for (Pos& p : C) p.y -= y, x1 = std::min(x1, p.x), x2 = std::max(x2, p.x);
		X[i][LEFT] = x1; X[i][RIGHT] = x2;
		W[i] = x2 - x1;
		R[i] = half_monotone_chain(C, RIGHT);
		L[i] = half_monotone_chain(C, LEFT);
		if (R[i].back().y == R[i][R[i].size() - 2].y) R[i].pop_back();
		if (L[i].back().y == L[i][L[i].size() - 2].y) L[i].pop_back();
		std::reverse(L[i].begin(), L[i].end());
	}
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj30839 29688