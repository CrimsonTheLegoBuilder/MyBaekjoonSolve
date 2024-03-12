#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 205;
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
	friend ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
	friend ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
	friend int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
		ld ret = cross(d1, d2, d3); return zero(ret) ? 0 : ret > 0 ? 1 : -1;
	}
	friend int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
		ld ret = (d2 - d1) / (d4 - d3);
		return zero(ret) ? 0 : ret > 0 ? 1 : -1;
	}
	friend bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
		ld ret = dot(d1, d3, d2);
		return zero(cross(d1, d2, d3)) && (ret > 0 || zero(ret));
	}
	friend bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
		ld ret = dot(d1, d3, d2);
		return zero(cross(d1, d2, d3)) && ret > 0;
	}
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
} H[LEN]; const Pos O = { 0, 0 };
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
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
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line operator + (const ld& scalar) const {
		ld tol = hypot(s.vy, s.vx) * scalar;
		ld nc = c + tol;
		return Line(s, nc);
	}
	Line operator - (const ld& scalar) const {
		ld tol = hypot(s.vy, s.vx) * scalar;
		ld nc = c - tol;
		return Line(s, nc);
	}
	Line& operator += (const ld& scalar) {
		ld tol = hypot(s.vy, s.vx) * scalar;
		c += tol;
		return *this;
	}
	Line& operator -= (const ld& scalar) {
		ld tol = hypot(s.vy, s.vx) * scalar;
		c -= tol;
		return *this;
	}
	Line& operator *= (const ld& scalar) { s *= scalar; c *= scalar; return *this; }
	Line& operator /= (const ld& scalar) { s /= scalar; c /= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c << "\n";
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
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
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
bool across(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	int ret = ccw(s1, s2, d1) * ccw(s1, s2, d2);
	return (ret < 0 || zero(ret)) ? 1 : 0;
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {//concave
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
void brute(const int& i, const int& j, ld& MAX) {
	Pos S = H[i], E = H[j];
	//std::cout << (S - E).mag() << "\n";
	Line ij = L(S, E);
	std::vector<Pos> SEG;
	for (int k = 0; k < N; k++) {
		Pos d1 = H[k], d2 = H[(k + 1) % N];
		Pos d0 = H[(k - 1 + N) % N], d3 = H[(k + 2) % N];
		if (intersect(S, E, d1, d2)) return;
		if (on_seg_strong(S, E, d1) && on_seg_strong(S, E, d2) &&
			intersect(S, E, d0, d3)) return;
		//std::cout << across(S, E, d1, d2) << "\n";
		if (across(S, E, d1, d2)) {
			if (zero(ccw(S, E, d1, d2))) {
				SEG.push_back(d1);
				SEG.push_back(d2);
			}
			else SEG.push_back(intersection(ij, L(d1, d2)));
		}
	}
	std::sort(SEG.begin(), SEG.end());
	Pos u = SEG[0], v = *(SEG.end() - 1);
	if ((u - v).mag() < MAX) return;
	ld dist = 0;
	int sz = SEG.size();
	for (int k = 0; k < sz - 1; k++) {
		Pos cur = SEG[k], nxt = SEG[(k + 1) % sz];
		if (inner_check(H, N, (cur + nxt) * .5)) dist += (cur - nxt).mag();
		else {
			MAX = std::max(MAX, dist);
			dist = 0;
		}
	}
	MAX = std::max(MAX, dist);
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> H[i];
	ld MAX = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			brute(i, j, MAX);
		}
	}
	std::cout << MAX << "\n";
	return;
}
int main() { solve(); return 0; }//boj14633 Airport Construction