#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
typedef long double ld;
typedef std::pair<int, int> pi;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
struct Info { ll area, l, r; };

//struct Pos3D {
//	ll x, y, z;
//	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
//	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
//	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
//	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
//	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
//	Pos3D operator / (const Pos3D& p) const {
//		Pos3D ret;
//		ret.x = y * p.z - z * p.y;
//		ret.y = z * p.x - x * p.z;
//		ret.z = x * p.y - y * p.x;
//		return ret;
//	}
//	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
//	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
//	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
//	Pos3D operator / (const ll& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
//	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
//	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
//	Pos3D& operator *= (const ll& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
//	Pos3D& operator /= (const ll& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
//	ll Euc() const { return x * x + y * y + z * z; }
//	ld mag() const { return sqrtl(Euc()); }
//	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
//		is >> p.x >> p.y >> p.z;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
//		os << p.x << " " << p.y << " " << p.z << "\n";
//		return os;
//	}
//} candi[LEN], willy, MAXP{ INF, INF, INF };
//const Pos3D O3D = { 0, 0, 0 };
//using Face = std::array<int, 3>;
//std::vector<Face> Hull3D;
//struct Edge {
//	int face_num, edge_num;
//	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
//};
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& d4) { return (d2 - d1) * (d4 - d3); }
//bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
//	ll ret = dot(d1, d3, d2);
//	return !cross(d1, d2, d3).mag() && ret >= 0;
//}
//bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
//	ll ret = dot(d1, d3, d2);
//	return !cross(d1, d2, d3).mag() && ret > 0;
//}
//int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
//	Pos3D CCW = cross(d1, d2, d3);
//	std::cout << CCW;
//	ll ret = CCW * norm;
//	std::cout << "ret: " << ret << "\n";
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//ld area(const std::vector<Pos3D>& H, const Pos3D& norm) {
//	ll ret = 0;
//	if (H.size() < 3) return ret;
//	Pos3D O = H[0];
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
//		ret += cross(O, cur, nxt) * norm / norm.Euc();
//	}
//	return std::abs(ret * .5);
//}
//bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
//	return cross(a, b, c) * (p - a) > 0;
//}

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
	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
		is >> p.x >> p.y >> p.z;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
		os << p.x << " " << p.y << " " << p.z << "\n";
		return os;
	}
};
const Pos3D O3D = { 0, 0, 0 };
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
int main() {
	std::cout << std::fixed;
	std::cout.precision(15);
	Pos3D a, b, c;
	std::cin >> a >> b >> c;
	std::cout << "above: " << above(a, b, c, O3D) << "\n";
	Pos3D n = cross(a, b, c);
	int x = ccw(a, b, c, n);
	std::cout << x << "\n";
	std::cout << (a - b);
	std::cout << (b - c);
	std::cout << "ccw a b: " << ccw(a, b, O3D, n) << "\n";
	std::cout << "ccw b c: " << ccw(b, c, O3D, n) << "\n";
	std::cout << "ccw c a: " << ccw(c, a, O3D, n) << "\n";
	std::cout << x << "\n";
	std::cout << n.Euc() << "\n";
	std::cout << n;
	std::cout << n.mag() << "\n";
	std::cout << n * (O3D - a) << "\n";
}

/*

1 3
-280000 -450000 0
-437600 259800 148400
275680 452688 1484

9223372036854775807
9011369165176521728
312533749465952362168320

*/