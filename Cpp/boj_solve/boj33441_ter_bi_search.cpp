#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>
typedef long long ll;
const ll INF = 1e18;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define LO 1
#define HI -1

int N, T, Q;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Polygon half_monotone_chane(Polygon& C, const int& d = LO) {
	if (d == LO) std::sort(C.begin(), C.end());
	if (d == HI) std::reverse(C.begin(), C.end());
	Polygon H;
	for (const Pos& p : C) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H.back(), p) <= 0) H.pop_back();
		H.push_back(p);
	}
	return H;
}
Pos ternary_search(const Polygon& H, Pos v, const int& d) {
	if (v.x * d < 0) v *= d;
	int s = 0, e = H.size() - 1;
	while (e - s > 2) {
		int m1 = (s + s + e) / 3;
		int m2 = (s + e + e) / 3;
		ll tq1 = v / H[m1];
		ll tq2 = v / H[m2];
		if (tq1 > tq2) s = m1;
		else e = m2;
	}
	ll tq = INF; Pos p = H[s];
	for (int i = s; i <= e; i++) if (v / H[i] < tq) tq = v / H[i], p = H[i];
	return p;
}
int query(const Polygon& L, const Polygon& U) {
	int q;
	Pos p1, p2;
	std::cin >> q >> p1;
	if (q == 1) {
		return 1;
	}
	else if (q == 2) {
		std::cin >> p2;
		return 0;
	}
	else { assert(0); }
	return -1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/.txt", "r", stdin);
	//freopen("../../../input_data/.txt", "w", stdout);
	std::cin >> N >> Q;
	Polygon C(N); for (Pos& p : C) std::cin >> p;
	Polygon L = half_monotone_chane(C, LO);
	Polygon U = half_monotone_chane(C, HI);
	while (Q--) query(L, U);
	return;
}
int main() { solve(); return 0; }//boj33441