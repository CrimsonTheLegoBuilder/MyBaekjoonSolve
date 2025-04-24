#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

//https://cpp.mazurok.com/e-olymp-8528/

int main() {
    double eps = 1e-6;
    double x1, y1, x2, y2, x3, y3, r1, r2, r3, x, y;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> r1 >> r2 >> r3;
    x = ((y2 - y1) * (r2 * r2 - r3 * r3 - y2 * y2 + y3 * y3 - x2 * x2 + x3 * x3) - (y3 - y2) * (r1 * r1 - r2 * r2 - y1 * y1 + y2 * y2 - x1 * x1 + x2 * x2)) / (2 * ((y3 - y2) * (x1 - x2) - (y2 - y1) * (x2 - x3)));
    y = ((x2 - x1) * (r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3) - (x3 - x2) * (r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2)) / (2 * ((x3 - x2) * (y1 - y2) - (x2 - x1) * (y2 - y3)));
    if (abs((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) - r1 * r1) < eps) cout << fixed << setprecision(6) << x << " " << y << endl;
    else cout << "Impossible" << endl;
    return 0;
}

/*

#include <iostream>
#include <algorithm>
#include <cmath>
typedef double ld;
const ld TOL = 1e-6;
bool zero(const ld& x) { return std::abs(x) < TOL; }
//https://cpp.mazurok.com/e-olymp-8528/

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ld Euc() const { return x * x + y * y; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n"; return os;
	}
};
void solve() {
	std::cout << std::fixed;
	std::cout.precision(6);
	ld x1, y1, x2, y2, x3, y3, r1, r2, r3, x, y;
	std::cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> r1 >> r2 >> r3;
	x = ((y2 - y1) * (r2 * r2 - r3 * r3 - y2 * y2 + y3 * y3 - x2 * x2 + x3 * x3) -
		(y3 - y2) * (r1 * r1 - r2 * r2 - y1 * y1 + y2 * y2 - x1 * x1 + x2 * x2)) /
		(2 * ((y3 - y2) * (x1 - x2) - (y2 - y1) * (x2 - x3)));
	y = ((x2 - x1) * (r2 * r2 - r3 * r3 - x2 * x2 + x3 * x3 - y2 * y2 + y3 * y3) -
		(x3 - x2) * (r1 * r1 - r2 * r2 - x1 * x1 + x2 * x2 - y1 * y1 + y2 * y2)) /
		(2 * ((x3 - x2) * (y1 - y2) - (x2 - x1) * (y2 - y3)));
	Pos p1 = Pos(x1, y1), g = Pos(x, y);
	if (zero((p1 - g).Euc() - r1 * r1)) std::cout << g;
	else std::cout << "Impossible\n";
}
int main() { solve(); return 0; }//boj29566 ¬³¬Ú¬ã¬ä¬Ö¬Þ¬Ñ ¬Ô¬Ý¬à¬Ò¬Ñ¬Ý¬î¬ß¬Ö¬Û¬ê¬Ö¬Ô¬à ¬á¬à¬Ù¬Ú¬è¬Ú¬à¬ß¬Ú¬â¬à¬Ó¬Ñ¬ß¬Ú¬ñ
//ssibal budongsosujum

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <vector>
//typedef long long ll;
//typedef long double ld;
////typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-9;
//const ld PI = acos(-1);
//const int LEN = 1e3;
//int N, M, T, Q;
//bool V[LEN + 1][LEN + 1];
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ld norm(ld& th) {
//	while (th < -TOL) th += PI * 2;
//	while (th > PI * 2) th -= PI * 2;
//	return th;
//}
//
//struct Pos {
//	ld x, y;
//	int i;
//	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
//	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar, 0 }; }
//	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar, 0 }; }
//	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x, 0 }; }
//	ld operator ! () const { return x * y; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y << "\n"; return os;
//	}
//}; const Pos O = { 0, 0, -1 };
//struct Circle {
//	Pos c;
//	ld R;
//	Circle(Pos C = Pos(0, 0, 0), ld r = 0) : c(C), R(r) {}
//	bool operator == (const Circle& p) const { return c == p.c && std::abs(R - p.R) < TOL; }
//	friend std::istream& operator >> (std::istream& is, Circle& c) {
//		is >> c.c.x >> c.c.y >> c.R;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Circle& c) {
//		os << c.c.x << " " << c.c.y << " " << c.R << "\n"; return os;
//	}
//};
//std::vector<Pos> get_node(const Circle& a, const Circle& b) {
//	Pos ca = a.c, cb = b.c;
//	Pos vec = cb - ca;
//	ld ra = a.R, rb = b.R;
//	ld distance = vec.mag();
//
//	if (distance > ra + rb + TOL) return {};
//	if (distance < std::abs(ra - rb) - TOL) return {};
//
//	//2nd hyprblc law of cos
//	ld X = (ra * ra - rb * rb + distance * distance) / (2 * distance);
//	ld Y = sqrt(ra * ra - X * X);
//	Pos m = ca + vec * (X / distance);
//	if (Y > 0) {
//		ld ratio = Y / distance;
//		return { m + ~vec * ratio, m - ~vec * ratio };
//	}
//	else return { m };
//}
//void solve() {
//	std::cout << std::fixed;
//	std::cout.precision(6);
//	Circle a, b, c;
//	std::cin >> a.c.x >> a.c.y >> b.c.x >> b.c.y >> c.c.x >> c.c.y >> a.R >> b.R >> c.R;
//	a.c *= 10000;
//	b.c *= 10000;
//	c.c *= 10000;
//	a.R *= 10000;
//	b.R *= 10000;
//	c.R *= 10000;
//	std::vector<Pos> AB = get_node(a, b);
//	std::vector<Pos> BC = get_node(b, c);
//	std::vector<Pos> CA = get_node(c, a);
//	int absz = AB.size();
//	//std::cout << absz << "\n";
//	if (!absz) {
//		//if (BC.size() == 1 && CA.size() == 0 && (BC[0] - a.c).mag() < a.R + TOL) { std::cout << BC[0].x << " " << BC[0].y << "\n"; return; }
//		//if (CA.size() == 1 && BC.size() == 0 && (CA[0] - b.c).mag() < b.R + TOL) { std::cout << CA[0].x << " " << CA[0].y << "\n"; return; }
//		//else { std::cout << "Impossible\n"; return; }
//		std::cout << "Impossible\n"; return;
//	}
//	if (absz == 1) {
//		//if (BC.size() == 0 && CA.size() == 0 && (AB[0] - c.c).mag() < c.R + TOL) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		if (zero((AB[0] - c.c).Euc() - c.R * c.R)) { std::cout << AB[0].x / 10000 << " " << AB[0].y / 10000 << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	if (absz == 2) {
//		if (zero((AB[0] - c.c).Euc() - c.R * c.R) && zero((AB[1] - c.c).mag() - c.R)) { std::cout << "Impossible\n"; return; }
//		if (zero((AB[0] - c.c).Euc() - c.R * c.R)) { std::cout << AB[0].x / 10000 << " " << AB[0].y / 10000 << "\n"; return; }
//		else if (zero((AB[1] - c.c).Euc() - c.R * c.R)) { std::cout << AB[1].x / 10000 << " " << AB[1].y / 10000 << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	return;
//}
//int main() { solve(); return 0; }

//void solve() {
//	std::cout << std::fixed;
//	std::cout.precision(6);
//	Circle a, b, c;
//	std::cin >> a.c.x >> a.c.y >> b.c.x >> b.c.y >> c.c.x >> c.c.y >> a.R >> b.R >> c.R;
//	std::vector<Pos> AB = find_inx(a, b);
//	std::vector<Pos> BC = find_inx(b, c);
//	std::vector<Pos> CA = find_inx(c, a);
//	int absz = AB.size();
//	//std::cout << absz << "\n";
//	if (!absz) {
//		if (BC.size() == 1 && CA.size() == 0 && (BC[0] - a.c).mag() < a.R + TOL) { std::cout << BC[0].x << " " << BC[0].y << "\n"; return; }
//		if (CA.size() == 1 && BC.size() == 0 && (CA[0] - b.c).mag() < b.R + TOL) { std::cout << CA[0].x << " " << CA[0].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//		//std::cout << "Impossible\n"; return;
//	}
//	if (absz == 1) {
//		if (BC.size() == 0 && CA.size() == 0 && (AB[0] - c.c).mag() < c.R + TOL) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		if (zero((AB[0] - c.c).mag() - c.R)) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	if (absz == 2) {
//		if (zero((AB[0] - c.c).mag() - c.R) && zero((AB[1] - c.c).mag() - c.R)) { std::cout << "Impossible\n"; return; }
//		if (zero((AB[0] - c.c).mag() - c.R)) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		if (zero((AB[1] - c.c).mag() - c.R)) { std::cout << AB[1].x << " " << AB[1].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	return;
//}
//int main() { solve(); return 0; }

//void solve() {
//	std::cout << std::fixed;
//	std::cout.precision(6);
//	Circle a, b, c;
//	std::cin >> a.c.x >> a.c.y >> b.c.x >> b.c.y >> c.c.x >> c.c.y >> a.R >> b.R >> c.R;
//	std::vector<Pos> AB = find_inx(a, b);
//	std::vector<Pos> BC = find_inx(b, c);
//	std::vector<Pos> CA = find_inx(c, a);
//	int absz = AB.size();
//	std::cout << absz << "\n";
//	if (!absz) {
//		if (BC.size() == 1 && (BC[0] - a.c).mag() < a.R + TOL) { std::cout << BC[0].x << " " << BC[0].y << "\n"; return; }
//		if (CA.size() == 1 && (CA[0] - b.c).mag() < b.R + TOL) { std::cout << CA[0].x << " " << CA[0].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	if (absz == 1) {
//		if (BC.size() == 0 && CA.size() == 0 && (AB[0] - c.c).mag() < c.R + TOL) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		//if (zero((AB[0] - c.c).mag() - c.R)) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	if (absz == 2) {
//		if (zero((AB[0] - c.c).mag() - c.R)) { std::cout << AB[0].x << " " << AB[0].y << "\n"; return; }
//		if (zero((AB[1] - c.c).mag() - c.R)) { std::cout << AB[1].x << " " << AB[1].y << "\n"; return; }
//		else { std::cout << "Impossible\n"; return; }
//	}
//	return;
//}
//int main() { solve(); return 0; }

*/