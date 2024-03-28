#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
typedef long long ll;
typedef double ld;
ll LEN = 1000000000;
int K, M;

struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	//bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	//bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
std::vector<Pos> C, H;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("boj25002_tc", "w", stdout);
	//std::cout << 250000 << "\n";
	//for (int i = 1; i <= 250000; i++) std::cout << i << " " << i << " " << i << "\n";
	//for (int i = 1; i <= 250000; i++) {
	//	std::cout << i << " " << i << " ";
	//	int x = (i < 200000 ? i < 150000 ? i < 100000 ? i < 50000 ? 5 : 3 : 1 : 2 : 4);
	//	std::cout << x << "\n";
	//}

	C = { Pos(0, 0), Pos(10, 10), Pos(10, 0), Pos(0, 10), Pos(15, 5), };
	Pos Dir[10] = { Pos(1000, 1000), Pos(2000, 0), Pos(1000, 1000), Pos(1000, -1000), Pos(0, -2000), Pos(-1000, -1000),
	Pos(-2000, 0), Pos(-1000, -1000), Pos(50, 100), Pos(-50, -100) };
	for (int i = 0; i < 5; i++) {
		for (int j = 1; j < 5000; j++) {
			for (int k = 0; k < 10; k++) {
				H.push_back(C[i] + Dir[k] * j);
			}
		}
	}
	shuffle(H.begin(), H.end(), std::mt19937(0x14003));
	std::cout << H.size() + 2 << "\n";
	std::cout << "-1000000000 1000000000 1\n1000000000 -1000000000 250000\n";
	int sz = H.size();
	for (int i = 0; i <= sz; i++) {
		std::cout << H[i] << " ";
		int x = (i < sz * 4 / 5 ? i < sz * 3 / 5 ? i < sz * 2 / 5 ? i < sz * 1 / 5 ? 1 : 125000 : 75000 : 175000 : 250000);
		std::cout << x << "\n";
	}
	std::cout << ((LEN * 2) * (LEN * 2)) * 2 << "\n";
	return 0;
}

/*

124999000002

*/