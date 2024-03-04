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
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5;
int N;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ld norm(ld& th) {
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

struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar, 0 }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar, 0 }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, 0 }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
} pos[LEN]; const Pos O = { 0, 0, -1 };
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
};
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
		0
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3)) && ret > 0;
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {
	int cnt{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
	//bool f3 = on_seg_strong(s1, s2, d1) ||
	//	on_seg_strong(s1, s2, d2) ||
	//	on_seg_strong(d1, d2, s1) ||
	//	on_seg_strong(d1, d2, s2);
	//return (f1 && f2) || f3; 
}
void sweeping() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	ll cnt = 0;
	Pos top, bot;
	int f = 0;
	std::cin >> top >> bot;
	if (top < bot) std::swap(top, bot);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> pos[i];
	for (int i = 0; i < N; i++) {
		Pos pre = pos[(i - 1 + N) % N], cur = pos[i], nxt = pos[(i + 1) % N];
		if (intersect(pre, cur, top, bot)) cnt++;
		else if (on_seg_strong(top, bot, cur)) {
			if (!ccw(pre, cur, top)) f = ccw(bot, top, pre);
			if (ccw(bot, top, pre) * ccw(bot, top, nxt) == -1) cnt++;
			else if (!ccw(cur, nxt, top)) continue;
			else if (ccw(pre, cur, bot) >= 0 && ccw(bot, top, nxt) <= 0) continue;
			else if (ccw(pre, cur, bot) >= 0 && ccw(bot, top, nxt) > 0) cnt += 2;
			else if (ccw(pre, cur, top) >= 0 && ccw(bot, top, nxt) >= 0) continue;
			else if (ccw(pre, cur, top) >= 0 && ccw(bot, top, nxt) < 0) cnt += 2;
			
		}
	}

}
int main() { sweeping(); return 0; }//boj1873 cutting