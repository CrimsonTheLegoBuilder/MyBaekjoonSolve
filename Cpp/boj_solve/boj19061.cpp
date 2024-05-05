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
//#include <quadmath.h>
typedef long long ll;
//typedef double ld;
typedef long double ld;
//typedef __float128 ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
ld A;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	ld x, y;
	ld t;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { t = norm(atan2(y, x)); }
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
	ld rad() const { return norm(atan2l(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
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
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
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
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
Line L(const Vec& s, const Pos& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cosl(the) - y * sinl(the));
	ld vy = x * sinl(the) + y * cosl(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
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
	ld lo, hi;// [lo, hi] - radian range of arc
	ld r;
	Arc(ld LO = 0, ld HI = 0, ld R = 0) : lo(LO), hi(HI), r(R) {}
	bool operator < (const Arc& a) const {
		return !sign(r - a.r) ? !sign(lo - a.lo) ?
			hi < a.hi :
			lo < a.lo :
			r > a.r;
	}
	ld area() const { return (hi - lo) * r * r; }
};
typedef std::vector<Arc> Fan;
//std::vector<Pos> circle_line_intersection(const Pos& o, const ld& r, const Pos& p1, const Pos& p2) {
//	ld d = dist(p1, p2, o);
//	if (std::abs(d) > r) return {};
//	Pos vec = p2 - p1;
//	Pos m = intersection(p1, p2, o, o + ~vec);
//	ld distance = vec.mag();
//	ld ratio = sqrt(r * r - d * d);
//	Pos m1 = m - vec * ratio / distance;
//	Pos m2 = m + vec * ratio / distance;
//	if (dot(p1, p2, m1, m2) < 0) std::swap(m1, m2);
//	return { m1, m2 };//p1->p2
//}
//ld circle_cut(const Pos& p1, const Pos& p2, const ld& r) {
//	Pos v1 = p1, v2 = p2;
//	std::vector<Pos> inx = circle_line_intersection(O, r, v1, v2);
//	if (inx.empty()) return r * r * rad(v1, v2) * .5;
//	Pos m1 = inx[0], m2 = inx[1];
//	bool d1 = dot(m1, v1, m2) > -TOL, d2 = dot(m1, v2, m2) > -TOL;
//	if (d1 && d2) return (v1 / v2) * .5;
//	else if (d1) return (v1 / m2 + r * r * rad(m2, v2)) * .5;
//	else if (d2) return (r * r * rad(v1, m1) + m1 / v2) * .5;
//	else if (dot(v1, m1, v2) > 0 && dot(v1, m2, v2) > 0)
//		return (r * r * (rad(v1, m1) + rad(m2, v2)) + m1 / m2) * .5;
//	else return (r * r * rad(v1, v2)) * .5;
//}
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;
	ld J = b * b - 4 * a * c;
	if (J < TOL) return {};
	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);
	//if (hi < lo) std::swap(lo, hi);
	if (hi < 0 || 1 < lo) return {};
	return { { lo, hi } };//ratio, ratio
}
ld circle_cutting(const Pos& p1, const Pos& p2, const ld& r) {
	std::vector<Pos> inx = circle_line_intersections(p1, p2, O, r);
	if (inx.empty()) return cross(O, p1, p2) * .5;
	ld s = inx[0].x, e = inx[0].y;
	Pos vec = p2 - p1;
	if (0 < s && e < 1) {
		Pos m1 = p1 + vec * s, m2 = p1 + vec * e;
		return cross(O, p1, p2) * (s + 1 - e) * .5 + r * r * rad(m1, m2);
	}
	else if (0 < s) {
		Pos m = p1 + vec * s;
		return cross(O, p1, p2) * s * .5 + r * r * rad(m, p2);
	}
	else if (e < 1) {
		Pos m = p1 + vec * e;
		return cross(O, p1, p2) * (1 - e) * .5 + r * r * rad(p1, m);
	}
	else return r * r * rad(p1, p2);
}
ld sweep(const Polygon& H, const Fan& F) {
	ld ret = 0;
	int szf = F.size();
	int szh = H.size();
	for (int i = 0, j = 0, k; i < szf; i++) {
		ld lo = F[i].lo, hi = F[i].hi;
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
	}
	return ret;
}
ld query() {
	std::cin >> N >> A;
	H.resize(N);
	V.resize(N);
	std::vector<Arc> arcs;
	std::vector<Arc> valid_arcs;//stack
	for (int i = 0; i < N; i++) {
		Pos p, v;
		std::cin >> p;
		v = p.rot(A);
		H[i] = p, V[i] = v;
		Arc a1, a2;
		if (p.t < v.t) {
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
	std::vector<Line> HP;
	for (int i = 0; i < N; i++) HP.push_back(L(H[i], H[(i + 1) % N]));
	for (int i = 0; i < N; i++) HP.push_back(L(V[i], V[(i + 1) % N]));
#ifdef ASSERT
	assert(half_plane_intersection(HP, HPI));
#endif
	half_plane_intersection(HP, HPI);
	std::sort(arcs.begin(), arcs.end());
	for (Arc& a : arcs) {//sweep circle
		ld lo = a.lo;
		ld hi = a.hi;
		ld r = a.r;
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
		if (!sign(lo - hi)) continue;
		valid_arcs.push_back(Arc(lo, hi, r));
	}
	std::sort(valid_arcs.begin(), valid_arcs.end());
	Fan fan;
	int sz = valid_arcs.size();
	for (int i = 0; i < sz; i++) {
		fan.push_back(valid_arcs[i]);
		Arc btwn = Arc(valid_arcs[i].hi, valid_arcs[(i + 1) % sz].lo, 0);
		fan.push_back(btwn);
	}
	ld area_origin = 0, area_convert = 0, area_hpi = 0, area_arcs = 0;
	std::sort(H.begin(), H.end(), cmpt);
	std::sort(V.begin(), V.end(), cmpt);
	std::sort(HPI.begin(), HPI.end(), cmpt);
	std::sort(fan.begin(), fan.end());
	for (Arc& a : fan) area_arcs += a.area();
	area_origin = sweep(H, fan);
	area_convert = sweep(V, fan);
	area_hpi = sweep(HPI, fan);
	ld total = area_origin + area_convert + area_hpi - area_arcs - area_arcs;
	std::cout << total << "\n";
	return total;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj19061
//Petrozavodsk Programming Camp Summer 2017 Day 3: Ural Contest G