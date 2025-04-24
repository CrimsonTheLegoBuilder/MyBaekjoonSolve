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
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;

int N;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N;
	std::cout << std::fixed;
	std::cout.precision(2);
	Polygon H(N);
	Pos T = O;
	for (Pos& p : H) std::cin >> p, T += p;
	std::cout << T << "\n";
	ll d = -1;
	Pos Q;
	for (const Pos& p : H) {
		T -= p;
		if (d == -1 || T.Euc() < d) d = T.Euc(), Q = T;
		T += p;
	}
	std::cout << Q.mag() << "\n";
}
int main() { solve(); return 0; }