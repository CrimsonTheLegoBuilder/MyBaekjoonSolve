#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll nC2(const ll& n) { return (n - 1) * n >> 1; }

#define LOW 1
#define UPPER -1

int N;
ld A, x1, x2, y1, y2;
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
Polygon U, L;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
std::vector<Pos> monotone_chain(std::vector<Pos>& C, int f = LOW) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && f * ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
int idx_bi_search(const Polygon& H, const ld& x) {
	int s = 1, e = H.size() - 1;
	if (sign(H[1].x - x) >= 0) return 0;
	if (sign(H[2].x - x) >= 0) return 1;
	int cnt = 70;
	while (cnt--) {
		int m = s + e >> 1;
		if (sign(H[m].x - x) >= 0) s = m;
		else e = m;
	}
	return s;
}
ld height_bi_search(const ld& x, const ld& w) {
	int l1, l2, u1, u2;
	l1 = idx_bi_search(L, x);
	l2 = idx_bi_search(L, x + w);
	u1 = idx_bi_search(U, x);
	u2 = idx_bi_search(U, x + w);
	return 0;
}
ld area_ternary_search(const ld& w) {
	ld s = L[0].x, e = L.back().x - w;
	ld x1, x2, h;
	int cnt = 100;
	while (cnt--) {
		x1 = (s + s + e) / 3;
		x2 = (s + e + e) / 3;
		ld h1 = height_bi_search(x1, w);
		ld h2 = height_bi_search(x2, w);
		ld a1 = w * h1, a2 = w * h2;
		if (sign(a2 - a1) >= 0) h = h2, s = x1;
		else h = h1, e = x2;
	}
	return s * h;
}
void area_ternary_search() {
	ld s = 0, e = L.back().x - L[0].x;
	ld w1, w2;
	int cnt = 100;
	while (cnt--) {
		w1 = (s + s + e) / 3;
		w2 = (s + e + e) / 3;
		ld a1 = area_ternary_search(w1);
		ld a2 = area_ternary_search(w2);
		if (sign(a2 - a1) >= 0) s = w1;
		else e = w2;
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Polygon H(N);
	L = monotone_chain(H, LOW);
	U = monotone_chain(H, UPPER);
	area_ternary_search();
	std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
	return;
}
int main() { solve(); return 0; }//boj9583
