#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 40;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }

Vld ans;
int N, M;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	Line(Vec V = Vec(0, 0), Pos p = Pos(0, 0)) : s(V) { c = s.vy * p.x + s.vx * p.y; }
	Line(Pos ps = Pos(0, 0), Pos pe = Pos(0, 0)) {
		ld dy, dx;
		dy = pe.y - ps.y;
		dx = ps.x - pe.x;
		s = Vec(dy, dx);
		c = dy * ps.x + dx * ps.y;
	}
	ld operator / (const Line& l) const { return s / l.s; }
	ld operator * (const Line& l) const { return s * l.s; }
};
Line L(const Pos& s, const Pos& e) { return Line(s, e); }
Line rotate90(const Line& l, const Pos& p) { return Line(~l.s, p); }
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
bool norm(Polygon& H) {
	ld a = area(H);
	if (zero(a)) return 1;
	if (a < 0) std::reverse(H.begin(), H.end());
	return 0;
}
Polygon rotate_and_norm(Polygon B, const int& j0, const Polygon& A, const int& i0, const ld& t) {
	int sz = B.size();
	for (int j = 0; j < sz; j++) B[j] = B[j].rot(t);
	Pos v = A[i0] - B[j0];
	for (int j = 0; j < sz; j++) B[j] += v;
	return B;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
} INVAL = { { 0, 0 }, -1 };
Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Line l1 = rotate90(L(u, v), (u + v) * .5);
	Line l2 = rotate90(L(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pos c = intersection(l1, l2);
	ld r = (c - u).mag();
	return Circle(c, r);
}
Circle minimum_enclose_circle(std::vector<Pos> P) {
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
					for (int k = 0; k <= j; k++) {
						if (ans < P[k]) {
							int CCW = ccw(P[i], P[j], P[k]);
							Circle c = enclose_circle(P[i], P[j], P[k]);
							if (c.r < 0) continue;
							else if (CCW > 0 && (l.r < 0 || cross(P[i], P[j], c.c) > cross(P[i], P[j], l.c))) l = c;
							else if (CCW < 0 && (r.r < 0 || cross(P[i], P[j], c.c) < cross(P[i], P[j], r.c))) r = c;
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
Circle minimum_enclose_circle(Polygon A, const int& i, Polygon B, const int& j, const ld& d) {
	Pos v = (A[(i + 1) % A.size()] - B[(j + 1) % B.size()]).unit() * d;
	for (Pos& p : B) A.push_back(p + v);
	return minimum_enclose_circle(A);
}
ld ternary_search(const Polygon& A, const int& i, const Polygon& B, const int& j) {
	int a = A.size(), b = B.size();
	const Pos& pa = A[(i + 1) % a], & pb = B[(j + 1) % b];
	ld s = 0, e = (pa - pb).mag(), m1, m2, r1 = 0, r2 = 0;
	int cnt = 25; while (cnt--) {
		m1 = (s + s + e) / 3;
		m2 = (s + e + e) / 3;
		r1 = minimum_enclose_circle(A, i, B, j, m1).r;
		r2 = minimum_enclose_circle(A, i, B, j, m2).r;
		if (r1 > r2) s = m1;
		else e = m2;
	}
	return r2;
}
ld fit(const Polygon& P, const ld& r) {
	auto dist = [&](const Pos& s1, const Pos& s2, const Pos& c) -> ld {
		ld d = cross(s1, s2, c) / (s1 - s2).mag();
		return r + d;
		};
	int sz = P.size();
	ld ret = INF;
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			ld tmp = INF;
			const Pos& p = P[i], & q = P[j];
			Pos mid = (p + q) * .5;
			Pos vec = p - mid;
			ld h = vec.mag();
			if (sign(h - r) > 0) continue;
			ld w = sqrt(r * r - h * h);
			Pos c1 = mid + ~vec.unit() * w;
			Pos c2 = mid - ~vec.unit() * w;
			for (const Pos& c : { c1, c2 }) {
				for (int k = 0; k < sz; k++) {
					const Pos& p0 = P[(k - 1 + sz) % sz], & p1 = P[k], & p2 = P[(k + 1) % sz];
					assert(ccw(p0, p1, p2) >= 0);
					if (k == i || k == j) continue;
					if (sign((P[k] - c).mag() - r) > 0) { tmp = INF; break; }
					if (ccw(p0, p1, c) < 0 && ccw(p1, p2, c) < 0 && dot(p0, p1, c) > 0 && dot(p2, p1, c) > 0) {
						tmp = std::min(tmp, r - (c - p1).mag());
					}
					else tmp = std::min({ tmp, dist(p0, p1, c), dist(p1, p2, c) });
				}
				ret = std::min(ret, tmp);
			}
		}
	}
	return ret;
}
bool fit(const Polygon& A, const Polygon& B, const ld& r) {
	ld da = fit(A, r), db = fit(B, r);
	return sign(r + r - (da + db)) >= 0 ? 1 : 0;
}
ld bi_search(const Polygon& A, const Polygon& B, ld s, ld e) {
	int cnt = 30; while (cnt--) {
		ld m = (s + e) * .5;
		if (fit(A, B, m)) e = m;
		else s = m;
	}
	return (s + e) * .5;
}
bool query() {
	std::cin >> N;
	if (!N) return 0;
	Polygon A(N);
	for (Pos& p : A) std::cin >> p;
	norm(A);
	std::cin >> M;
	Polygon B(M);
	for (Pos& p : B) std::cin >> p;
	norm(B);
	ld ret = INF, r1 = INF;
	ld ra = minimum_enclose_circle(A).r, rb = minimum_enclose_circle(B).r;
	for (int i = 0; i < N; i++) {
		const Pos& I0 = A[i], & I1 = A[(i + 1) % N];
		for (int j = 0; j < M; j++) {
			const Pos& J0 = B[j], & J1 = B[(j + 1) % M];
			ld t = rad(J0 - J1, I1 - I0);
			Polygon B2 = rotate_and_norm(B, j, A, i, t);
			r1 = std::min(r1, ternary_search(A, i, B2, j));
		}
	}
	ld r2 = bi_search(A, B, std::max(ra, rb), r1);
	ret = std::min({ ret, r1, r2 }) * 2;
	ans.push_back(ret);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (query());
	for (const ld& ret : ans) std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj20231 Idealistic Canister
