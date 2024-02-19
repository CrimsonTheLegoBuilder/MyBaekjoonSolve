#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
int N, M, T, Q;

bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pt3 {
	ld x, y, z;
	Pt3(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	ld operator * (const Pt3& p) const { return x * p.x + y * p.y + z * p.z; }
	Pt3 operator / (const Pt3& p) const {
		Pt3 ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pt3 operator + (const Pt3& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pt3 operator - (const Pt3& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pt3 operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pt3& operator += (const Pt3& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pt3& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	ld mag() const { return sqrtl(x * x + y * y + z * z); }
};
struct Ln3 {
	Pt3 dir, p0;
	Ln3(Pt3 DIR = Pt3(0, 0, 0), Pt3 P0 = Pt3(0, 0, 0)) : dir(DIR), p0(P0) {}
};
Ln3 L(const Pt3& p1, const Pt3& p2) { return { p2 - p1, p1 }; }
struct Plane{
	Pt3 norm, p0;
	Plane(Pt3 NORM = Pt3(0, 0, 0), Pt3 P0 = Pt3(0, 0, 0)) : norm(NORM), p0(P0) {}
};
Plane P(const Pt3& p1, const Pt3& p2, const Pt3& p3) {
	Pt3 norm = (p2 - p1) / (p3 - p2);
	return Plane(norm, p1);
}
Pt3 intersection(const Plane& S, const Ln3& l) {
	ld det = S.norm * l.dir;
	if (z(det)) return { INF, INF, INF };
	ld t = S.norm * S.p0 - S.norm * l.p0;

}