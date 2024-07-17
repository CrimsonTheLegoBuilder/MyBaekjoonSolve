#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 2e3 + 5;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

int N, T;
struct Pos {
	ld x, y;
	inline Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	inline bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	inline bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	inline Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	inline ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	inline Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	inline Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	inline Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	inline Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	//freopen("stone.in", "r", stdin);
	//freopen("stone_ret.out", "w", stdout);
	std::cin >> T;
	while (T--) {
		std::cin >> N;
		Pos cen = Pos(0, 0);
		Polygon H(N);
		for (Pos& p : H) std::cin >> p;
		if (N == 1) { std::cout << H[0] << "\n"; continue; }
		if (N == 2) { std::cout << (H[0] + H[1]) * .5 << "\n"; continue; }
		ld A = 0;
		for (int i = 0; i < N; i++) {
			ld a = H[i] / H[(i + 1) % N];
			cen += (H[i] + H[(i + 1) % N]) * a;
			A += a;
		}
		A *= .5;
		cen /= 6;
		if (!zero(A)) cen /= A;
		//std::cout << cen << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj3484