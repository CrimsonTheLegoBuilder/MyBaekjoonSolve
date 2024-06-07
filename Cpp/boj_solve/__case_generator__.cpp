#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
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
ll cross(Pos a, Pos b, Pos c) { return (b - a) / (c - b); }
std::vector<Pos> C, H;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("boj11002_tc.txt", "w", stdout);
	int N = 10;
	std::mt19937 E(std::time(0));

	int x = 1, y = 0;
	C.push_back(Pos(x, 0));
	for (int i = 0; i < N; i++) {
		x += E() % 30 + 1;
		y = E() % 30;
		C.push_back(Pos(x, y));
	}
	C.push_back(Pos(x += E() % 30 + 1, y));

	int M = 10;
	int cnt = 0;
	while (1) {
		int xx = E() % x;
		y = E() % 35;
		Pos p = Pos(xx, y);
		bool f = 0;
		for (int i = 0; i < 11; i++) {
			if (C[i].x <= p.x && p.x <= C[i + 1].x) {
				if (cross(C[i], C[i + 1], p) < 0) {
					f = 1; break;
				}
			}
		}
		if (!f) {
			H.push_back(p);
			cnt++;
		}
		if (cnt == M) break;
	}
	std::cout << N + 2 << "\n";
	for (Pos& p : C) std::cout << p << "\n";
	std::cout << M << "\n";
	for (Pos& p : H) std::cout << p << "\n";
	return 0;
}

/*



*/