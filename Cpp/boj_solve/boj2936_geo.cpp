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

int N;
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
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos unit() const { return *this / mag(); }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	inline ld rad() const { return atan2(y, x); }
	inline int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	inline friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = dot(d1, d3, d2); return !ccw(d1, d2, d3) && sign(ret) >= 0; }
inline ld area(const Polygon& H) {
	Pos pivot = Pos(0, 0);
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	Pos p, m = Pos(125, 125), r = Pos(250, 0), l = Pos(0, 250);
	std::cin >> p;
	if (p == m) { std::cout << "0.00 0.00\n"; return; }
	if (p == O) { std::cout << "125.00 125.00\n"; return; }
	if (p == r) { std::cout << "0.00 125.00\n"; return; }
	if (p == l) { std::cout << "125.00 0.00\n"; return; }
	Pos ans;
	ld A = 125 * 250;
	if (on_seg_strong(O, r, p)) {
		if (p.x >= 125) {
			ld y = A / p.x;
			std::cout << "0.00 " << y << "\n";
			return;
		}
		ld w = (r - p).mag();
		ld h = A / w;
		ans = r - Pos(h, 0) + Pos(0, h);
	}
	if (on_seg_strong(O, l, p)) {
		if (p.y >= 125) {
			ld x = A / p.y;
			std::cout << x << " 0.00\n";
			return;
		}
		ld w = (l - p).mag();
		ld h = A / w;
		ans = l - Pos(0, h) + Pos(h, 0);
	}
	if (on_seg_strong(m, r, p)) {
		ld w = (l - p).mag();
		ld h = A / w;
		ans = l - Pos(0, h * sqrt(2));
	}
	if (on_seg_strong(m, l, p)) {
		ld w = (r - p).mag();
		ld h = A / w;
		ans = r - Pos(h * sqrt(2), 0);
	}
	//std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj2936