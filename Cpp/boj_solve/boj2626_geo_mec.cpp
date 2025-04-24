#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <cstdlib>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N;
bool zero(const ld& x) { return std::abs(x) < TOL; }

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
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y;
		return os;
	}
}; const Pos O = { 0, 0 };
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
	ld operator / (const Line& l) const { return s / l.s; }
	ld operator * (const Line& l) const { return s * l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c;
		return os;
	}
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3); return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && ret > 0;
}
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& p) const { return c == p.c && std::abs(r - p.r) < TOL; }
	bool operator != (const Circle C) const { return std::abs(C.c.x - c.x) >= TOL || std::abs(C.c.y - c.y) >= TOL || std::abs(r - C.r) >= TOL; }
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }//is valid
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { {c.x + C.c.x, c.y + C.c.y}, r + C.r }; }
	Circle operator - (const Circle& C) const { return { {c.x - C.c.x, c.y - C.c.y}, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	friend std::istream& operator >> (std::istream& is, Circle& c) {
		is >> c.c.x >> c.c.y >> c.r;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Circle& c) {
		os << c.c.x << " " << c.c.y << "\n" << c.r; return os;
	}
} INVAL = { { 0, 0 }, -1 };
Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Pos B = v - u, C = w - u;
	Line B_ = Line({ B.x, B.y }, B.Euc() / 2);
	Line C_ = Line({ C.x, C.y }, C.Euc() / 2);
	if (zero(B_ / C_)) return { { 0, 0 }, -1 };
	Pos inx = intersection(B_, C_);
	return Circle(inx + u, inx.mag());
}
//Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
//	Line l1 = rotate90(L(u, v), (u + v) * .5);
//	Line l2 = rotate90(L(v, w), (v + w) * .5);
//	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
//	Pos c = intersection(l1, l2);
//	ld r = (c - u).mag();
//	return Circle(c, r);
//}
bool valid_check(const Circle& c, const std::vector<Pos>& P) {
	for (const Pos& p : P) if (c < p) return 0;
	return 1;
}
Circle get_min_circle(std::vector<Pos>& P) {
	int sz = P.size();
	assert(sz <= 3);
	if (!sz) return Circle(Pos(0, 0), 0);
	if (sz == 1) return Circle(P[0], 0);
	if (sz == 2) return enclose_circle(P[0], P[1]);
	for (int i = 0; i < 2; i++) {
		for (int j = i + 1; j < 3; j++) {
			Circle ec = enclose_circle(P[i], P[j]);
			if (valid_check(ec, P)) return ec;
		}
	}
	return enclose_circle(P[0], P[1], P[2]);
}
Circle welzl(std::vector<Pos>& P, std::vector<Pos> R, int sz) {
	if (!sz || R.size() == 3) return get_min_circle(R);
	int idx = rand() % sz;
	Pos p = P[idx];
	std::swap(P[idx], P[sz - 1]);
	Circle mec = welzl(P, R, sz - 1);
	if (mec >= p) return mec;
	R.push_back(p);
	return welzl(P, R, sz - 1); 
}
Circle welzl(std::vector<Pos>& P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	return welzl(P, {}, P.size());
}
std::vector<Pos> pos;

Circle MEC(std::vector<Pos> P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	Circle mec = INVAL;
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		if (mec.r < -1 || mec < P[i]) {
			mec = Circle(P[i], 0);
			for (int j = 0; j <= i; j++) {
				if (mec < P[j]) {
					Circle ans = enclose_circle(P[i], P[j]);
					if (zero(mec.r)) { mec = ans; continue; }
					Circle l = INVAL, r = INVAL;
					Pos vec = P[j] - P[i];
					for (int k = 0; k <= j; k++) {
						if (ans < P[k]) {
							ld CCW = vec / (P[k] - P[j]);
							Circle c = enclose_circle(P[i], P[j], P[k]);
							if (c.r < 0) continue;
							else if (CCW > 0 && (l.r < 0 || (vec / (c.c - P[i])) > (vec / (l.c - P[i])))) l = c;
							else if (CCW < 0 && (r.r < 0 || (vec / (c.c - P[i])) < (vec / (r.c - P[i])))) r = c;
						}
					}
					if (l.r < 0 && r.r < 0) mec = ans;
					else if (l.r < 0) mec = r;
					else if (r.r < 0) mec = l;
					else mec = l.r < r.r ? l : r;
				}
			}
		}
	}
	return mec;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> N;
	pos.resize(N);
	for (int i = 0; i < N; i++) std::cin >> pos[i];
	//Circle mec = welzl(pos);
	Circle mec = MEC(pos);
	std::cout << mec;
	return;
}
int main() { solve(); return 0; }//boj2626 H
