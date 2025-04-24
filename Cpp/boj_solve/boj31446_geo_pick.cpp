#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

//#define DEBUG
#define WHAT_THE_FUCK

//Pick`s Theorem : A = i + b/2 - 1

int N, S2, i2;
struct Pos {
	int x, y;
	Pos(int X = 0,int Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> S2;
	i2 = S2 - N + 2;
	Pos p = Pos(0, 1);
	if (i2 < 0) { std::cout << "NO\n"; return; }
	std::cout << "YES\n";
	if (!i2 && N == 3) {
		std::cout << p << "\n";
		p = Pos(S2, 0);
		std::cout << p << "\n";
		p = Pos(S2, 1);
		std::cout << p << "\n";
		return;
	}
	if (i2 & 1) {
		N++;
		i2 = S2 - N + 2;
		int i = i2 >> 1;
		N--;
		if (N == 3) {
			std::cout << p << "\n";
			p = Pos(i + 1, 0);
			std::cout << p << "\n";
			p = Pos(i + 1, 2);
			std::cout << p << "\n";
			return;
		}
		//else 
		N -= 3;
		std::cout << p << "\n";
		p = Pos(2, 1);
		std::cout << p << "\n";
		p = Pos(i + 3, 0);
		std::cout << p << "\n";
	}
	else {
		int i = i2 >> 1;
		N -= 2;
		std::cout << p << "\n";
		p = Pos(i + 1, 0);
		std::cout << p << "\n";
	}
	while (N--) {
		if (!N) {
			p.x--;
			p.y = 2;
		}
		else {
			if (!p.y) p.y++;
			else p.x++, p.y--;
		}
		std::cout << p << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj31446