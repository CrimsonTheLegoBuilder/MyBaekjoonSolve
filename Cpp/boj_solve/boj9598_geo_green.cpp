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
const ld TOL = 1e-8;
const ld PI = acos(-1);
const int LEN = 505;
int N, M, T, Q;
ld A, MIN;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ll sq(int x) { return (ll)x * x; }
inline ld sqr(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

struct Pii {
	int x, y;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const int& n) const { return { x * n, y * n }; }
	Pii operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pii operator - () const { return { -x, -y }; }
	Pii operator ~ () const { return { -y, x }; }
	Pii operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	inline ld rad() const { return norm(atan2(y, x)); }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	inline friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (int)INF, (int)INF };
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
inline ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
inline ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
inline int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	inline Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	inline ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	inline ld rad() const { return norm(atan2(y, x)); }
	inline friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
struct Circle {
	Pii c;
	int r;
	Circle(Pii C = Pii(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	inline bool operator < (const Circle& q) const {
		ll rsub = sq(r - q.r);
		return r < q.r && rsub >= (c - q.c).Euc();
	}
	inline bool operator ^ (const Circle& q) const {
		ll rsum = sq(r + q.r);
		return rsum <= (c - q.c).Euc();
	}
	bool operator > (const Pii& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pii& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pii& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	inline ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	//int i;
	Arc(ld LO = 0, ld HI = 0) : lo(LO), hi(HI) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline ld area(const Circle& cen) const { return (hi - lo) * cen.r * cen.r; }
	inline ld green(const Circle& cen) const {
		Pos LO = -Pos(1, 0).rot(lo) * cen.r;
		Pos HI = Pos(1, 0).rot(hi) * cen.r;
		Pos vec = Pos(cen.c.x, cen.c.y);
		return (area(cen) + vec / (HI + LO)) * .5;
	}
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi; return os; }
};
typedef std::vector<Arc> Arcs;
bool VO[LEN], VI[LEN];
inline std::vector<Pos> intersection(const Circle& a, const Circle& b) {
	Pii ca = a.c, cb = b.c;
	Pii vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();

	if (vec.Euc() >= sq(ra + rb)) return {};
	if (vec.Euc() <= sq(ra - rb)) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	if (zero(h)) return {};
	return { Pos(norm(rd - h), norm(rd + h)) };
}
inline ld union_area(std::vector<Circle>& OC, std::vector<Circle>& IC) {
	memset(VO, 0, sizeof VO);
	memset(VI, 0, sizeof VI);
	int sz = OC.size();
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			if (i == j) continue;
			if (i < j && OC[i] == OC[j]) VO[j] = 1;
			if (i < j && IC[i] == IC[j]) VI[j] = 1;
			if (OC[i] == IC[j]) VO[i] = 1, VI[j] = 1;
			if (OC[j] == IC[i]) VO[j] = 1, VI[i] = 1;
			if (IC[j] < OC[i] && OC[i] < OC[j]) VO[i] = 1;
			if (IC[j] < IC[i] && IC[i] < OC[j]) VI[i] = 1;
			if (IC[i] < OC[j] && OC[j] < OC[i]) VO[j] = 1;
			if (IC[i] < IC[j] && IC[j] < OC[i]) VI[j] = 1;
			if (IC[j] ^ OC[i] && OC[i] < OC[j]) VO[i] = 1;
			if (IC[j] ^ IC[i] && IC[i] < OC[j]) VI[i] = 1;
			if (IC[i] ^ OC[j] && OC[j] < OC[i]) VO[j] = 1;
			if (IC[i] ^ IC[j] && IC[j] < OC[i]) VI[j] = 1;
		}
	}
#ifdef DEBUG
	for (int i = 0; i < sz; i++)
		std::cout << "V[" << i << "] : " << VO[i] << " " << VI[i] << "\n";
#endif
	ld union_area_ = 0;
	for (int i = 0; i < sz; i++) {
		if (VO[i] && VI[i]) continue;//OUT && IN both swallowed
		Arcs VAO;
		Arcs VAI;
		for (int j = 0; j < sz; j++) {
			if (j == i) continue;
			if (!VO[i]) {
				auto inx1 = intersection(OC[i], OC[j]);
				auto inx2 = intersection(OC[i], IC[j]);
				if (inx1.size() || inx2.size()) {
					if (inx1.size() && inx2.size()) {//O && I
						ld loo = inx1[0].x;
						ld hio = inx1[0].y;
						ld loi = inx2[0].x;
						ld hii = inx2[0].y;

						Arc a1, a2;
						if (loo > loi) {
							a1 = Arc(loo, 2 * PI);
							a2 = Arc(0, loi);
							VAO.push_back(a1);
							VAO.push_back(a2);
						}
						else {
							a1 = Arc(loo, loi);
							VAO.push_back(a1);
						}

						if (hii > hio) {
							a1 = Arc(hii, 2 * PI);
							a2 = Arc(0, hio);
							VAO.push_back(a1);
							VAO.push_back(a2);
						}
						else {
							a1 = Arc(hii, hio);
							VAO.push_back(a1);
						}
					}//O && I
					else if (inx1.size()) {//O
						ld lo = inx1[0].x;
						ld hi = inx1[0].y;

						Arc a1, a2;
						if (lo > hi) {
							a1 = Arc(lo, 2 * PI);
							a2 = Arc(0, hi);
							VAO.push_back(a1);
							VAO.push_back(a2);
						}
						else {
							a1 = Arc(lo, hi);
							VAO.push_back(a1);
						}
					}//O
					else if (inx2.size()) {//I
						ld lo = inx2[0].x;
						ld hi = inx2[0].y;

						Arc a1, a2;
						if (hi > lo) {
							a1 = Arc(hi, 2 * PI);
							a2 = Arc(0, lo);
							VAO.push_back(a1);
							VAO.push_back(a2);
						}
						else {
							a1 = Arc(hi, lo);
							VAO.push_back(a1);
						}
					}//I
				}
			}//OC

			if (!VI[i] && IC[i].r > 0) {
				auto inx3 = intersection(IC[i], OC[j]);
				auto inx4 = intersection(IC[i], IC[j]);
				if (inx3.size() || inx4.size()) {
					if (inx3.size() && inx4.size()) {//O && I
						ld loo = inx3[0].x;
						ld hio = inx3[0].y;
						ld loi = inx4[0].x;
						ld hii = inx4[0].y;

						Arc a1, a2;
						if (loo > loi) {
							a1 = Arc(loo, 2 * PI);
							a2 = Arc(0, loi);
							VAI.push_back(a1);
							VAI.push_back(a2);
						}
						else {
							a1 = Arc(loo, loi);
							VAI.push_back(a1);
						}

						if (hii > hio) {
							a1 = Arc(hii, 2 * PI);
							a2 = Arc(0, hio);
							VAI.push_back(a1);
							VAI.push_back(a2);
						}
						else {
							a1 = Arc(hii, hio);
							VAI.push_back(a1);
						}
					}//O && I
					else if (inx3.size()) {//O
						ld lo = inx3[0].x;
						ld hi = inx3[0].y;

						Arc a1, a2;
						if (lo > hi) {
							a1 = Arc(lo, 2 * PI);
							a2 = Arc(0, hi);
							VAI.push_back(a1);
							VAI.push_back(a2);
						}
						else {
							a1 = Arc(lo, hi);
							VAI.push_back(a1);
						}
					}//O
					else if (inx4.size()) {//I
						ld lo = inx4[0].x;
						ld hi = inx4[0].y;

						Arc a1, a2;
						if (hi > lo) {
							a1 = Arc(hi, 2 * PI);
							a2 = Arc(0, lo);
							VAI.push_back(a1);
							VAI.push_back(a2);
						}
						else {
							a1 = Arc(hi, lo);
							VAI.push_back(a1);
						}
					}//I
				}
			}//IC
		}


		ld hi;
		if (!VO[i]) {
			std::sort(VAO.begin(), VAO.end());
			VAO.push_back(Arc(2 * PI, 2 * PI));
			hi = 0;
			//std::cout << "DEBUG::" << i << "\n";
			for (const Arc& a : VAO) {
				if (a.lo > hi) union_area_ += Arc(hi, a.lo).green(OC[i]), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
		if (!VI[i] && IC[i].r > 0) {
			std::sort(VAI.begin(), VAI.end());
			VAI.push_back(Arc(2 * PI, 2 * PI));
			hi = 0;
			//std::cout << "DEBUG::" << i << "\n";
			for (const Arc& a : VAI) {
				if (a.lo > hi) union_area_ -= Arc(hi, a.lo).green(IC[i]), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
	}
	return union_area_;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> T;
	M = 0;
	while (T--) {
		std::cin >> N;
		Disks OC(N), IC(N);
		int d;
		for (int i = 0; i < N; i++) {
			Circle c;
			std::cin >> c >> d;
			OC[i] = Circle(c.c, c.r + d);
			IC[i] = Circle(c.c, std::max(0, c.r - d));
		}
		std::cout << "Case " << ++M << ": ";
		std::cout << union_area(OC, IC) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj17804

/*

2
1
0 0 10 1
2
0 0 7 1
13 0 10 1

Case 1: 125.6637061
Case 2: 205.4749312

1
3
0 0 1 1
0 0 3 1
0 0 5 1

Case 1: 113.0973355

2
6
0 0 1 1
0 0 3 1
0 0 5 1
6 0 1 1
6 0 3 1
6 0 5 1
2
0 0 3 3
6 0 3 3

Case 1: 181.9733619
Case 2: 181.9733619

2
4
0 0 3 1
0 0 5 1
6 0 3 1
6 0 5 1
2
0 0 4 2
6 0 4 2

Case 1: 168.5156109
Case 2: 168.5156109

1
6
0 0 1 1
0 0 4 1
0 0 7 1
0 0 10 1
0 0 13 1
0 0 16 1

Case 1: 640.8849013

*/