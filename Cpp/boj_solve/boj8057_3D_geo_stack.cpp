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
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acosl(-1);
const int LEN = 5005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

int N;
struct Pos {
	int x, y;
	ll c;
	Pos(int X = 0, int Y = 0, ll C = 0) : x(X), y(Y), c(C) {}
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
struct Pos3D {
	int x, y, z;
	Pos3D(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {}
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
	Pos3D operator * (const int& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const int& n) const { return { x / n, y / n, z / n }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const int& n) { x *= n; y *= n; z *= n; return *this; }
	Pos3D& operator /= (const int& n) { x /= n; y /= n; z /= n; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
} P[LEN];
typedef std::vector<Pos3D> Polygon3D;
typedef std::vector<Polygon3D> Polyhedron;
ll largest_volume(const Pos& p, Polygon& V, const int& h, int& x, int& y) {
	if (V.empty()) return 0;
	ll ret = std::abs((ll)p.x * p.y);
	std::sort(V.begin(), V.end());
	Polygon S;
	for (const Pos& q : V) {
		if (S.size() && S.back().x == q.x) continue;
		while (S.size() && S.back().y >= q.y) S.pop_back();
		S.push_back(q);
	}
	int sz = S.size();
	for (int i = 0; i < sz - 1; i++) {
		const Pos& q1 = S[i], & q2 = S[i + 1];
		if (ret < std::abs((ll)q1.x * q2.y)) {
			x = std::abs(q1.x);
			y = std::abs(q2.y);
			ret = (ll)x * y;
		}
	}
	return ret * h;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout << std::fixed;
	std::cin >> N;
	ll vol = 0;
	Pos3D Q = Pos3D(0, 0, 0);
	for (int i = 0; i < N; i++) std::cin >> P[i];
	P[N++] = Pos3D(1000000, 0, 0);
	P[N++] = Pos3D(0, 1000000, 0);
	P[N++] = Pos3D(0, 0, 1000000);
	for (int i = 0; i < N; i++) {
		const Pos3D& p = P[i];
		Polygon V; Pos q;
		int x = 0, y = 0, z = 0;
		ll v;
		x = p.x;
		q = Pos(-p.y, p.z);
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			if (p.x > P[j].x) {
				if (p.y > P[j].y && p.z > P[j].z) { V.clear(); break; }
				V.push_back(Pos(-P[j].y, P[j].z));
			}
		}
		v = largest_volume(q, V, x, y, z);
		if (v > vol) vol = v, Q = Pos3D(x, y, z);
		V.clear();
		y = p.y;
		q = Pos(-p.z, p.x);
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			if (p.y > P[j].y) {
				if (p.z > P[j].z && p.x > P[j].x) { V.clear(); break; }
				V.push_back(Pos(-P[j].z, P[j].x));
			}
		}
		v = largest_volume(q, V, y, z, x);
		if (v > vol) vol = v, Q = Pos3D(x, y, z);
		V.clear();
		z = p.z;
		q = Pos(-p.x, p.y);
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			if (p.z > P[j].z) {
				if (p.x > P[j].x && p.y > P[j].y) { V.clear(); break; }
				V.push_back(Pos(-P[j].x, P[j].y));
			}
		}
		v = largest_volume(q, V, z, x, y);
		if (v > vol) vol = v, Q = Pos3D(x, y, z);
	}
	std::cout << Q << "\n";
	return;
}
int main() { solve(); return 0; }//boj8057