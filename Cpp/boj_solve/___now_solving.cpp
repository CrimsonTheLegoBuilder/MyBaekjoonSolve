#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
const ld INF = 1e21;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e5 + 10;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

int N, K, Q;
ll A[LEN];
struct Pdd {
	ld x, y;
	Pdd(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	ld operator / (const Pdd& p) const { return x * p.x + y * p.y; }
	Pdd operator + (const Pdd& p) const { return Pdd(x + p.x, y + p.y); }
	Pdd operator - (const Pdd& p) const { return Pdd(x - p.x, y - p.y); }
	Pdd rot(const ld& t) const { return Pdd(x * cos(t) - y * sin(t), x * sin(t) + y * cos(t)); }
};
struct Pos {
	int x, y;
	ld t;
	Pos(int x_ = 0, int y_ = 0, ld t_ = 0) : x(x_), y(y_), t(t_) {}
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
	Pdd p() const { return Pdd(x, y); }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld rad(const Pos& d1, const Pos& d2, const Pos& d3) { return rad(d2 - d1, d3 - d1); }
ld rad(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return rad(d2 - d1, d4 - d3); }
ld operator / (const Pos& p, const Pdd& q) { return p.x * q.y - p.y * q.x; }
Pdd operator + (const Pos& p, const Pdd& q) { return Pdd(p.x + q.x, p.y + q.y); }
Pdd operator - (const Pos& p, const Pdd& q) { return Pdd(p.x - q.x, p.y - q.y); }
ld operator / (const Pdd& p, const Pos& q) { return p.x * q.x + p.y * q.y; }
Pdd operator + (const Pdd& p, const Pos& q) { return Pdd(p.x + q.x, p.y + q.y); }
Pdd operator - (const Pdd& p, const Pos& q) { return Pdd(p.x - q.x, p.y - q.y); }
ld intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pdd& q2) {
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Pos bi_search(const Polygon& H, const Pos& c, const int& i1, const int& i2, ld& ix) {
	auto fit = [&](const int& i) -> int { return i % N; };
	int f = ccw(c, H[i1], H[i2]);
	assert(f);
	int sz = H.size();
	ld t, r;
	int s = 1, e = N - 2;
	if (f > 0) {
		Pos h = ~(H[i2] - H[i1]);
		Pos v = c - H[i1];
		//t = rad(h, v);
		t = rad(H[i1], c, H[i2]);
		t = norm(PI - t);
		Pdd m0 = v.p().rot(t);
		r = norm(rad(H[i1], H[i2], H[(i1 - 1 + sz) % sz]));
		if (r <= t) return Pos(-1, -1);
		while (s < e) {
			int m = s + e >> 1;
			r = norm(rad(H[i1], H[i2], H[fit(i1 + m)]));
			if (eq(t, r)) { ix = 0; return Pos(fit(i1 + m), -1); }
			if (r < t) s = m + 1;
			else e = m;
		}
		ix = intersection(H[fit(s - 1)], H[fit(s)], H[i1], m0);
		return Pos(fit(i1 + s - 1 + N), fit(i1 + s));
	}
	else {
		Pos h = -~(H[i2] - H[i1]);
		Pos h2 = H[i1] - (H[i2] - H[i1]);
		Pos v = c - H[i1];
		//t = rad(h, v);
		t = rad(H[i1], H[i2], c);
		t = norm(PI - t);
		Pdd m0 = v.p().rot(t);
		t = norm(PI * .5 + t);
		r = norm(rad(H[i1], h2, H[(i1 + 1) % sz]));
		if (r >= t) return Pos(-1, -1);
		while (s < e) {
			int m = s + e >> 1;
			r = norm(rad(H[i1], h2, H[fit(i1 + m)]));
			if (eq(t, r)) { ix = 0; return Pos(fit(i1 + m), -1); }
			if (r < t) s = m + 1;
			else e = m;
		}
		ix = intersection(H[fit(s - 1)], H[fit(s)], H[i1], m0);
		return Pos(fit(i1 + s - 1 + N), fit(i1 + s));
	}
	assert(0);
	return Pos();
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(13);
	//freopen("../../../input_data//", "r", stdin);
	//freopen("../../../input_data//", "w", stdout);
	std::cin >> N;
	Polygon H(N); for (Pos& p : H) std::cin >> p;
	Pos c; std::cin >> c;
	for (int i = 0; i < N; i++) A[i + 1] = A[i] + cross(c, H[i], H[(i + 1) % N]);
	for (int i = 0; i < N; i++) {
		int i0 = (i - 1 + N) % N, i1 = i % N, i2 = (i + 1) % N, i3 = (i + 2) % N;
		const Pos& p0 = H[i0], & p1 = H[i1], & p2 = H[i2], & p3 = H[i3];
		ld ix;
		ld al = 0, ar = 0;
		ld tri = 0;
		ll fan = 0;
		Pos pl = bi_search(H, c, i1, i2, ix);
		if (pl.x == -1) al = 0;
		else {
			if (pl.y == -1) {
				int j = pl.x;
				if (i1 < j) {
					fan = A[j] - A[i1] + cross(c, H[j], H[i1]);
					al = A[N] - fan;
				}
				else al = A[i1] - A[j] + cross(c, H[i1], H[j]);
			}
			else {
				int j0 = pl.x, j1 = pl.y;
				if (i1 < j1) {
					fan = A[j1] - A[i1] + cross(c, H[j1], H[i1]);
					al = A[N] - fan;
				}
				else al = A[i1] - A[j1] + cross(c, H[i1], H[j1]);
				tri = cross(H[i1], H[j0], H[j1]) * (1 - ix);
				al += tri;
			}
		}
		Pos pr = bi_search(H, c, i2, i1, ix);
		if (pr.x == -1) ar = 0;
		else {
			if (pr.y == -1) {
				int j = pr.x;
				if (i1 < j) ar = A[j] - A[i1] + cross(c, H[j], H[i1]);
				else {
					fan = A[i1] - A[j] + cross(c, H[i1], H[j]);
					ar = A[N] - fan;
				}
			}
			else {
				int j0 = pr.x, j1 = pr.y;
				if (i1 < j0) ar = A[j0] - A[i1] + cross(c, H[j0], H[i1]);
				else {
					fan = A[i1] - A[j0] + cross(c, H[i1], H[j0]);
					ar = A[N] - fan;
				}
				tri = cross(H[i1], H[j0], H[j1]) * ix;
				ar += tri;
			}
		}
		std::cout << A[N] - al - ar << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj28942


//29688
//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//
//int X, Y, N, W, D;
//std::string s;
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(10);
//	std::cin >> X >> Y >> N;
//	while (N--) {
//		std::cin >> W;
//		D = W >= 1000 ? W - 1000 : 0;
//		std::cout << W << " " << std::min(1000, W) * X + D * Y << "\n";
//	}
//	return 0;
//}