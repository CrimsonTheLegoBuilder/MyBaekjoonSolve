#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 2e4;
int Q, N;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	inline Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	inline Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline Pos operator ! () const { return { y, x }; }
	inline ld xy() const { return x * y; }
	inline Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	inline Pos unit() const { return *this / mag(); }
	inline ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	inline ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	inline ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	inline Vec operator ~ () const { return { -vx, vy }; }
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
	inline ld operator * (const Line& l) const { return s * l.s; }
	inline ld operator / (const Line& l) const { return s / l.s; }
	inline Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	inline Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	inline Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	inline ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	inline ld dist(const Pos& p) const { return above(p) / hypot(s.vy, s.vx); }
	ld mag() const { return s.mag(); }
	friend inline ld rad(const Line& b, const Line& l) { return atan2(b / l, b * l); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
typedef std::vector<Line> Lines;
inline Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
inline Line L(const Vec& s, const Pos& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
inline Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
inline Line rot90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
inline Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
inline bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
inline ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	if (sign(projection(d1, d2, d2, t)) <= 0 && 
		sign(projection(d2, d1, d1, t)) <= 0) 
		return std::abs(cross(d1, d2, t)) / (d1 - d2).mag();
	return std::min((d1 - t).mag(), (d2 - t).mag());
}
inline int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
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
} INVAL = { { 0, 0 }, -1 };
typedef std::vector<Circle> Disks;
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
inline Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
inline Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	if (!ccw(u, v, w)) return INVAL;
	Pos m1 = (u + v) * .5, v1 = ~(v - u);
	Pos m2 = (u + w) * .5, v2 = ~(w - u);
	Pos c = intersection(m1, m1 + v1, m2, m2 + v2);
	return Circle(c, (u - c).mag());
}
inline Circle inclose_circle(const Line& I, const Line& J, const Line& K) {
	if (zero(I / J) || zero(J / K)) return INVAL;
	if (sign(I / J) > 0 && sign(J / K) > 0 && K.above(intersection(I, J)) < 0) {
		Pos p0 = intersection(I, J), q0 = intersection(J, K);
		Pos p1 = intersection(I + 69, J + 69), q1 = intersection(J + 74, K + 74);
		Pos c = intersection(L(p0, p1), L(q0, q1));
		return Circle(c, std::abs(I.dist(c)));
	}
	else return INVAL;
}
inline Disks inclose_circle(const Line& I, const Line& J, const Pos& p) {
	if (I.above(p) > 0 || J.above(p) > 0 || sign(I / J) < 0) return {};//INVAL
	if (zero(I / J) && I * J < 0) {
		/*
		J ____________
		       * p
		  ____________ I
		*/
		Line b = rot90(I, p);
		Pos p1 = intersection(I, b);
		Pos p2 = intersection(J, b);
		Pos m = (p1 + p2) * .5;
		Pos vec = ~(p1 - p2);
		ld r = vec.mag() * .5;
		ld h = (p - m).mag();
		ld w = sqrt(r * r - h * h);
		Pos cen1 = m + vec.unit() * w;
		Pos cen2 = m - vec.unit() * w;
		return { Circle(cen1, r), Circle(cen2, r) };
	}
	else if (sign(I / J) > 0) {
		/*      m h p J
		     \---+-*-/perp
		      \  |bi/
			   \D| /
			  I \|/
				 *
			    inx
		*/
		ld the = rad(J, I * -1) * .5;
		Pos inx = intersection(I, J);
		Line bi = rotate(J, inx, the);
		Line perp = rot90(bi, p);
		Pos m = intersection(bi, perp);
		ld D = (inx - m).mag();
		ld h = (p - m).mag();
		ld A = (1 - (1 / sin(the) / sin(the)));
		ld B = 2 * D / sin(the);
		ld C = -(h * h + D * D);
		ld JD = B * B - 4 * A * C;
		if (JD < 0) return {};//INVAL
		ld r1 = (-B - sqrt(JD)) * .5 / A;
		ld r2 = (-B + sqrt(JD)) * .5 / A;
		Pos cen1 = inx + (m - inx).unit() * (r1 / sin(the));
		Pos cen2 = inx + (m - inx).unit() * (r2 / sin(the));
		return { Circle(cen1, r1), Circle(cen2, r2) };
		//ld r = std::min(r1, r2);
		//Pos cen = inx + (m - inx).unit() * (r / sin(the));
		//return { Circle(cen, r) };
	}
	return {};//INVAL
}
inline Disks inclose_circle(const Line& I, const Pos& p, const Pos& q) {
	ld AP = I.above(p), AQ = I.above(q);
	if (AP > 0 || AQ > 0) return {};//INVAL
	if (zero(AP) && zero(AQ)) return {};//INVAL
	if (sign(AP) <= 0 && sign(AQ) <= 0) {
		if (zero(L(p, q) * I)) {
			/*
			      * q
				  
				  * p
			______________ I
			*/
			Pos m = (p + q) * .5;
			ld r = std::abs(I.dist(m));
			ld h = (p - m).mag();
			ld w = sqrt(r * r - h * h);
			Pos vec = ~(p - q).unit();
			Pos cen1 = m + vec * w;
			Pos cen2 = m - vec * w;
			return { Circle(cen1, r), Circle(cen2, r) };
		}
		Circle en = enclose_circle(p, q);
		if (sign(std::abs(I.dist(en.c)) - en.r) > 0) {
			if (zero(AP - AQ)) {
				/*
				    * q   * p

				_______________ I
				*/
				Pos m = (p + q) * .5;
				Line bi = rot90(L(p, q), m);
				Pos inx = intersection(I, bi);
				ld D = std::abs(I.dist(m));
				ld d = (p - m).mag();
				ld r = (D * D + d * d) * .5 / D;
				Pos cen = inx + (m - inx).unit() * r;
				return { Circle(cen, r) };
			}
			/*
			     * q
		             * m
					/    * p
			   inx /bi
			______*_________ I
			*/
			Pos m = (p + q) * .5;
			ld h = (p - m).mag();
			Line bi = rot90(L(p, q), m);
			if (I / bi < 0) bi *= -1;
			Pos inx = intersection(I, bi);
			ld the = rad(I, bi);
			ld D = (inx - m).mag();
			ld A = (1 - (1 / sin(the) / sin(the)));
			ld B = 2 * D / sin(the);
			ld C = -(h * h + D * D);
			ld JD = B * B - 4 * A * C;
			if (JD < 0) return {};//INVAL
			ld r1 = (-B - sqrt(JD)) * .5 / A;
			ld r2 = (-B + sqrt(JD)) * .5 / A;
			Pos cen1 = inx + (m - inx).unit() * (r1 / sin(the));
			Pos cen2 = inx + (m - inx).unit() * (r2 / sin(the));
			return { Circle(cen1, r1), Circle(cen2, r2) };
			//ld r = std::min(r1, r2);
			//Pos cen = inx + (m - inx).unit() * (r / sin(the));
			//return { Circle(cen, r) };
		}
	}
	return {};//INVAL
}
inline bool valid_check(const Polygon& H, const Circle& c) {
	if (c.r < 0) return 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
#ifdef DEBUG
		std::cout << "dist : " << dist(H[i], H[(i + 1) % sz], c.c) << "\n";
#endif
		if (sign(dist(H[i], H[(i + 1) % sz], c.c) - c.r) < 0) {
#ifdef DEBUG
			std::cout << "out\n";
#endif
			return 0;
		}
	}
	return inner_check(H, c.c) > 0;
}
inline ld query(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < sz; j++) {
			if (j == i) continue;
			for (int k = 0; k < sz; k++) {
				if (k == i || k == j) continue;
				const Pos& p0 = H[i], q0 = H[j], r0 = H[k];
				const Pos& p1 = H[(i + 1) % sz];
				const Pos& q1 = H[(j + 1) % sz];
				const Pos& r1 = H[(k + 1) % sz];

				Circle en, in;
				//___________________________________________________//
				en = enclose_circle(p0, q0, r0);
#ifdef DEBUG
				std::cout << "en : " << en << "\n";
#endif
				if (valid_check(H, en)) ret = std::max(ret, en.r);
				
				//___________________________________________________//
				in = inclose_circle(L(p0, p1), L(q0, q1), L(r0, r1));
#ifdef DEBUG
				std::cout << "in : " << in << "\n";
#endif
				if (valid_check(H, in)) ret = std::max(ret, in.r);
				

				Disks ins;
				//___________________________________________________//
				ins = inclose_circle(L(q0, q1), L(r0, r1), p0);
#ifdef DEBUG
				std::cout << "ins.size() : " << ins.size() << "\n";
				for (Circle& c : ins) std::cout << "ins : " << c << "\n";
#endif
				for (const Circle& c : ins) 
					if (valid_check(H, c))
						ret = std::max(ret, c.r);
				
				//___________________________________________________//
				ins = inclose_circle(L(r0, r1), p0, q0);
#ifdef DEBUG
				std::cout << "ins.size() : " << ins.size() << "\n";
				for (Circle& c : ins) std::cout << "ins : " << c << "\n";
#endif
				for (const Circle& c : ins) 
					if (valid_check(H, c))
						ret = std::max(ret, c.r);
			}
		}
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	//freopen("G.in", "r", stdin);
	//freopen("G.out", "w", stdout);
	std::cin >> Q;
	while (Q--) {
		std::cin >> N;
		Polygon H(N);
		for (Pos& p : H) std::cin >> p;
		std::cout << query(H) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj5438 Secret Island Base

/*

1
4
0 0
5 0
5 5
0 5

1
10
15 20
15 23
14 23
14 21
11 21
11 24
15 24
15 25
10 25
10 20

2
3
-100 -100
98 100
-2 -1
4
-99 -100
100 100
-100 -99
99 99

*/