#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
typedef double ld;
const ld TOL = 1e-7;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& p, const ld& q) { return zero(p - q); }

int N, L;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
bool eqx(const Pos& p, const Pos& q) { return p.x == q.x; }
std::pair<ld, ld> intersections(const Pos& p, const ld& d) {
	ll y = p.y;
	if (y && sign(y - d) > 0) return { -1, -1 };
	ld dx = sqrtl(d * d - y * y);
	ld x0 = std::max((ld)0, p.x - dx);
	ld x1 = std::min((ld)L, p.x + dx);
	return { x0, x1 };
}
bool F(const Polygon& B, const ld& d) {
	int sz = B.size();
	ld x = 0;
	for (int i = 0; i < sz; i++) {
		auto xx = intersections(B[i], d);
		ld x0 = xx.first, x1 = xx.second;
		if (sign(x - x0) >= 0) x = std::max(x, x1);
	}
	return eq(x, L);
}
ld bi_search(const Polygon& B) {
	int cnt = 40;
	ld s = 0, e = 2e9;
	while (cnt--) {
		ld m = (s + e) * .5;
		if (F(B, m)) e = m;
		else s = m;
	}
	return (s + e) * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> L;
	Polygon B(N);
	for (Pos& p : B) std::cin >> p, p.y = std::abs(p.y);
	std::sort(B.begin(), B.end());
	B.erase(unique(B.begin(), B.end(), eqx), B.end());
	std::cout << bi_search(B) << "\n";
	return;
}
int main() { solve(); return 0; }//boj3346
