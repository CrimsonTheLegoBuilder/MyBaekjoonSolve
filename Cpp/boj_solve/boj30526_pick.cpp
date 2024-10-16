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
const ll MOD = 1'000'000'007;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
inline ld tol(const ld& x) { return x + TOL * (x / std::abs(x)); }

//#define DEBUG
//#define BIG
#define WHAT_THE_FUCK

//Pick`s Theorem : A = i + b/2 - 1
int N, M, T, Q;
struct Pos {
	int x, y;
#ifdef BIG
	int den;
#endif
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {
#ifdef BIG
		den = 1;
#endif
	}
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
#ifdef BIG
	int modx() const { return std::abs(x) % std::abs(den); }
	int mody() const { return std::abs(y) % std::abs(den); }
	bool x_int() const { return !modx(); }
	bool y_int() const { return !mody(); }
	bool is_int() const { return !modx() && !mody(); }
	ld x() const { return x * 1. / den; }
	ld y() const { return y * 1. / den; }
#endif
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
std::pair<ld, ld> intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	ld x = (p1.x * a2 + p2.x * a1) * (ld)1. / (a1 + a2);
	ld y = (p1.y * a2 + p2.y * a1) * (ld)1. / (a1 + a2);
	return { x, y };
}
#ifdef BIG
Pos intersection_big(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	int a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	int x = (p1.x * a2 + p2.x * a1);
	int y = (p1.y * a2 + p2.y * a1);
	Pos ret = Pos(x, y);
	ret.den = a1 + a2;
	return ret;
}
#endif
ld get_y(Pos p0, Pos p1, ld x) {
	if (p1.x < p0.x) std::swap(p1, p0);
	Pos vec = p1 - p0;
	ld d = (ld)vec.y / vec.x;
	return p0.y + (x - p0.x) * d;
}
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
ll remain_count(Pos p0, Pos p1, Pos s, ll x, ll y, ld xf) {
#ifdef DEBUG
	std::cout << "FUCK:: remain\n";
	std::cout << "p0:: " << p0 << "\n";
	std::cout << "p1:: " << p1 << "\n";
	std::cout << " s:: " << s << "\n";
#endif
	if (p1 == s) return 0;
	if (p0.y == p1.y) return (s.y - y - 1) * (std::abs(s.x - x) + !zero(x - xf));
	if (std::abs(s.x - x) == 0) return (s.y - y - 1);
	ll dx = ll(p1.x - p0.x) / std::abs(p1.x - p0.x);
	ll dy = ll(p1.y - p0.y) / std::abs(p1.y - p0.y);
	int sz = std::abs(x - s.x);
	if (zero(x - xf)) sz--;
	ll cnt = 0;
	while (sz-- >= 0) {
		while (dx * ccw(p0, p1, s) < 0) { 
			s.y += dy;
		}
		s.y -= dy;
		cnt += s.y - y - (dy < 0 ? 1 : 0);
#ifdef DEBUG
			std::cout << "REMAIN::\n";
			std::cout << " sz:: " << sz << "\n";
			std::cout << "s.y:: " << s.y << "\n";
			std::cout << "  y:: " << y << "\n";
			std::cout << "cnt:: " << cnt << "\n";
			std::cout << "s.x:: " << s.x << "\n";
#endif
		s.x += dx;
	}
	return cnt;
}
ll tri_count(Pos p0, Pos p1, Pos p2, Pos p3) {
	/*
	p3 <-
	   <- p2 <-
	      ^    <-  Pos(x, y)
	      |    ->      *
	   -> p1 ->
	p0 ->
	*/
	ll cnt = 0;
	ll t0 = 0, t1 = 0, t2 = 0;
	ll c0 = 0, c1 = 0, c2 = 0;
	auto [x, y] = intersection(p0, p1, p2, p3);
#ifdef DEBUG
	std::cout << "tri start:: p0:: " << p0 << " p1 :: " << p1 << " p2 :: " << p2 << " p3 :: " << p3 << " FUCK::\n";
	std::cout << "inx:: (" << x << ", " << y << ")\n";
#endif
	ld dx0 = x - p0.x;
	ld dx1 = x - p1.x;
	ld dy0 = y - p0.y;
	ld dx2 = x - p2.x;
	ld dx3 = x - p3.x;
	ld dy3 = y - p3.y;
	ll x0 = (ll)tol(dx0) + p0.x;
	ll y0 = (ll)tol(dy0) + p0.y;
	ll x3 = (ll)tol(dx3) + p3.x;
	ll y3 = (ll)tol(dy3) + p3.y;
	ll Y = std::min({ y0 - 1, y3 - 1, (ll)p0.y, (ll)p1.y, (ll)p2.y, (ll)p3.y });
#ifdef DEBUG
	std::cout << "DEBUG:: dx0:: " << dx0 << " FUCK::\n";
	std::cout << "DEBUG:: Y:: " << Y << " FUCK::\n";
#endif
	Pos v0 = p1 - p0, v3 = p2 - p3;
	ll gcd0 = gcd(std::abs(v0.x), std::abs(v0.y)), gcd3 = gcd(std::abs(v3.x), std::abs(v3.y));
	v0 /= (int)gcd0;
	v3 /= (int)gcd3;
	Pos v2 = p2 - p1;
	ll gcd2 = gcd(std::abs(v2.x), std::abs(v2.y));
	c0 = std::abs(gcd2);
	t0 = pick(p1, p2, Y);
	if (v0.x) {
		int n0 = (ll)tol(dx0) / v0.x;
		c1 = std::abs((ll)tol(dx1) / v0.x);
		if (zero(x - (p1.x + c1 * v0.x)) && zero(y - (p1.y + c1 * v0.y))) c1--;
		Pos q0 = p0 + v0 * n0;
		if (ccw(p3, p2, q0) > 0) q0 -= v0;
		t1 = pick(p1, q0, Y);
		ll X = x0;
		//if (zero((ll)tol(x) - x)) X -= v0.x / std::abs(v0.x);
#ifdef DEBUG
		std::cout << "FUCK:: v0\n";
		std::cout << "FUCK:: t1:: " << t1 << "\n";
		std::cout << "FUCK:: t1:: p0:: " << p0 << " p1:: " << p1 << " X:: "  << X << "\n";
		std::cout << "FUCK:: t1:: q0:: " << q0 << "\n";
#endif
		t1 += remain_count(p0, p1, q0, X, Y, x);
#ifdef DEBUG
		std::cout << "FUCK:: t1:: " << t1 << "\n";
#endif
	}
	if (v3.x) {
		int n3 = (ll)tol(dx3) / v3.x;
		c2 = std::abs((ll)tol(dx2) / v3.x);
		if (zero(x - (p2.x + c2 * v3.x)) && zero(y - (p2.y + c2 * v3.y))) c2--;
		Pos q3 = p3 + v3 * n3;
		if (ccw(p0, p1, q3) < 0) q3 -= v3;
		t2 = pick(p2, q3, Y);
		ll X = x3;
		//if (zero((ll)tol(x) - x)) X -= v3.x / std::abs(v3.x);
#ifdef DEBUG
		std::cout << "FUCK:: v3\n";
		std::cout << "FUCK:: t2:: " << t2 << "\n";
		std::cout << "FUCK:: t2:: p2:: " << p2 << " p3:: " << p3 << " X:: " << X << "\n";
		std::cout << "FUCK:: t2:: q3:: " << q3 << "\n";
#endif
		t2 += remain_count(p3, p2, q3, X, Y, x);
#ifdef DEBUG
		std::cout << "FUCK:: t2:: " << t2 << "\n";
#endif
	}
#ifdef DEBUG
	std::cout << "DEBUG:: t0:: " << t0 << " t1:: " << t1 << " t2:: " << t2 << "\n";
	std::cout << "DEBUG:: c0:: " << c0 << " c1:: " << c1 << " c2:: " << c2 << "\n";
#endif
	//what the fuck
	Pos a0 = p1, a1 = p2;
	if (a0.x == a1.x) return std::abs(t1 - t2);
	if (a1.x < a0.x) std::swap(a0, a1);
	if (sign(x - a0.x) >= 0 && sign(a1.x - x) >= 0) {
		if (std::abs(a0.x - a1.x) <= 1) return 0;
		ld my = get_y(a0, a1, x);
		if (sign(my - y) > 0) {
			cnt = t0 - t1 - t2;
			cnt -= c1 + c2;
			if (zero((ll)tol(x) - x)) {
				ll dy = (ll)tol(y - Y);
				cnt += dy;
				if (zero((ll)tol(y) - y)) cnt++;
			}
		}
		else {
			cnt = t1 + t2 - t0;
			cnt -= c0 - 1;
			if (zero((ll)tol(x) - x)) {
				ll dy = (ll)tol(y - ll(my + 1));
				cnt += dy + 1;
				if (zero((ll)tol(y) - y)) cnt--;
			}
		}
	}
	else if (v2.x > 0) {
		cnt += t0;
		if (sign(x - a1.x) > 0) {
			cnt += t2;
			cnt -= t1;
			cnt -= c1;
			ll dy = a1.y - Y - 1;
			cnt += dy;
		}
		else if (sign(x - a0.x) < 0) {
			cnt += t1;
			cnt -= t2;
			cnt -= c2;
			ll dy = a0.y - Y - 1;
			cnt += dy;
		}
	}
	else if (v2.x < 0) {
		cnt -= t0;
		cnt -= c0 - 1;
		if (sign(x - a1.x) > 0) {
			cnt += t2;
			cnt -= t1;
			cnt -= c1;
			ll dy = a1.y - Y;
			cnt -= dy;
		}
		else if (sign(x - a0.x) < 0) {
			cnt += t1;
			cnt -= t2;
			cnt -= c2;
			ll dy = a0.y - Y;
			cnt -= dy;
		}
	}
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
	ll ret = 0;
	if (N == 3) { std::cout << "infinitely many\n"; return; }
	if (N == 4) {
		const Pos& p0 = H[0], p1 = H[1], p2 = H[2], p3 = H[3];
		if (!cross(p0, p1, p2, p3) && !cross(p1, p2, p3, p0) && area(H) == 2) { std::cout << "0\n"; return; }
	}
	for (int i = 0; i < N; i++) {
		const Pos& p0 = H[(i - 1 + N) % N], p1 = H[i], p2 = H[(i + 1) % N], p3 = H[(i + 2) % N];
		if (ccw(p0, p1, p2, p3) <= 0) {
			std::cout << "infinitely many\n";
			return;
		}
		ll tmp = tri_count(p0, p1, p2, p3);
#ifdef DEBUG
		std::cout << "\n";
		std::cout << "CNT:: " << tmp << "\n";
		std::cout << "\n";
#endif
		ret += tmp;
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj30526


//refer to SolitaryDream
//#include <bits/stdc++.h>
//using namespace std;
//#define FOR(i,s,t) for(int i=(s),_t=(t); i<=_t; ++i)
//#define DOR(i,s,t) for(int i=(s),_t=(t); i>=_t; --i)
//typedef double db;
//typedef long long ll;
//const int N = 55;
//struct Point {
//	int x, y;
//	Point operator +(const Point& s) const {
//		return { x + s.x,y + s.y };
//	}
//	Point operator -(const Point& s) const {
//		return { x - s.x,y - s.y };
//	}
//	int Len2() const {
//		return x * x + y * y;
//	}
//};
//int det(Point a, Point b) {
//	return a.x * b.y - a.y * b.x;
//}
//db intersetion(Point a, Point b, Point c, Point d) {
//	return a.x + (b - a).x * (1.0 * det(d - c, a - c) / det(b - a, d - c));
//}
//ll f(ll n, ll a, ll b, ll m) {
//	if (b == 0) return n * (a / m);
//	if (a >= m) return n * (a / m) + f(n, a % m, b, m);
//	if (b >= m) return (n - 1) * n / 2 * (b / m) + f(n, a, b % m, m);
//	return f((a + b * n) / m, (a + b * n) % m, m, b);
//}
//ll g(ll n, ll a, ll b, ll m) {
//	if (m < 0) m = -m, a = -a, b = -b;
//	if (b < 0) {
//		a += b * (n - 1);
//		b = -b;
//	}
//	ll s = 0;
//	if (a < 0) {
//		ll aa = (a % m + m) % m;
//		s -= (aa - a) / m * n;
//		a = aa;
//	}
//	return s + f(n, a, b, m);
//}
//const db eps = 1e-9;
//db mnx;
//ll solve(Point a, Point b, db l, db r) {
//	if (a.x == b.x) return 0;
//	if (l > r) swap(l, r);
//	if (fabs(mnx - l) < eps) l += eps;
//	ll xl = ceil(l), xr = floor(r - eps);
//	if (a.x > b.x) {
//		return g(xr - xl + 1, -(-a.x * b.y + b.x * a.y + xl * (b.y - a.y)) - 1, -(b.y - a.y), -(b.x - a.x));
//	}
//	else {
//		// if(l==-5 && r==-1) cerr << xl << ' ' << xr << endl;
//		return -g(xr - xl + 1, -a.x * b.y + b.x * a.y + xl * (b.y - a.y), b.y - a.y, b.x - a.x);
//	}
//}
//Point a[N];
//int main() {
//	ios::sync_with_stdio(false);
//	cin.tie(nullptr);
//	int n;
//	cin >> n;
//	FOR(i, 0, n - 1) cin >> a[i].x >> a[i].y;
//	ll res = 0;
//	FOR(i, 0, n - 1) {
//		int j = (i + 1) % n, k = (j + 1) % n, h = (k + 1) % n;
//		int s = det(a[j] - a[i], a[h] - a[k]);
//		if (s < 0) {
//			cout << "infinitely many\n";
//			return 0;
//		}
//		if (s == 0) {
//			mnx = 0;
//			if (solve(a[i], a[j], 0, 1e9) + solve(a[k], a[h], 0, 1e9)) {
//				cout << "infinitely many\n";
//				return 0;
//			}
//			continue;
//		}
//		db px = intersetion(a[i], a[j], a[k], a[h]);
//		mnx = std::min({ px,(db)a[j].x,(db)a[k].x });
//		res += solve(a[i], a[j], a[j].x, px);
//		res += solve(a[k], a[h], a[k].x, px);
//		res += solve(a[k], a[j], a[j].x, a[k].x);
//		// if(i==0) {
//		//     cerr << solve(a[i],a[j],a[j].x,px) << ' ' << solve(a[k],a[h],a[k].x,px) << ' ' << solve(a[k],a[j],a[j].x,a[k].x) << endl;
//		// }
//		// cerr << a[j].x << ' ' << a[j].y << ' ' << res << endl;
//	}
//	cout << res << '\n';
//	return 0;
//}