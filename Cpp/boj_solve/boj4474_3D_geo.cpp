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
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
ll gcd(ll a, ll b, ll c) { a = std::abs(a); b = std::abs(b); c = std::abs(c); a = gcd(a, b); return gcd(a, c); }

Vint ans;
ld sc[4];
int N, M, K, Q;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
}; const Pos O = { 0, 0 };
struct Pos3D {
	ll x, y, z;
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ll& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const ll& n) const { return { x / n, y / n, z / n }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ll& n) { x *= n; y *= n; z *= n; return *this; }
	Pos3D& operator /= (const ll& n) { x /= n; y /= n; z /= n; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
};
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
const Pos3D MAXP3D = { INF, INF, INF };
typedef std::vector<Pos3D> Polygon3D;
typedef std::vector<Polygon3D> Polyhedron;
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) * (d4 - d3); }
Pos3D norm(const Pos3D& p) { ll n = gcd(p.x, p.y, p.z); return p / n; }
Pos3D norm(const Polygon3D& P) {
	int sz = P.size();
	assert(sz >= 3);
	Pos3D v = cross(P[0], P[1], P[2]);
	return norm(v);
}
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& n) { return sign(n * norm(cross(d1, d2, d3))); }
void update_sc(const Pos3D& p) {
	ld angle1 = -atan2l(p.y, p.x);
	ld dx = sqrtl(p.x * p.x + p.y * p.y);
	ld angle2 = -atan2l(dx, p.z);
	sc[0] = sinl(angle1);
	sc[1] = cosl(angle1);
	sc[2] = sinl(angle2);
	sc[3] = cosl(angle2);
	return;
}
Pos rotate(const Pos3D& p) {
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	x = z * sc[2] + x * sc[3];
	y = y;
	//z = z * sc[3] - x * sc[2];
	return Pos(x, y);
}
bool query() {
	std::cin >> N;
	int cnt = 0;
	if (!N) return 0;
	Polyhedron H(N);
	for (int i = 0; i < N; i++) {
		Polygon3D& C = H[i];
		C.resize(3);
		for (Pos3D& p : C) std::cin >> p;
	}
	for (int i = 0; i < N; i++) {
#ifdef INT
		Pos3D ni = norm(H[i]);
		Pos3D ref = norm(H[i][1] - H[i][0]);
		Polygon3D V;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos3D nj = norm(H[j]);
			if ((ni / nj).Euc() == 0) continue;
			V.push_back(nj);
		}
		std::sort(V.begin(), V.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
			int tq1 = ccw(O3D, ref, p, ni);
			bool f1 = tq1 > 0 || (tq1 == 0 && ref * p > 0);
			int tq2 = ccw(O3D, ref, q, ni);
			bool f2 = tq2 > 0 || (tq2 == 0 && ref * q > 0);
			if (f1 != f2) return f1;
			return ccw(O3D, p, q, ni) > 0;
			});
		int sz = V.size();
		for (int j = 0, k = 1; j < sz; j++) {
			while (ccw(O3D, V[j], V[k], ni) > 0) k = (k + 1) % sz;
			cnt = std::max(cnt, (k - j + sz) % sz + 1);
		}
#else
		Pos3D ni = norm(H[i]);
		update_sc(ni);
		Pos ref = rotate(norm(H[i][1] - H[i][0]));
		Vld V;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos3D nj = norm(H[j]);
			if ((ni / nj).Euc() == 0) continue;
			V.push_back(norm(rad(ref, rotate(nj))));
		}
		std::sort(V.begin(), V.end());
		int sz = V.size();
		for (int j = 0, k = 1; j < sz; j++) {
			while (norm(V[j] - V[k]) > PI) k = (k + 1) % sz;
			cnt = std::max(cnt, (k - j + sz) % sz + 1);
		}
#endif
	}
	//std::cout << cnt << "\n";
	ans.push_back(cnt);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (query());
	for (const int& i : ans) std::cout << i << "\n";
	return;
}
int main() { solve(); return 0; }//boj4474 Rescue Beacon