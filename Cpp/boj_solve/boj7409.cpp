#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ld> Vld;
const ld TOL = 1e-6;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ld sq(const ld& x) { return x * x; }

Vld B, H;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	Pos operator - () const { return { -x, -y }; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
};
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	//freopen("bricks.in", "r", stdin);
	//freopen("bricks.out", "w", stdout);
	B.resize(3); for (ld& x : B) std::cin >> x;
	H.resize(2); for (ld& x : H) std::cin >> x;
	std::sort(B.begin(), B.end());
	std::sort(H.begin(), H.end());
	if (B[1] <= H[1] && B[0] <= H[0]) std::cout << "YES\n";
	else if (B[0] > H[0]) std::cout << "NO\n";
	else if (B[1] > H[1]) {
		ld h = H[0], w = H[1];
		ld d = sqrt(sq(B[0]) + sq(B[1]));
		ld w_ = sqrt(sq(d) - sq(h));
		ld h_ = sqrt(sq(d) - sq(w));
		Pos p1 = Pos(w_, h);
		ld t = atan2(B[1], B[0]);
		Pos v = (-p1).rot(t).unit() * B[0] * 2;
		Pos p2 = p1 + v;
		if (w_ > w || h_ > h || p2.x > w) std::cout << "NO\n";
		else std::cout << "YES\n";
	}
	else std::cout << "NO\n";
	return;
}
int main() { solve(); return 0; }//boj7409
