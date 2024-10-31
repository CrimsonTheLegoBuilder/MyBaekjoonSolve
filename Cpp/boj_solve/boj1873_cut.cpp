#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
const int LEN = 1e5 + 1;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

int N, cnt;
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} s, e; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = 1) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> s >> e >> N;
	Polygon P(N);
	assert(s != e);
	if (s.x != e.x) s.x = -1, e.x = 1e6 + 1;
	else if (s.y != e.y) s.y = -1, e.y = 1e6 + 1;
	for (Pos& p : P) std::cin >> p;
	for (int i = 0; i < N; i++) {
		const Pos& p0 = P[i], & p1 = P[(i + 1) % N], & p2 = P[(i + 2) % N], & p3 = P[(i + 3) % N];
		if (ccw(s, e, p1) * ccw(s, e, p2) == 1) continue;
		if (ccw(s, e, p1) * ccw(s, e, p2) == -1) cnt++;
		else if (collinear(s, e, p1, p2)) { cnt += ccw(p1, p2, p0) < 0; cnt += ccw(p1, p2, p3) < 0; }
		else if (collinear(s, e, p1, p0)) continue;
		else if (on_seg_weak(s, e, p1)) cnt += inside(p0, p1, p2, s) + inside(p0, p1, p2, e);
	}
	assert(cnt % 2 == 0);
	std::cout << (cnt >> 1) + 1 << "\n";
}
int main() { solve(); return 0; }//boj1873
