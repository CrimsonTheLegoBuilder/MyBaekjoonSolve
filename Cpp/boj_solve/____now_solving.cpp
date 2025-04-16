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
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
const ld PI = acos(-1);
const int LEN = 1e5 + 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ll gcd(ll a, ll b) {
	a = std::abs(a);
	b = std::abs(b);
	while (b) {
		ll tmp = a % b;
		a = b;
		b = tmp;
	}
	return a;
}

//freopen("../../../input_data/triathlon_tests/triath.20", "r", stdin);
//freopen("../../../input_data/triathlon_tests/triathlon_out.txt", "w", stdout);

int N, M, T, Q;
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
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { sign(cross(d1, d2, d3, d4)); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos L[LEN][2], A[LEN], B[LEN];
ld D[LEN];
void intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2, ld& x, ld& y) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	ld det = a1 + a2;
	x = (p1.x * a2 + p2.x * a1) / det;
	y = (p1.y * a2 + p2.y * a1) / det;
	return;
}
ld dist(const Pos& a, const Pos& b, const ld& x, const ld& y) {
	ld a_ = hypot(a.x - x, a.y - y);
	ld b_ = hypot(b.x - x, b.y - y);
	return std::abs(a_ - b_);
}
void query() {
	std::cin >> N >> Q;
	for (int i = 0; i < N; i++) std::cin >> L[i][0] >> L[i][1];
	for (int q = 0; q < Q; q++) std::cin >> A[q] >> B[q];
	for (int q = 0; q < Q; q++) D[q] = -1;
	int irr = 0;
	for (int i = 1; i < N; i++) {
		ll fc = dot(L[0][0], L[0][1], L[i][0]);
		ll tq = cross(L[0][0], L[0][1], L[i][0]);
		if (fc && tq && std::abs(fc) != std::abs(tq)) {//other 0, PI/4, PI/2
			irr = 1;
			break;
		}
	}
	if (irr) {
		int cen = 1;
		int j = 1;
		ll c0 = -1, c1 = -1, g = -1;
		for (j; j < N; j++) {
			if (!ccw(L[j][0], L[j][1], L[0][0], L[0][1])) continue;
			else {
				c0 = cross(L[j][0], L[j][1], L[0][0]);
				c1 = cross(L[j][0], L[j][1], L[0][1]);
				g = gcd(c0, c1);
				c0 /= g; c1 /= g;
				if (c0 < 0) c0 *= -1, c1 *= -1;
				break;
			}
		}
		assert(j != N);
		for (int i = 1; i < N; i++) {
			if (!ccw(L[i][0], L[i][1], L[0][0], L[0][1])) {
				if (!collinear(L[i][0], L[i][1], L[0][0], L[0][1])) {
					cen = 0;
					break;
				}
			}
			else {
				ll t0 = cross(L[i][0], L[i][1], L[0][0]);
				ll t1 = cross(L[i][0], L[i][1], L[0][1]);
				g = gcd(t0, t1);
				t0 /= g; t1 /= g;
				if (t0 < 0) t0 *= -1, t1 *= -1;
				if (c0 != t0 || c1 != t1) {
					cen = 0;
					break;
				}
			}
		}
		if (cen) {//every line meet at only one point
			ld x, y;
			intersection(L[0][0], L[0][1], L[j][0], L[j][1], x, y);
			for (int q = 0; q < Q; q++) D[q] = dist(A[q], B[q], x, y);
		}
		else { for (int q = 0; q < Q; q++) D[q] = 0; }
	}
	else {
		int quart = 0;
		for (int i = 1; i < N; i++) {
			ll fc = dot(L[0][0], L[0][1], L[i][0]);
			ll tq = cross(L[0][0], L[0][1], L[i][0]);
			if (fc && tq && std::abs(fc) == std::abs(tq)) {
				quart = 1;
				break;
			}
		}
		if (quart) {

		}
		else {

		}
	}
	for (int q = 0; q < Q; q++) std::cout << D[q] << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> T;
	for (int t = 0; t < T; t++) query();
}
int main() { solve(); return 0; }//boj25726