#include <iostream>
typedef long long ll;
ll C;

struct Pos { ll x, y; }p1, p2, p3;
struct Vec { ll vx, vy; }v1, v2;

Vec V(const Pos& p1, const Pos& p2) { return { p2.x - p1.x, p2.y - p1.y }; }
ll cross(const Vec& v1, const Vec& v2) { return v1.vx * v2.vy - v2.vx * v1.vy; }

int main() {
	std::cin >> p1.x >> p1.y >> p2.x >> p2.y >> p3.x >> p3.y;
	v1 = V(p1, p2); v2 = V(p2, p3);
	C = cross(v1, v2);
	std::cout << (C ? 2 * (C > 0) - 1 : 0) << "\n";
	return 0;
}