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
int idx_bi_search(const Polygon& H, const ld& x) {
	int s = 1, e = H.size() - 1;
	if (H[1].x > x) return 0;
	if (H[2].x > x) return 1;
	if (!sign(H.back().x - x)) return e;
	while (s < e) {
		int m = s + e + 1 >> 1;//refer to tourist
		if (sign(H[m].x - x) == 0) return m;
		if (sign(H[m].x - x) < 0) s = m;
		else e = m - 1;
	}
	return s;
}
//ld height_search(const ld& x, const ld& w) {
//	auto cal_y = [&](const Polygon& V, const int i, const ld& x_) -> ld {
//		if (zero(V[i].x - x_)) return V[i].y;
//		int nxt = std::min(i + 1, (int)V.size() - 1);
//		ll den = V[nxt].x - V[i].x;
//		ll num = V[nxt].y - V[i].y;
//		return (ld)V[i].y + (x_ - V[i].x) * (ld)num / den;;
//		};
//	int l1, l2, u1, u2;
//	ld x1 = x, x2 = x + w;
//	l1 = idx_bi_search(L, x1);
//	l2 = idx_bi_search(L, x2);
//	u1 = idx_bi_search(U, x1);
//	u2 = idx_bi_search(U, x2);
//	ld y1 = cal_y(L, l1, x1);
//	ld y2 = cal_y(U, u1, x1);
//	ld y3 = cal_y(L, l2, x2);
//	ld y4 = cal_y(U, u2, x2);
//	ld yu = std::min(y2, y4), yl = std::max(y1, y3);
//	X1 = x, X2 = x + w, Y1 = yl, Y2 = yu;
//	return std::max(yu - yl, (ld)0);
//}
//ld area_ternary_search(const ld& w) {
//	ld s = L[0].x, e = L.back().x - w;
//	ld x1, x2;
//	//while (sign(e - s) > 0) {
//	int cnt = 50; while (cnt--) {
//		x1 = (s + s + e) / 3;
//		x2 = (s + e + e) / 3;
//		ld h1 = height_search(x1, w);
//		ld h2 = height_search(x2, w);
//		ld a1 = w * h1, a2 = w * h2;
//		if (sign(a2 - a1) > 0) s = x1;
//		else e = x2;
//	}
//	height_search((s + e) * .5, w);
//	return (X2 - X1) * (Y2 - Y1);
//}
//void area_ternary_search() {
//	ld s = 0, e = L.back().x - L[0].x;
//	ld w1, w2;
//	//while (sign(e - s) > 0) {
//	int cnt = 50; while (cnt--) {
//		w1 = (s + s + e) / 3;
//		w2 = (s + e + e) / 3;
//		ld a1 = area_ternary_search(w1);
//		ld a2 = area_ternary_search(w2);
//		if (sign(a2 - a1) > 0) s = w1;
//		else e = w2;
//	}
//	area_ternary_search((s + e) * .5);
//	return;
//}
void query() {
	int ic, jc;
	std::cin >> ic >> jc;

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
	}
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj30839 29688