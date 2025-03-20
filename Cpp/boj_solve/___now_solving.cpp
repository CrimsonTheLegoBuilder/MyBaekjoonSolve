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
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

#define LOWER 1
#define UPPER -1
#define LEFT 1
#define RIGHT -1

int N, K, Q;
ld X1, X2, Y1, Y2;
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
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Polygon half_monotone_chain(Polygon& C, int f = LEFT) {
	Polygon H;
	std::sort(C.begin(), C.end()), cmpyx;
	if (f == RIGHT) std::reverse(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		assert(0);
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
	if (f == RIGHT) std::reverse(H.begin(), H.end());
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
int check(const Polygon& r, const Pos& ir, const Polygon& l, const Pos& il, ld& t) {
	int szr = r.size(), szl = l.size();
	if (ir.y == -1 && il.y == -1) {
		
		return 0;
	}
	if (ir.y == -1) {
		
		return 0;
	}
	if (il.y == -1) {
		
		return 0;
	}
	assert(0);
	return 0;
}
ld bi_search(const Polygon& r, const Polygon& l) {
	int szr = r.size(), szl = l.size();
	int yr = r[szr - 1].y, yl = l[szl - 1].y;
	ld t;
	if (yr >= yl) {
		int s = 0, e = szr - 1;
		while (s < e) {
			int m = s + e >> 1;
			Pos il = idx_bi_search(l, m);
			Pos ir = Pos(m, -1);
			int f = check(r, ir, l, il, t);
			if (!f) return t;
			else if (f > 0) e = m;
			else s = m + 1;
		}
	}
}
void query() {
	int ic, jc;
	std::cin >> ic >> jc;
	ic--; jc--;
	ld d1 = bi_search(R[ic], L[jc]);
	ld d2 = bi_search(R[jc], L[ic]);
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
		for (Pos& p : C) std::cin >> p, y = std::min(y, p.y);
		for (Pos& p : C) p.y -= y;
		L[i] = half_monotone_chain(C, LEFT);
		R[i] = half_monotone_chain(C, RIGHT);
		if (L[i].back().y == L[i][L[i].size() - 2].y) L[i].pop_back();
		if (R[i].back().y == R[i][R[i].size() - 2].y) R[i].pop_back();
		std::reverse(R[i].begin(), R[i].end());
	}
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj30839 29688