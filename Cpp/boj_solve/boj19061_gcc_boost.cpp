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
#include <numeric>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/float128.hpp>
using namespace boost::multiprecision;
typedef long long ll;
//typedef double ld;
typedef long double ld;
//typedef __float128 LD;
typedef cpp_dec_float_50 LD;
const LD INF = 1e17;
const LD TOL = 1e-15;
// const LD PI = acosq(-1);
const LD PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
LD A;
bool zero(const LD& x) { return std::abs(x) <= TOL; }
int sign(const LD& x) { return x < -TOL ? -1 : x > TOL; }
LD norm(LD th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

//#define DEBUG
//#define ASSERT

struct Pos {
	LD x, y;
	LD t;
	Pos(LD X = 0, LD Y = 0) : x(X), y(Y) { t = norm(atan2q(y, x)); }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const LD& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const LD& scalar) const { return { x / scalar, y / scalar }; }
	LD operator * (const Pos& p) const { return x * p.x + y * p.y; }
	LD operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const LD& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const LD& scale) { x /= scale; y /= scale; return *this; }
	LD xy() const { return x * y; }
	Pos rot(LD the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	LD Euc() const { return x * x + y * y; }
	//LD mag() const { return sqrt(Euc()); }
	ld mag() const { return hypotq(x, y); }
	//ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	LD rad() const { return norm(atan2q(y, x)); }
	//LD rad() const { return norm(atan2(y, x)); }
	friend LD rad(const Pos& p1, const Pos& p2) { return norm(atan2q(p1 / p2, p1 * p2)); }
	//friend LD rad(const Pos& p1, const Pos& p2) { return norm(atan2(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	//friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	//friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = { 0, 0 };
const Pos X_norm = { 1000, 0 };
typedef std::vector<Pos> Polygon;
Polygon H, V, HPI;//Hull, conVert, HalfPlaneIntersection
//bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
//bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
//bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
bool cmpt(const Pos& p, const Pos& q) { return p.t < q.t; }
struct Vec {
	LD vy, vx;
	Vec(LD Y = 0, LD X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	LD operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	LD operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const LD& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const LD& scalar) { vy /= scalar; vx /= scalar; return *this; }
	LD mag() const { return hypotq(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	LD c;
	Line(Vec V = Vec(0, 0), LD C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		LD CCW = s / l.s;
		return zero(CCW) ? c * hypotq(l.s.vy, l.s.vx) < l.c * hypotq(s.vy, s.vx) : CCW > 0;
	}
	LD operator * (const Line& l) const { return s * l.s; }
	LD operator / (const Line& l) const { return s / l.s; }
	Line operator + (const LD& scalar) const { return Line(s, c + hypotq(s.vy, s.vx) * scalar); }
	Line operator - (const LD& scalar) const { return Line(s, c - hypotq(s.vy, s.vx) * scalar); }
	Line operator * (const LD& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const LD& scalar) { c += hypotq(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const LD& scalar) { c -= hypotq(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const LD& scalar) { s *= scalar, c *= scalar; return *this; }
	LD dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	LD above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	LD mag() const { return s.mag(); }
	//friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
LD cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
LD cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	LD ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
LD dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
LD dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	LD ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	LD ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
LD projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
LD dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	LD a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
//ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
Line L(const Pos& s, const Pos& e) {
	LD dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
Line L(const Vec& s, const Pos& p) {
	LD c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	LD det = v1 / v2;
	return Pos(
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	);
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
		if (l1.s / l2.s < TOL) return 0;
		Pos p = intersection(l1, l2);
		//return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
		return target.above(p) > -TOL;
		};
	std::deque<Line> dq;
	std::sort(HP.begin(), HP.end());
	for (const Line& l : HP) {
		if (!dq.empty() && zero(dq.back() / l)) continue;
		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
		dq.push_back(l);
	}
	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
	for (int i = 0; i < dq.size(); i++) {
		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
		if (cur / nxt < TOL) {
			hull.clear();
			return 0;
		}
		hull.push_back(intersection(cur, nxt));
	}
	return 1;
}
struct Arc {
	LD lo, hi;// [lo, hi] - radian range of arc
	LD r;
	Arc(LD LO = 0, LD HI = 0, LD R = 0) : lo(LO), hi(HI), r(R) {}
	bool operator < (const Arc& a) const {
		return !sign(hi - a.hi) ? !sign(lo - a.lo) ?
			r > a.r :
			lo < a.lo :
			hi < a.hi;
	}
	LD area() const { return (hi - lo) * r * r; }
	//friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.r; return os; }
};
typedef std::vector<Arc> Fan;
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const LD& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	LD a = vec * vec;
	LD b = 2 * (vec * OM);
	LD c = OM * OM - r * r;
	LD J = b * b - 4 * a * c;
	if (J < TOL) return {};
	LD lo = (-b - sqrtq(J)) / (2 * a);
	LD hi = (-b + sqrtq(J)) / (2 * a);
	//if (hi < lo) std::swap(lo, hi);
	if (hi < 0 || 1 < lo) return {};
	return { { lo, hi } };//ratio, ratio
}
ld circle_cutting(const Pos& p1, const Pos& p2, const LD& r) {
	std::vector<Pos> inx = circle_line_intersections(p1, p2, O, r);
	if (inx.empty()) return cross(O, p1, p2);
	LD s = inx[0].x, e = inx[0].y;
	Pos vec = p2 - p1;
	if (0 < s && e < 1) {
		Pos m1 = p1 + vec * s, m2 = p1 + vec * e;
		return cross(O, p1, p2) * (s + 1 - e) + r * r * rad(m1, m2);
	}
	else if (0 < s) {
		Pos m = p1 + vec * s;
		return cross(O, p1, p2) * s + r * r * rad(m, p2);
	}
	else if (e < 1) {
		Pos m = p1 + vec * e;
		return cross(O, p1, p2) * (1 - e) + r * r * rad(p1, m);
	}
	else return r * r * rad(p1, p2);
}
LD sweep(const Polygon& H, const Fan& F) {
	LD ret = 0;
	int szf = F.size();
	int szh = H.size();
	for (int i = 0, j = 0, k; i < szf; i++) {
		LD lo = F[i].lo, hi = F[i].hi;
		Pos inx_lo, inx_hi, LO, HI;
		LO = X_norm.rot(lo);
		HI = X_norm.rot(hi);
		while (j < szh && sign(lo - H[j].t) >= 0) j++;
		k = j;
		while (k < szh - 1 && sign(hi - H[(k + 1) % szh].t) > 0) k++;
		if (sign(H[j % szh].t - lo) >= 0 && sign(hi - H[j % szh].t) >= 0) {
			inx_lo = intersection(O, LO, H[(j - 1 + szh) % szh], H[j % szh]);
			ret += circle_cutting(inx_lo, H[j % szh], F[i].r);
			inx_hi = intersection(O, HI, H[k % szh], H[(k + 1) % szh]);
			ret += circle_cutting(H[k % szh], inx_hi, F[i].r);
			for (int l = j; l < k; l++) {
				ret += circle_cutting(H[l], H[(l + 1) % szh], F[i].r);
			}
		}
		else {
			Pos inx_lo = intersection(O, LO, H[(j - 1 + szh) % szh], H[j % szh]);
			Pos inx_hi = intersection(O, HI, H[(j - 1 + szh) % szh], H[j % szh]);
			ret += circle_cutting(inx_lo, inx_hi, F[i].r);
		}

#ifdef DEBUG
		std::cout << "lo : " << lo << " hi : " << hi << "\n";
		std::cout << "ret : " << ret << "\n";
#endif

	}
	return ret * .5;
}
LD query() {
	ld alpha;
	std::cin >> N >> alpha;
	A = alpha;
	H.resize(N);
	V.resize(N);
	std::vector<Arc> arcs;
	std::vector<Arc> valid_arcs;//stack
	for (int i = 0; i < N; i++) {
		ld x, y;
		Pos p, v;
		std::cin >> x >> y;
		p = Pos(x, y);
		v = p.rot(A);
		H[i] = p, V[i] = v;
		Arc a1, a2;
		if (p.t <= v.t) {
			a1 = Arc(p.t, v.t, p.mag());
			arcs.push_back(a1);
		}
		else {
			a1 = Arc(p.t, PI * 2, p.mag());
			a2 = Arc(0, v.t, p.mag());
			arcs.push_back(a1);
			arcs.push_back(a2);
		}
	}

#ifdef DEBUG
	Q = 0;
	for (Arc& a : arcs) std::cout << "arcs[" << Q++ << "] : " << a << "\n";
#endif

	std::vector<Line> HP;
	for (int i = 0; i < N; i++) HP.push_back(L(H[i], H[(i + 1) % N]));
	for (int i = 0; i < N; i++) HP.push_back(L(V[i], V[(i + 1) % N]));

	HPI.clear();
#ifdef ASSERT
	assert(half_plane_intersection(HP, HPI));
#else
	half_plane_intersection(HP, HPI);
#endif

	std::sort(arcs.begin(), arcs.end());

#ifdef DEBUG
	Q = 0;
	for (Arc& a : arcs) std::cout << "arcs[" << Q++ << "] : " << a << "\n";
#endif

	for (Arc& a : arcs) {//sweep circle
		LD lo = a.lo;
		LD hi = a.hi;
		LD r = a.r;
		while (valid_arcs.size()) {
			if (sign(valid_arcs.back().hi - a.lo) <= 0) break;
			else {
				if (sign(valid_arcs.back().lo - a.lo) >= 0) {
					if (sign(r - valid_arcs.back().r) >= 0) valid_arcs.pop_back();
					else {
						lo = valid_arcs.back().hi;
						break;
					}
				}
				else {//valid_arcs.back().lo < a.lo;
					if (sign(r - valid_arcs.back().r) >= 0) valid_arcs.back().hi = lo;
					else lo = valid_arcs.back().hi;
					break;
				}
			}
		}
		//if (!sign(lo - hi)) continue;
		valid_arcs.push_back(Arc(lo, hi, r));
	}
	std::sort(valid_arcs.begin(), valid_arcs.end());
	Fan fan;
	int sz = valid_arcs.size();
	for (int i = 0; i < sz; i++) {
		fan.push_back(valid_arcs[i]);
		if (valid_arcs[i].hi > valid_arcs[(i + 1) % sz].lo) {
			Arc btwn1 = Arc(valid_arcs[i].hi, PI * 2, 0);
			Arc btwn2 = Arc(0, valid_arcs[(i + 1) % sz].lo, 0);
			fan.push_back(btwn1);
			fan.push_back(btwn2);
		}
		else {
			Arc btwn = Arc(valid_arcs[i].hi, valid_arcs[(i + 1) % sz].lo, 0);
			fan.push_back(btwn);
		}
	}
	LD area_origin = 0, area_convert = 0, area_hpi = 0, area_arcs = 0;
	std::sort(H.begin(), H.end(), cmpt);
	std::sort(V.begin(), V.end(), cmpt);
	std::sort(HPI.begin(), HPI.end(), cmpt);
	std::sort(fan.begin(), fan.end());
	area_origin = sweep(H, fan);
	area_convert = sweep(V, fan);
	area_hpi = sweep(HPI, fan);

#ifdef DEBUG
	std::cout << "origin : " << area_origin << "\n";
	std::cout << "convert : " << area_convert << "\n";
	std::cout << "hpi : " << area_hpi << "\n";
#endif

	LD total = area_origin + area_convert - area_hpi;
	//std::cout << "covered area : " << total << "\n";
	std::cout << (ld)total << "\n";
	return (ld)total;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj19061
//Petrozavodsk Programming Camp Summer 2017 Day 3: Ural Contest G

/*

1
4 3.14159265
-1 -1
1 -1
1 1
-1 1
6.283185307179588

1
4 1.6
-1 -1
1 -1
1 1
-1 1
6.283185307179587

1
4 3.14159265
-1000000 -1000000
1000000 -1000000
1000000 1000000
-1000000 1000000
6283185307179.58593750000

1
4 1.6
-1000000 -1000000
1000000 -1000000
1000000 1000000
-1000000 1000000
6283185307179.58593750000

1
4 3.14159265
-1000000000 -1000000000
1000000000 -1000000000
1000000000 1000000000
-1000000000 1000000000
6283185307179589632.000000000000000


1
4 1.6
-1000000000 -1000000000
1000000000 -1000000000
1000000000 1000000000
-1000000000 1000000000
6.283185307179587
6283185307179585536.000000000000000

*/