#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 30000;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
const ld PI = acos(-1);
int N, M, T, Q;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }

#define UP 0
#define DOWN 1

#define V 0
#define L 1
#define R 2

bool B[LEN][LEN];
int sx, ex, sy, ey;
int SV[15], SH[15];
struct Pos {
	int x, y;
	Pos(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
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
	Pos& operator *= (const int& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const int& n) { x /= n; y /= n; return *this; }
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
Pos dir[6] = { Pos(1, 0), Pos(1, 1), Pos(-1, 1), Pos(-1, 0), Pos(-1, -1), Pos(1, -1)};
void recur(Pos ps, Pos pe, int d, int s) {
	if (s == 1) {
		sx = std::min({ sx, ps.x, pe.x });
		ex = std::max({ ex, ps.x, pe.x });
		sy = std::min({ sy, ps.y, pe.y });
		ey = std::max({ ey, ps.y, pe.y });
		if (d == 0 || d == 3) {
			assert(ps.y == pe.y);
			int sx_ = std::min(ps.x, pe.x);
			int ex_ = std::max(ps.x, pe.x);
			int y = ps.y;
			for (int i = sx_; i <= ex_; i++) {
				B[y][i] = 1;
			}
		}
		else {
			int fx = std::abs(ps.x - pe.x);
			int fy = std::abs(ps.y - pe.y);
			assert(fx == fy);
			int dx = sign(pe.x - ps.x);
			int dy = sign(pe.y - ps.y);
			int x = ps.x;
			int y = ps.y;
			while (fx--) {
				B[y][x] = 1;
				x += dx;
				y += dy;
			}
		}
		return;
	}
	Pos pv = dir[d];
	int itx = pv.y ? SV[s - 1] - 1 : SH[s - 1] - 1;
	int ity = pv.y ? SV[s - 1] - 1 : 0;
	pv.x *= itx;
	pv.y *= ity;
	Pos p0 = ps;
	Pos p1 = ps + pv;
	Pos p2 = p1 + pv;
	Pos p3 = p2 + pv;
	assert(p3 == pe);
	recur(p0, p1, d, s - 1);
	recur(p2, p3, d, s - 1);
	pv = dir[(d + 5) % 6];
	itx = pv.y ? SV[s - 1] - 1 : SH[s - 1] - 1;
	ity = pv.y ? SV[s - 1] - 1: 0;
	pv.x *= itx;
	pv.y *= ity;
	Pos pm = p1 + pv;
	recur(p1, pm, (d + 5) % 6, s - 1);
	recur(pm, p2, (d + 1) % 6, s - 1);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	freopen("koch.out", "w", stdout);
	std::cin >> N;
	assert(N);
	if (N == 1) {
		std::cout << "  *****  \n";
		std::cout << " *     * \n";
		std::cout << "*       *\n";
		std::cout << " *     * \n";
		std::cout << "  *****  \n";
		return;
	}
	SV[1] = 3;
	SH[1] = 5;
	for (int i = 2; i <= 11; i++) {
		SV[i] = SV[i - 1] * 3 - 2;
		SH[i] = SH[i - 1] * 3 - 2;
	}

	//for (int i = 1; i < 9; i++) {
	//	std::cout << SV[i] << " ";
	//}
	//std::cout << "\n";
	//for (int i = 1; i < 9; i++) {
	//	std::cout << SH[i] << " ";
	//}

	Pos p0, p1, p2;
	sx = LEN >> 1;
	sy = LEN >> 1;
	p0 = Pos(sx, sy) - Pos(SH[N], SV[N - 1] / 2);
	p1 = p0 + Pos(SH[N] - 1);
	p2 = p0 + Pos(SV[N] - 1, SV[N] - 1);
	recur(p0, p1, 0, N);
	recur(p1, p2, 2, N);
	recur(p2, p0, 4, N);

	//std::cout << "v0 " << p1 - p0 << "\n";
	//std::cout << "v1 " << p2 - p1 << "\n";
	//std::cout << "v2 " << p0 - p2 << "\n";

	//std::cout << "\n";
	//std::cout << "sy:: " << sy << " ey:: " << ey << "\n";
	//std::cout << "sx:: " << sx << " ex:: " << ex << "\n";
	for (int y = sy; y <= ey; y++) {
		for (int x = sx; x <= ex; x++) {
			std::cout << (B[y][x] ? "*" : " ");
		}
		std::cout << "\n";
	}
	return;
}
int main() { solve(); return 0; }