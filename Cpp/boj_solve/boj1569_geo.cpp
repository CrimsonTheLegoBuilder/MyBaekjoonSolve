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
const int INF = 1e9;

int N;
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
bool on_box_check(const Pos p0, const Pos p1, const Polygon& H) {
	for (const Pos& p : H) {
		if (p.x == p0.x || p.x == p1.x || p.y == p0.y || p.y == p1.y) continue;
		else return 0;
	}
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Polygon H(N);

	int l = INF, r = -INF, d = INF, u = -INF;
	bool f = 0;
	int D;
	for (Pos& p : H) {
		std::cin >> p;
		l = std::min(l, p.x);
		r = std::max(r, p.x);
		d = std::min(d, p.y);
		u = std::max(u, p.y);
	}
	if ((r - l) == (u - d)) {
		Pos p0 = Pos(l, d);
		Pos p1 = Pos(r, u);
		f = on_box_check(p0, p1, H);
		D = r - l;
	}
	else {
		int h = r - l;
		int v = u - d;
		if (h < v) {
			int dx = v - h;
			Pos p0 = Pos(l, d);
			Pos p1 = Pos(r + dx, u);
			Pos p2 = Pos(l - dx, d);
			Pos p3 = Pos(r, u);
			f = on_box_check(p0, p1, H) || on_box_check(p2, p3, H);
			D = v;
		}
		else if (h > v) {
			int dy = h - v;
			Pos p0 = Pos(l, d);
			Pos p1 = Pos(r, u + dy);
			Pos p2 = Pos(l, d - dy);
			Pos p3 = Pos(r, u);
			f = on_box_check(p0, p1, H) || on_box_check(p2, p3, H);
			D = h;
		}
	}
	if (f) std::cout << D << "\n";
	else std::cout << "-1\n";
	return;
}
int main() { solve(); return 0; }