#ifndef _GEOMETRY_SOURCE_
#define _GEOMETRY_SOURCE_


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>

namespace crimson231 {
	//#define _CRT_SECURE_NO_WARNINGS
	//#include <iostream>
	//#include <algorithm>
	//#include <cstring>
	//#include <cassert>
	//#include <cmath>
	//#include <vector>
	//#include <queue>
	//#include <deque>
	//#include <random>
	//#include <array>
	//#include <tuple>
	//#include <complex>

	typedef long long ll;
	typedef long double ld;
	//typedef double ld;

	const ld INF = 1e17;
	const ld PI = acos(-1);
	const ld TOL = 1e-7;
	bool zero(const ld& x) { return std::abs(x) < TOL; }
	int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
	int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
	ld norm(ld th) {
		while (th < -TOL) th += PI * 2;
		while (th > PI * 2) th -= PI * 2;
		return th;
	}
	ld flip(ld lat) {
		if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
		if (zero(lat)) return PI * .5;
		if (lat > 0) return PI * .5 - lat;
		if (lat < 0) return -(PI * .5) - lat;
		return INF;
	}
	ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
	ll gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
	template<typename Tpos, typename Tret> class Geo2D {
		struct Pos {
			Tpos x, y;
			Pos(Tpos X = 0, Tpos Y = 0) : x(X), y(Y) {}
			bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
			bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
			bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
			Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
			Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
			Pos operator * (const Tpos& scalar) const { return { x * scalar, y * scalar }; }
			Pos operator / (const Tpos& scalar) const { return { x / scalar, y / scalar }; }
			Tret operator * (const Pos& p) const { return { (Tret)x * p.x + (Tret)y * p.y }; }
			Tret operator / (const Pos& p) const { return { (Tret)x * p.y - (Tret)y * p.x }; }
			Pos operator ~ () const { return { -y, x }; }
			Tret operator ! () const { return x * y; }
			Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
			Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
			Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
			Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
			Tret Euc() const { return (Tret)x * x + (Tret)y * y; }
			Tret Man() const { return std::abs(x) + std::abs(y); }
			ld mag() const { return hypot(x, y); }
			Pos unit() const { return *this / mag(); }
			friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
			friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
		}; const Pos<Tpos, Tret> O = { 0, 0 };
		struct Vec {
			Tpos vy, vx;
			Vec(Tpos Y = 0, Tpos X = 0) : vy(Y), vx(X) {}
			bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
			bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
			Tret operator * (const Vec& v) const { return (Tret)vy * v.vy + (Tret)vx * v.vx; }
			Tret operator / (const Vec& v) const { return (Tret)vy * v.vx - (Tret)vx * v.vy; }
			Vec operator ~ () const { return { -vx, vy }; }
			Vec& operator *= (const Tpos& scalar) { vy *= scalar; vx *= scalar; return *this; }
			Vec& operator /= (const Tpos& scalar) { vy /= scalar; vx /= scalar; return *this; }
			ld mag() const { return hypot(vy, vx); }
		}; const Vec Zero = { 0, 0 };
		struct Line {//ax + by = c
			Vec s;
			Tpos c;
			Line(Vec V = Vec(0, 0), Tpos C = 0) : s(V), c(C) {}
			bool operator < (const Line& l) const {
				bool f1 = Zero < s;
				bool f2 = Zero < l.s;
				if (f1 != f2) return f1;
				Tret CCW = s / l.s;
				return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
			}
			ld operator * (const Line& l) const { return s * l.s; }
			ld operator / (const Line& l) const { return s / l.s; }
			Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
			Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
			Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
			Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
			Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
			Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
			ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
			ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
			ld mag() const { return s.mag(); }
			friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
		};
	};

}



#endif // !_GEOMETRY_SOURCE_