//#include <iostream>
//typedef long long ll;
//ll C;
//
//struct Pos { ll x, y; }p1, p2, p3;
//struct Vec { ll vx, vy; }v1, v2;
//
//Vec V(const Pos& p1, const Pos& p2) { return { p2.x - p1.x, p2.y - p1.y }; }
//ll cross(const Vec& v1, const Vec& v2) { return v1.vx * v2.vy - v2.vx * v1.vy; }
//
//int main() {
//	std::cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y;
//	v1 = V(p1, p2); v2 = V(p2, p3);
//	C = cross(v1, v2);
//	std::cout << (C ? 2 * (C > 0) - 1 : 0) << "\n";
//	return 0;
//}

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
inline int sign(const ll& x) { return x < 0 ? -1 : x > 0; }

int N;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	Pos p1, p2, p3;
	std::cin >> p1 >> p2 >> p3;
	std::cout << ccw(p1, p2, p3) << "\n";
	return 0;
}