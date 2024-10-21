/* https://codeforces.com/blog/entry/68748 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-6;
const ld PI = acos(-1);
const int LEN = 20005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }

#define CUT(i, j) std::cout << "scissors\n"; std::cout << (i) << " " << (j) << "\n";
#define TAPE(c) std::cout << "tape\n"; << std::cout << (c) << " ";	  

int N, M;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon S, T;
ld l;
struct Event {
	int i;
	Vint idx;
	Polygon H;
} ;
Polygon P[LEN]; int t;
void print(const Polygon& H) {
	std::cout << H.size();
	for (const Pos& p : H) std::cout << " " << p;
	std::cout << "\n";
	return;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ld ret = cross(d1, d2, d3, d4); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
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
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
void norm(Polygon& H) { ld a = area(H); if (sign(a) < 0) std::reverse(H.begin(), H.end()); }
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) > 0 || ccw(p1, p2, q) > 0;
	return ccw(p0, p1, q) > 0 && ccw(p1, p2, q) > 0;
}
bool closer(const Polygon& H, const int& i, const int& j) {
	int sz = H.size();
	int i0 = (i - 1 + sz) % sz, i2 = (i + 1) % sz;
	if (!inside(H[i0], H[i], H[i2], H[j])) return 0;
	for (int k0 = 0; k0 < sz; k0++) {
		int k1 = (k0 + 1) % sz;
		if (k0 == i || k0 == j || k1 == i || k1 == j) continue;
		if (intersect(H[i], H[j], H[k0], H[k1])) return 0;
	}
	return 1;
}
void triangulation(const Polygon& H) {
	int sz = H.size();
	if (sz == 3) { P[t++] = H; return; }
	for (int i = 0; i < sz; i++) {
		for (int j = i + 1; j < sz; j++) {
			if (closer(H, i, j)) {
				Polygon H1, H2;
				for (int k = i; k != (j + 1) % sz; k = (k + 1) % sz) H1.push_back(H[k]);
				for (int k = j; k != (i + 1) % sz; k = (k + 1) % sz) H2.push_back(H[k]);
				triangulation(H1);
				triangulation(H2);
				return;
			}
		}
	}
	return;
}
ld h[LEN];
Polygon rect_to_rect(const Polygon& H, const ld& l, const int& i, bool& f) {
	ld l_ = H[1].x - H[0].x;
	ld h_ = H[3].y - H[0].y;
	if (eq(l, l_) && eq(h[i], h_)) {
		f = 1;
		return {};
	}
	if (sign(l_ - h_ * 2) > 0) {

	}
	Polygon H0;
	l_ = H0[1].x - H0[0].x;
	if (eq(l, h[i])) f = 1;
	return H0;
}
void polygon_to_square() {
	l = sqrt(area(S));
	t = 1;
	triangulation(S);
	CUT(0, N - 2);
	for (int i = 0; i < t; i++) print(P[i]);
	return;
}
void square_to_target() {

}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(6);
	std::cin >> N;
	S.resize(N);
	for (Pos& p : S) std::cin >> p;
	norm(S);
	std::cin >> M;
	T.resize(M);
	for (Pos& p : T) std::cin >> p;
	norm(T);
	polygon_to_square();
	square_to_target();
	std::cout << M;
	for (const Pos& p : T) std::cout << " " << p;
	return;
}
/* https://codeforces.com/blog/entry/68748 */