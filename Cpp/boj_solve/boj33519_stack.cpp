#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

#define RIGHT 1
#define LEFT -1

int N;
bool V[LEN];
struct Pos {
	int x, y, d;
	Pos(int x_ = 0, int y_ = 0, int d_ = 0) : x(x_), y(y_), d(d_) {}
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon H, S;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ll area(const std::vector<Pos>& H) {
	ll a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += cross(O, H[i], H[(i + 1) % sz]);
	return a;
}
bool norm(std::vector<Pos>& H) {
	ll A = area(H);
	assert(A);
	if (A > 0) { std::reverse(H.begin(), H.end()); return 1; }
	return 0;
}
bool invisible(const Pos& p1, const Pos& p2, const Pos& t) {
	return ccw(O, p1, t) <= 0 && ccw(O, p2, t) >= 0 && ccw(p1, p2, t) >= 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	H.resize(N);
	for (Pos& p : H) std::cin >> p;
#ifndef ASSERT
	ll A = area(H);
	assert(A > 0);
#endif
	S.push_back(H[0]);
	for (int i = 1; i < N - 1; i++) {
		Pos& pre = H[(i - 1 + N) % N], cur = H[i % N], nxt = H[(i + 1) % N];
		while (S.size()) {

		}
	}
	return;
}
int main() { solve(); return 0; }//boj33519 Andrew the Diver