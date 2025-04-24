#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }

struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3D& operator /= (const ld& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	ld lon() const { return atan2(y, x); }
	ld lat() const { return atan2(z, sqrtl(x * x + y * y)); }
	Pos3D unit() const { return *this / mag(); }
	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
	Pos3D rotate(const ld& th, const Pos3D& axis) const {
		ld SIN = sin(th), COS = cos(th);
		Pos3D u = axis.unit();
		return u * (*this * u) * (1 - COS) + (*this * COS) - (*this / u) * SIN;
	}
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }
};
struct Sphere {
	Pos3D c;
	ld r;
	Sphere(Pos3D C = Pos3D(0, 0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Sphere& S) const { return c == S.c && std::abs(r - S.r) < TOL; }
	bool operator != (const Sphere& S) const { return !(*this == S); }
	bool operator > (const Pos3D& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos3D& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos3D& p) const { return r < (c - p).mag(); }
	Sphere operator + (const Sphere& S) const { return { c + S.c, r + S.r }; }
	Sphere operator - (const Sphere& S) const { return { c - S.c, r - S.r }; }
	ld A() const { return r * r * PI; }
	ld S() const { return r * r * PI * 4; }
	ld V() const { return r * r * r * PI * 4 / 3; }
	friend std::istream& operator >> (std::istream& is, Sphere& S) { is >> S.c.x >> S.c.y >> S.c.z >> S.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Sphere& S) { os << S.c.x << " " << S.c.y << " " << S.c.z << " " << S.r; return os; }
};
Sphere minimum_enclose_Sphere(const std::vector<Pos3D>& C) {
	Pos3D center = Pos3D(0, 0, 0);
	int sz = C.size();
	for (const Pos3D& p : C) center += p;
	center /= (ld)sz;
	ld ratio = 0.1, desc = 0.998, R = -1;
	int cnt = 10000;
	while (cnt--) {
		int i = -1;
		R = -1;
		for (int j = 0; j < N; j++) {
			ld d = (center - C[j]).Euc();
			if (R < d) i = j, R = d;
		}
		center += (C[i] - center) * ratio;
		ratio *= desc;
	}
	return Sphere(center, sqrt(R));
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> N;
	std::vector<Pos3D> C3D(N);
	for (Pos3D& p : C3D) std::cin >> p;
	std::cout << minimum_enclose_Sphere(C3D).r << "\n";
	return;
}
int main() { solve(); return 0; }//boj11930 Smallest Enclosing Sphere


//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(2);
//	std::cin >> N;
//	std::vector<Pos3D> C3D(N);
//	Pos3D center = Pos3D(0, 0, 0);
//	for (Pos3D& p : C3D) std::cin >> p, center += p;
//	center /= (ld)N;
//	ld ratio = 0.1, R = 0;
//	int cnt = 20000;
//	while (cnt--) {
//		int i = -1;
//		R = 0;
//		for (int j = 0; j < N; j++) {
//			ld d = (center - C3D[j]).Euc();
//			if (R < d) i = j, R = d;
//		}
//		center += (C3D[i] - center) * ratio;
//		ratio *= 0.999;
//	}
//	std::cout << sqrt(R) << "\n";
//	return;
//}
//const Pos3D O3D = { 0, 0, 0 };
//const Pos3D X_axis = { 1, 0, 0 };
//const Pos3D Y_axis = { 0, 1, 0 };
//const Pos3D Z_axis = { 0, 0, 1 };
//const Pos3D MAXP3D = { INF, INF, INF };
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
//int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
//	Pos3D CCW = cross(d1, d2, d3);
//	ld ret = CCW * norm;
//	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//struct Line3D {
//	Pos3D dir, p0;
//	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
//};
