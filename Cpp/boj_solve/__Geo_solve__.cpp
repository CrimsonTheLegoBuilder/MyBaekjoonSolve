#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
inline int sign(const ll& x) { return x < 0 ? -1 : x > 0; }
inline ll sq(const int& x) { return (ll)x * x; }
inline ll sq(const ll& x) { return x * x; }

int d, T, N;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
Polygon dir = { Pos(-1, -1), Pos(1, 0), Pos(-1, 0), Pos(0, -1), Pos(0, 1) };
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Polygon H = { Pos(0, 0) };
	for (int _ = 0; _ < 6; _++) {
		std::cin >> d >> T;
		Pos cur = H.back() + dir[d] * T;
		H.push_back(cur);
	}
	ll ret = 0;
	for (int i = 0; i < 7; i++) ret += H[i] / H[(i + 1) % 7];
	std::cout << (ret * N >> 1)<< "\n";
	return 0;
}