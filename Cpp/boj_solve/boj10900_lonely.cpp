#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
//typedef long double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypotl(x, y); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
typedef std::vector<Circle> Disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	Circle cen;
	Arc(ld LO = 0, ld HI = 0, Circle CEN = Circle(Pos(0, 0), 0)) : lo(LO), hi(HI), cen(CEN) {}
	bool operator < (const Arc& a) const { return !sign(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	ld area() const { return hi - lo * cen.r * cen.r; }
	ld green() const {
		Pos LO = -Pos(1, 0).rot(lo) * cen.r;
		Pos HI = Pos(1, 0).rot(hi) * cen.r;
		return (area() + cen.c / (HI + LO)) * .5;
	}
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.cen.r; return os; }
};
typedef std::vector<Arc> Arcs;
std::vector<Pos> intersection(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ld ra = a.r, rb = b.r;
	ld distance = vec.mag();

	if (distance > ra + rb + TOL) return {};
	if (distance < std::abs(ra - rb) - TOL) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
	if (X < -ra + TOL || X > ra - TOL) return {};
	ld ratio = sqrt(ra * ra - X * X);
	Pos w = vec * X / distance;
	Pos h = ~vec.unit() * ratio;
	if (zero(ratio)) return { w };
	return { w - h, w + h };
}
ld union_except_x(const int& x, std::vector<Circle>& VC) {
	ld union_area = 0;
	int sz = VC.size();
	for (int i = 0; i < sz; i++) {
		if (i == x) continue;
		Circle& disk = VC[i];
		Arcs arcs;
		for (int j = 0; j < sz; j++) {
			if (j == x || j == i) continue;
			Pos& ca = VC[i].c, cb = VC[j].c;
			//auto inx = intersection(VC[i], VC[j]);
			////std::cout << inx.size() << "\n";
			//if (inx.size() < 2) continue;
			//ld lo = inx[0].rad();
			//ld hi = inx[1].rad();

			ll ra = VC[i].r, rb = VC[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			if (X < -ra + TOL || X > ra - TOL) continue;
			ld ratio = sqrt(ra * ra - X * X);
			Pos w = vec * X / distance;
			Pos h = ~vec.unit() * ratio;
			ld lo = (w - h).rad(), hi = (w + h).rad();

			Arc a1, a2;
			if (sign(hi - lo) >= 0) {
				a1 = Arc(lo, hi, disk);
				arcs.push_back(a1);
			}
			else {
				a1 = Arc(lo, PI * 2, disk);
				a2 = Arc(0, hi, disk);
				arcs.push_back(a1);
				arcs.push_back(a2);
			}
		}

		if (!arcs.size()) {
			//std::cout << "i : " << i << "\n";
			union_area += disk.A();
			continue;
		}

		std::sort(arcs.begin(), arcs.end());
		arcs.push_back(Arc(2 * PI, 2 * PI, disk));
		ld hi = 0;
		for (const Arc& a : arcs) {
			//std::cout << "arc[" << i << "] : " << a << "\n";
			if (a.lo > hi) union_area += Arc(hi, a.lo, disk).green(), hi = a.hi;
			else hi = std::max(hi, a.hi);
		}
	}
	return union_area;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);

	int ret = 0;
	std::cin >> N;
	std::vector<Circle> tmp(N);
	std::vector<bool> V(N, 0);
	for (Circle& c : tmp) std::cin >> c;
	for (int i = 0; i < N; i++) {//remove
		if (V[i]) continue;
		for (int j = 0; j < N; j++) {
			if (i < j && tmp[i] == tmp[j]) V[i] = 1;
			if (tmp[i] < tmp[j]) V[i] = 1;
			if (tmp[j] < tmp[i]) V[j] = 1;
		}
	}

	Disks VC;
	for (int i = 0; i < N; i++) {
		if (!V[i]) VC.push_back(tmp[i]);
		if (V[i]) ret++;
	}

	int sz = VC.size();
	//std::cout << "sz : " << sz << "\n";
	ld U = union_except_x(-1, VC);
	//std::cout << "U : " << U << "\n";
	for (int x = 0; x < sz; x++) {
		ld A = union_except_x(x, VC);
		//std::cout << "U : " << U << " A : " << A << "\n";
		ret += zero(U - A);//no-dabwon
	}
	std::cout << ret << "\n";
}
int main() { solve(); return 0; }//boj10900 lonely mdic

/*

3
3 0 4
-3 0 4
0 0 2

5
0 0 1
1 1 1
-1 1 1
-1 -1 1
1 -1 1

9
3 0 4
-3 0 4
0 0 2
9 0 4
6 0 2
15 0 4
12 0 2
21 0 4
18 0 2

5
1000 1000 1415
1000 -1000 1415
-1000 -1000 1415
-1000 1000 1415
0 0 1

5
1000 1000 1414
1000 -1000 1414
-1000 -1000 1414
-1000 1000 1414
0 0 1



*/


//ld union_except_x(const int& x, std::vector<Circle>& VC) {
//	ld union_area = 0;
//	int sz = VC.size();
//	for (int i = 0; i < sz; i++) {
//		if (i == x) continue;
//		Circle& disk = VC[i];
//		Arcs arcs;
//		for (int j = 0; j < sz; j++) {
//			if (j == x || j == i) continue;
//			Pos& ca = VC[i].c, cb = VC[j].c;
//			ll ra = VC[i].r, rb = VC[j].r;
//			Pos vec = cb - ca;//vec a -> b
//			ld distance = vec.mag();
//			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
//
//			if (X < -ra + TOL || X > ra - TOL) continue;
//
//			ld ratio = sqrt(ra * ra - X * X);
//			Pos w = vec * X / distance;
//			Pos h = ~vec.unit() * ratio;
//			ld lo = (w - h).rad(), hi = (w + h).rad();
//
//			Arc a1, a2;
//			if (sign(hi - lo) >= 0) {
//				a1 = Arc(lo, hi, disk);
//				arcs.push_back(a1);
//			}
//			else {
//				a1 = Arc(lo, PI * 2, disk);
//				a2 = Arc(0, hi, disk);
//				arcs.push_back(a1);
//				arcs.push_back(a2);
//			}
//		}
//
//		if (!arcs.size()) {
//			//std::cout << "i : " << i << "\n";
//			union_area += disk.A();
//			continue;
//		}
//
//		std::sort(arcs.begin(), arcs.end());
//		arcs.push_back(Arc(2 * PI, 2 * PI, disk));
//		ld hi = 0;
//		for (const Arc& a : arcs) {
//			//std::cout << "arc[" << i << "] : " << a << "\n";
//			if (a.lo > hi) union_area += Arc(hi, a.lo, disk).green(), hi = a.hi;
//			else hi = std::max(hi, a.hi);
//		}
//	}
//	return union_area;
//}