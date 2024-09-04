#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline ll nC2(const ll& n) { return (n - 1) * n >> 1; }

#define LOW 1
#define UPPER -1

int N, cnt;
ld A, x1, x2, y1, y2;
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
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
Polygon U, L;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
std::vector<Pos> monotone_chain(std::vector<Pos>& C, int f = LOW) {
	std::vector<Pos> H;
	std::sort(C.begin(), C.end());
	//C.erase(unique(C.begin(), C.end()), C.end());
	if (C.size() <= 2) {
		for (const Pos& pos : C) H.push_back(pos);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && f * ccw(H[H.size() - 2], H[H.size() - 1], C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
void area_ternary_search() {

}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Polygon H(N);
	L = monotone_chain(H, LOW);
	U = monotone_chain(H, UPPER);
	area_ternary_search();
	std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";
	return;
}
int main() { solve(); return 0; }//boj9583
