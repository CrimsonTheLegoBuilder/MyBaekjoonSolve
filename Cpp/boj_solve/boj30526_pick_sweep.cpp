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
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-10;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
inline ld tol(const ld& x) { return x + TOL * (x / std::abs(x)); }

#define DEBUG
#define WHAT_THE_FUCK

//Pick`s Theorem : A = i + b/2 - 1
int N, M, T, Q;
struct Pos {
	ll x, y;
	ll den;
	Pos(ll X = 0, ll Y = 0, ll d = 0) : x(X), y(Y), den(d) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
	ll modx() const { return std::abs(x) % std::abs(den); }
	ll mody() const { return std::abs(y) % std::abs(den); }
	bool x_int() const { return !modx(); }
	bool y_int() const { return !mody(); }
	bool is_int() const { return !modx() && !mody(); }
	ld x_() const { return x * 1. / den; }
	ld y_() const { return y * 1. / den; }
	Pos p() const { return Pos(x / den, y / den); }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return !ret ? 0 : ret > 0 ? 1 : -1; }
ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
void norm(Polygon& H) { ll A = area(H); if (A < 0) std::reverse(H.begin(), H.end()); return; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	ll x = (p1.x * a2 + p2.x * a1);
	ll y = (p1.y * a2 + p2.y * a1);
	Pos ret = Pos(x, y);
	ret.den = a1 + a2;
	return ret;
}
ld get_y(Pos p0, Pos p1, ld x) {
	if (p1.x < p0.x) std::swap(p1, p0);
	Pos vec = p1 - p0;
	ld d = (ld)vec.y / vec.x;
	return p0.y + (x - p0.x) * d;
}
ll gcd(const Pos& p) { return gcd(std::abs(p.x), std::abs(p.y)); }
ll pick(const Pos& p0, const Pos& p1, const ll& y) {
	if (std::abs(p0.x - p1.x) <= 1) return 0;
	ll cnt = 0;
	ll dx = (ll)p0.x - p1.x;
	ll dy = (ll)p0.y - p1.y;
	ll _gcd = gcd(std::abs(dx), std::abs(dy));
	ll A2 = std::abs(dx * ((ll)p0.y + p1.y - y - y));
	ll b = _gcd + std::abs(p0.y - y) + std::abs(p1.y - y) + std::abs(dx);
	//Pick`s Theorem : A = i + b / 2 - 1
	ll i = (A2 - b + 2);
	assert(!(i & 1ll));
	return i >> 1;
}
ll remain_count(const Pos& p0, const Pos& p1, Pos s, const ll& x, const ll& y) {
	Pos v = p1 - p0;
	assert(v.x);
	ll dx = v.x / std::abs(v.x);
	int sz = (x - s.x) * dx;
#ifdef DEBUG
	std::cout << "REMAIN::  s:: " << s << "\n";
	std::cout << "REMAIN::  x:: " << x << "\n";
	std::cout << "REMAIN:: sz:: " << sz << "\n";
#endif
	if (!v.y) {
		if (sz < 0) return 0;
		return (std::abs(x - s.x) + 1) * (s.y - y - 1);
	}
	ll dy = v.y / std::abs(v.y);
	assert(sz <= 2005);
	ll cnt = 0;
	while (sz-- >= 0) {
		while (dx * dy * ccw(p0, p1, s) < 0) s.y += dy;
		if (dx * ccw(p0, p1, s) >= 0) s.y--;
		cnt += s.y - y;
		s.x += dx;
	}
#ifdef DEBUG
	std::cout << "REMAIN:: cnt:: " << cnt << "\n";
#endif
	return cnt;
}
ll count(const Pos& p0, const Pos& p1, const Pos& p, const ll& miny, const ll& minx, const ll& dx) {
	if (!dx) {
		if (p0.x != minx) return 0;
		if (!p.den) return -std::abs(p0.y - p1.y);
		ll ty = p1.y;
		ll by = (p.y_int()) ? p.y / p.den :	floor(p.y_());
		return -std::abs(ty - by);
	}

	ll cnt = 0;
	Pos v = p1 - p0;

	if (!p.den) {//p1 - p2
		cnt = pick(p0, p1, miny);
		if (dx < 0) return -(cnt + (p1.y - miny) + (gcd(v) - 1));
		return cnt + (p0.y - miny) - (p0.x != minx);
	}

	v /= gcd(v);

	ll vx = ll(p.x_() - p1.x);
	if (p.x_int()) vx = (p.x / p.den - p1.x) - (v.x / std::abs(v.x));

	ll n = std::abs(vx) / std::abs(v.x);
	ll X = p1.x + vx;

	Pos q0;
	if (p.is_int()) q0 = p.p();
	else q0 = p1 + v * n;
	
	ll tmp = pick(p1, q0, miny);
	cnt += tmp;
#ifdef DEBUG
	std::cout << "CNT before remain:: " << cnt << "\n";
#endif
	if (q0.x == p1.x) q0.x += v.x / std::abs(v.x);
	cnt += remain_count(p0, p1, q0, X, miny);
	//if ((!cnt && std::abs(p1.x - p.x_()) > 1.) || cnt)
	//	cnt += remain_count(p0, p1, q0, X, miny);
#ifdef DEBUG
	std::cout << "CNT after remain:: " << cnt << "\n";
	std::cout << "               n:: " << n << "\n";
#endif
	Pos w0 = p1, w1 = p1;
	if (w1.x < w0.x) std::swap(w0, w1);
	if (p.x_int() && p.x_() < w0.x) {
		ll y = p.y_int() ? p.y / p.den : floor(p.y_());
		cnt += y - miny;
		if (dx > 0 && p.p().x != minx && p.y_int()) cnt--;
#ifdef DEBUG
		std::cout << "CNT left++ :: " << cnt << "\n";
#endif
	}
	else if (p.x_() > w0.x) {
		cnt += w0.y - miny;
		if (dx > 0 && w0.x != minx) cnt--;
#ifdef DEBUG
		std::cout << "CNT left++ :: " << cnt << "\n";
#endif
	}
	if (dx < 0) { cnt += n; cnt *= -1ll; }
#ifdef DEBUG
	std::cout << "CNT total:: " << cnt << "\n";
#endif
	return cnt;
}
ll count(Pos p0, Pos p1, Pos p2, Pos p3) {
	ll cnt = 0;
	Pos p = intersection(p0, p1, p2, p3);
	assert(p.den);
	ll minx = floor(p.x_());
	if (p.x_int()) minx = p.p().x;
	minx = std::min({ minx, p1.x, p2.x });
	ll miny = floor(p.y_());
	miny = std::min({ miny, p0.y, p1.y, p2.y, p3.y }) - 1;
	Pos z = Pos(0, 0, 0);
#ifndef DEBUG
	cnt += count(p0, p1, p, miny, minx, p0.x - p1.x);
	cnt += count(p1, p2, z, miny, minx, p2.x - p1.x);
	cnt += count(p3, p2, p, miny, minx, p2.x - p3.x);
#else
	ll tmp;
	std::cout << "\nCOUNT::\nDEBUG::  Y:: " << miny << " X:: " << minx << "\n\n";
	std::cout << "DEBUG:: p0:: " << p0 << " p1:: " << p1 << " p2:: " << p2 << " p3:: " << p3 << "\n\n";
	std::cout << "DEBUG:: intersection:: " << p.x_() << " " << p.y_() << "\n\n";
	std::cout << "DEBUG:: p0:: " << p0 << " p1:: " << p1 << "\n";
	tmp = count(p0, p1, p, miny, minx, p0.x - p1.x);
	cnt += tmp;
	std::cout << "DEBUG:: p0, p1:: tmp:: " << tmp << "\n\n";

	std::cout << "DEBUG:: p1:: " << p1 << " p2: " << p2 << "\n";
	tmp = count(p1, p2, z, miny, minx, p2.x - p1.x);
	cnt += tmp;
	std::cout << "DEBUG:: p1, p2:: tmp:: " << tmp << "\n\n";

	std::cout << "DEBUG:: p2:: " << p2 << " p3:: " << p3 << "\n";
	tmp = count(p3, p2, p, miny, minx, p2.x - p3.x);
	cnt += tmp;
	std::cout << "DEBUG:: p2, p3:: tmp:: " << tmp << "\n\n";
	
	std::cout << "DEBUG::          CNT:: " << cnt << "\n";
#endif
	return cnt;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);
	if (N == 3) { std::cout << "infinitely many\n"; return; }
	if (N == 4) {
		const Pos& p0 = H[0], p1 = H[1], p2 = H[2], p3 = H[3];
		if (!cross(p0, p1, p2, p3) && !cross(p1, p2, p3, p0) && area(H) == 2) { std::cout << "0\n"; return; }
	}
	ll ret = 0;
	for (int i = 0; i < N; i++) {
		const Pos& p0 = H[(i - 1 + N) % N], p1 = H[i], p2 = H[(i + 1) % N], p3 = H[(i + 2) % N];
		if (ccw(p0, p1, p2, p3) <= 0) {
			std::cout << "infinitely many\n";
			return;
		}
		ll cnt = count(p0, p1, p2, p3);
#ifdef DEBUG
		std::cout << "\n";
		std::cout << "CNT:: " << cnt << "\n";
		std::cout << "\n";
#endif
		ret += cnt;
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj30526

/*

5
0 2
-2 0
-1 -3
1 -3
2 1

23

8
0 2
0 1
1 0
2 0
3 1
3 2
2 3
1 3

0

6
0 0
400 1
400 2
0 3
-400 2
-400 1

1596

6
999 0
499 865
-499 865
-999 0
-499 -865
499 -865

2588522

*/

/*

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

int N;
struct Pos {
	ll x, y;
	ll den;
	Pos(ll X = 0, ll Y = 0, ll d = 0) : x(X), y(Y), den(d) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
	ll modx() const { return std::abs(x) % std::abs(den); }
	ll mody() const { return std::abs(y) % std::abs(den); }
	bool x_int() const { return !modx(); }
	bool y_int() const { return !mody(); }
	bool is_int() const { return !modx() && !mody(); }
	ld x_() const { return x * 1. / den; }
	ld y_() const { return y * 1. / den; }
	Pos p() const { return *this / den; }
}; const Pos O = Pos(0, 0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return !ret ? 0 : ret > 0 ? 1 : -1; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return !ret ? 0 : ret > 0 ? 1 : -1; }
ll area(const Polygon& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
void norm(Polygon& H) { ll A = area(H); if (A < 0) std::reverse(H.begin(), H.end()); return; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	ll x = (p1.x * a2 + p2.x * a1);
	ll y = (p1.y * a2 + p2.y * a1);
	return Pos(x, y, a1 + a2);
}
ll gcd(const Pos& p) { return gcd(std::abs(p.x), std::abs(p.y)); }
//Pick`s Theorem : A = i + b / 2 - 1
ll pick(const Pos& p0, const Pos& p1, const ll& y) {
	if (std::abs(p0.x - p1.x) <= 1) return 0;
	ll cnt = 0;
	Pos v = p1 - p0;
	ll _gcd = gcd(v);
	ll A2 = std::abs(v.x * (p0.y + p1.y - y - y));
	ll b = _gcd + std::abs(p0.y - y) + std::abs(p1.y - y) + std::abs(v.x);
	ll i2 = (A2 - b + 2);
	assert(!(i2 & 1ll));
	return i2 >> 1;
}
ll remain_count(const Pos& p0, const Pos& p1, Pos s, const ll& x, const ll& y) {
	Pos v = p1 - p0;
	assert(v.x);
	ll dx = v.x / std::abs(v.x);
	int sz = (x - s.x) * dx;
	if (!v.y) {
		if (sz < 0) return 0;
		return (std::abs(x - s.x) + 1) * (s.y - y - 1);
	}
	ll dy = v.y / std::abs(v.y);
	assert(sz <= 2005);
	ll cnt = 0;
	while (sz-- >= 0) {
		while (dx * dy * ccw(p0, p1, s) < 0) s.y += dy;
		if (dx * ccw(p0, p1, s) >= 0) s.y--;
		cnt += s.y - y;
		s.x += dx;
	}
	return cnt;
}
ll count(const Pos& p0, const Pos& p1, const Pos& p, const ll& miny, const ll& minx, const ll& dx) {
	if (!dx) {
		if (p0.x != minx) return 0;
		if (!p.den) return -std::abs(p0.y - p1.y);
		ll ty = p1.y;
		ll by = (p.y_int()) ? p.p().y : floor(p.y_());
		return -std::abs(ty - by);
	}

	ll cnt = 0;
	Pos v = p1 - p0;

	if (!p.den) {//p1 - p2
		cnt = pick(p0, p1, miny);
		if (dx < 0) return -(cnt + (p1.y - miny) + (gcd(v) - 1));
		return cnt + (p0.y - miny) - (p0.x != minx);
	}

	v /= gcd(v);
	ll qx = v.x / std::abs(v.x);

	ll vx = ll(p.x_() - p1.x);
	if (p.x_int()) vx = (p.p().x - p1.x) - qx;

	ll n = std::abs(vx) / std::abs(v.x);
	ll X = p1.x + vx;

	Pos q0;
	if (p.is_int()) q0 = p.p();
	else q0 = p1 + v * n;

	cnt += pick(p1, q0, miny);
	if (q0.x == p1.x) q0.x += qx;
	cnt += remain_count(p0, p1, q0, X, miny);

	Pos w0 = p1, w1 = p1;
	if (w1.x < w0.x) std::swap(w0, w1);
	if (p.x_int() && p.p().x < w0.x) {
		ll y = p.y_int() ? p.p().y : floor(p.y_());
		cnt += y - miny;
		if (dx > 0 && p.p().x != minx && p.y_int()) cnt--;
	}
	else if (p.x_() > w0.x) {
		cnt += w0.y - miny;
		if (dx > 0 && w0.x != minx) cnt--;
	}

	if (dx < 0) { cnt += n; cnt *= -1ll; }
	return cnt;
}
ll count(Pos p0, Pos p1, Pos p2, Pos p3) {
	ll cnt = 0;
	Pos p = intersection(p0, p1, p2, p3);
	assert(p.den);
	ll minx = floor(p.x_());
	if (p.x_int()) minx = p.p().x;
	minx = std::min({ minx, p1.x, p2.x });
	ll miny = floor(p.y_());
	miny = std::min({ miny, p0.y, p1.y, p2.y, p3.y }) - 1;
	cnt += count(p0, p1, p, miny, minx, p0.x - p1.x);
	cnt += count(p1, p2, O, miny, minx, p2.x - p1.x);
	cnt += count(p3, p2, p, miny, minx, p2.x - p3.x);
	return cnt;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);
	if (N == 3) { std::cout << "infinitely many\n"; return; }
	if (N == 4) {
		const Pos& p0 = H[0], p1 = H[1], p2 = H[2], p3 = H[3];
		if (!ccw(p0, p1, p2, p3) && !ccw(p1, p2, p3, p0) && area(H) == 2) { std::cout << "0\n"; return; }
	}
	ll ret = 0;
	for (int i = 0; i < N; i++) {
		const Pos& p0 = H[(i - 1 + N) % N], p1 = H[i], p2 = H[(i + 1) % N], p3 = H[(i + 2) % N];
		if (ccw(p0, p1, p2, p3) <= 0) { std::cout << "infinitely many\n"; return; }
		ret += count(p0, p1, p2, p3);
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj30526

*/