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
const ld INF = 1e21;
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
#define _1st x
#define _2nd y

//#define DEBUG
#ifdef DEBUG
ld D[LEN * 5];
#endif

int N, K, Q;
int X[LEN][2];
ll W[LEN];
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon L[LEN], R[LEN];
bool cmpyx(const Pos& p, const Pos& q) { return p.y == q.y ? p.x > q.x : p.y < q.y; }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
Polygon half_monotone_chain(Polygon& C, int f = RIGHT) {
	Polygon H;
	if (f == RIGHT) std::sort(C.begin(), C.end(), cmpyx);
	if (f == LEFT) std::reverse(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		//assert(0);
		for (const Pos& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	int sz = H.size();
	if (H[sz - 2].y == H[sz - 1].y) H.pop_back();
	if (f == LEFT) std::reverse(H.begin(), H.end());
	return H;
}
Pos idx_bi_search(const Polygon& H, const int& y) {
	int s = 0, e = H.size() - 1;
	assert(y >= 0);
	assert(H.size() > 1);
	//std::cout << "y:: " << H.back().y << " y:: " << y << "\n";
	if (H.back().y < y) return Pos(-1, -1);
	//std::cout << "y:: " << H.back().y << " y:: " << y << "\n";
	if (y == 0) return Pos(0, -1);
	if (y < H[1].y) return Pos(0, 1);
	if (H.back().y == y) return Pos(e, -1);
	while (s < e) {
		int m = s + e >> 1;
		if (H[m].y == y) return Pos(m, -1);
		if (H[m].y < y) s = m + 1;
		else e = m;
	}
	assert(s > 0);
	assert(H[s - 1].y < y);
	assert(y < H[s].y);
	return Pos(s - 1, s);
}
ld get_x(const Pos& p, const Pos& q, const int& y) {
	//std::cout << "p::" << p << "\n";
	//std::cout << "q::" << q << "\n";
	//std::cout << "y::" << y << "\n";
	assert(p.y <= y && y <= q.y);
	int dy = q.y - p.y;
	int dx = q.x - p.x;
	ld x = p.x + ((y - p.y) * (1. * dx / dy));
	return x;
}
int check(const int& ri, const Polygon& r, const Pos& ir, const int& li, const Polygon& l, const Pos& il, ld& t) {
	assert(ir._2nd == -1 || il._2nd == -1);
	if (ir._1st == -1 || il._1st == -1) return -1;
	//std::cout << "check:: r[ir.ist]:: " << r[ir._1st] << " ";
	//std::cout << "check:: l[il.ist]:: " << l[il._1st] << "\n";
	int szr = r.size(), szl = l.size();
	if (ir._2nd == -1 && il._2nd == -1) {
		//ll wr = std::max(W[ri] - (X[ri][RIGHT] - r[ir._1st].x), ll(l[il._1st].x - X[li][LEFT]));
		//ll wl = std::max(W[li] - (l[il._1st].x - X[li][LEFT]), ll(X[ri][RIGHT] - r[ir._1st].x));
		ld dr = X[ri][RIGHT] - r[ir._1st].x;
		ld dl = l[il._1st].x - X[li][LEFT];
		ld wr = std::max(W[ri] - dr, dl);
		ld wl = std::max(W[li] - dl, dr);
		t = wr + wl;
		if (ir._1st == 0) {
			assert(il._1st == 0);
			return ccw(r[0], r[1], l[0], l[1]) > 0;
		}
		if (ir._1st == szr - 1 || il._1st == szl - 1) {
			assert(ir._1st > 0 && il._1st > 0);
			return ccw(r[ir._1st - 1], r[ir._1st], l[il._1st - 1], l[il._1st]) >= 0 ? 0 : -1;
		}
		const Pos& r0 = r[ir._1st - 1], & r1 = r[ir._1st], & r2 = r[ir._1st + 1];
		const Pos& l0 = l[il._1st - 1], & l1 = l[il._1st], & l2 = l[il._1st + 1];
		assert(r1.y == l1.y);
		assert(ccw(r0, r1, r2) > 0);
		assert(ccw(l0, l1, l2) < 0);
		//Pos v = l1 - r1;
		//l0 += v; l1 += v; l2 += v;
		if (ccw(r1, r2, l1, l2) <= 0 && ccw(r0, r1, l0, l1) >= 0) return 0;
		if (ccw(r1, r2, l1, l2) > 0) return 1;
		if (ccw(r0, r1, l0, l1) < 0) return -1;
		//std::cout << "fuck:: y == y\n";
		assert(0 && 1);
		return 0;
	}
	if (ir._2nd == -1) {
		assert(ir._1st > 0);
		const Pos& l0 = l[il._1st], & l1 = l[il._2nd];
		ld dr = X[ri][RIGHT] - r[ir._1st].x;
		ld x_ = get_x(l0, l1, r[ir._1st].y);
		ld dl = x_ - X[li][LEFT];
		//ll wr = W[ri] - (X[ri][RIGHT] - r[ir._1st].x);
		ld wr = std::max(W[ri] - dr, dl);
		//ld wl = W[li] - (x_ - X[li][LEFT]);
		ld wl = std::max(W[li] - dl, dr);
		t = wr + wl;
		if (ir._1st == szr - 1) {
			return ccw(r[ir._1st - 1], r[ir._1st], l0, l1) >= 0 ? 0 : -1;
		}
		const Pos& r0 = r[ir._1st - 1], & r1 = r[ir._1st], & r2 = r[ir._1st + 1];
		if (ccw(l0, l1, r1, r2) >= 0 && ccw(l0, l1, r1, r0) >= 0) return 0;
		if (ccw(l0, l1, r1, r2) < 0) return 1;
		if (ccw(l0, l1, r1, r0) < 0) return -1;
		//std::cout << "fuck:: r.y\n";
		assert(0 && 2);
		return 0;
	}
	if (il._2nd == -1) {
		assert(il.x > 0);
		const Pos& r0 = r[ir._1st], & r1 = r[ir._2nd];
		ld dl = l[il._1st].x - X[li][LEFT];
		ld x_ = get_x(r0, r1, l[il._1st].y);
		ld dr = X[ri][RIGHT] - x_;
		//ll wl = W[li] - (l[il._1st].x - X[li][LEFT]);
		ld wl = std::max(W[li] - dl, dr);
		//ld wr = W[ri] - (X[ri][RIGHT] - x_);
		ld wr = std::max(W[ri] - dr, dl);
		t = wr + wl;
		if (il._1st == szl - 1) {
			return ccw(r0, r1, l[il._1st - 1], l[il._1st]) >= 0 ? 0 : -1;
		}
		const Pos& l0 = l[il._1st - 1], & l1 = l[il._1st], & l2 = l[il._1st + 1];
		if (ccw(r0, r1, l1, l2) <= 0 && ccw(r0, r1, l1, l0) <= 0) return 0;
		if (ccw(r0, r1, l1, l2) > 0) return 1;
		if (ccw(r0, r1, l1, l0) > 0) return -1;
		//std::cout << "fuck:: l.y\n";
		assert(0 && 3);
		return 0;
	}
	assert(0 && 4);
	return 0;
}
ld bi_search(const int& ri, const int& li) {
	const Polygon& r = R[ri], & l = L[li];
	ld t = 0;
	int s = 0, e = r.size() - 1;
	while (s <= e) {
		int m = s + e >> 1;
		Pos il = idx_bi_search(l, r[m].y);
		Pos ir = Pos(m, -1);
		int f = check(ri, r, ir, li, l, il, t);
		if (!f) return t;
		else if (f > 0) s = m + 1;
		else e = m - 1;
	}
	s = 0, e = l.size() - 1;
	while (s <= e) {
		int m = s + e >> 1;
		//std::cout << "m:: " << m << "\n";
		Pos ir = idx_bi_search(r, l[m].y);
		Pos il = Pos(m, -1);
		int f = check(ri, r, ir, li, l, il, t);
		//std::cout << "f:: " << f << "\n";
		if (!f) return t;
		else if (f > 0) s = m + 1;
		else e = m - 1;
	}
	return INF;
}
void query(const int& q = -1) {
	int ic, jc; std::cin >> ic >> jc; ic--; jc--;
	ld d1 = bi_search(ic, jc);
	ld d2 = bi_search(jc, ic);
#ifdef DEBUG
	D[q] = std::min(d1, d2);
#else
	std::cout << std::min(d1, d2) << "\n";
#endif
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);
#ifdef DEBUG
	const std::string dir = "../../../input_data/igor_and_toys/tests/";
	const std::string tn = "01";
	freopen((dir + tn).c_str(), "r", stdin);
	freopen("../../../input_data/igor_and_toys/tests/_igor_out.txt", "w", stdout);
#endif
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> K;
		Polygon C(K);
		int y = 1e9;
		int x1 = 1e9, x2 = -1e9;
		for (Pos& p : C) std::cin >> p, y = std::min(y, p.y);
		for (Pos& p : C) p.y -= y, x1 = std::min(x1, p.x), x2 = std::max(x2, p.x);
		X[i][LEFT] = x1; X[i][RIGHT] = x2;
		W[i] = x2 - x1;
		R[i] = half_monotone_chain(C, RIGHT);
		L[i] = half_monotone_chain(C, LEFT);
		assert(R[i].size() > 1);
		assert(L[i].size() > 1);
		//std::cout << "R[" << i << "] = \n";
		//for (Pos& p : R[i]) std::cout << p << "\n";
		//std::cout << "L[" << i << "] = \n";
		//for (Pos& p : L[i]) std::cout << p << "\n";
		//std::cout << "fuck::\n";
	}
	std::cin >> Q;
#ifndef DEBUG
	while (Q--) query();
#else
	for (int q = 0; q < Q; q++) query(q);
	freopen((dir + tn + ".a").c_str(), "r", stdin);
	bool f = 1;
	for (int q = 0; q < Q; q++) {
		ld a;
		std::cin >> a;
		ld w = D[q];
		ld err = std::abs(a - w) / a;
		std::cout << a << " " << w << " " << err << "\n";
		if (err >= 1e10) f = 0, std::cout << a << " " << w << " " << err << "\n";
	}
	if (f) std::cout << tn << " good\n";
#endif
	return;
}
int main() { solve(); return 0; }//boj30839
