#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ll INF = 1e17;
const int LEN = 1e5 + 1;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }

int N;
struct Pos {
	int x, y;
	ll c;
	Pos(int X = 0, int Y = 0, ll C = 0) : x(X), y(Y), c(C) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return (ll)x * y; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
int collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, bool f = 0) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	if (f) return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = 1) {
	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
}
int count(Pos s, Pos e, const Pos& q) {
	if (on_seg_strong(s, e, q)) return 2;
	if (s.y == e.y) return 0;
	if (s.y > e.y) std::swap(s, e);
	if (s.y >= q.y || q.y > e.y) return 0;
	return ccw(s, e, q) > 0;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	Pos s, e;
	Polygon P(N);
	std::cin >> s >> e;
	if (s == e) { std::cout << "1\n"; return; }
	int si = 0, ei = 0, cnt = 0;
	int s_in = 0, e_in = 0;
	for (Pos& p : P) std::cin >> p;
	if (e < s) std::swap(s, e);
	for (int i = 0; i < N; i++) {
		const Pos& p0 = P[i], p1 = P[(i + 1) % N], p2 = P[(i + 2) % N], p3 = P[(i + 3) % N];
		int f;
		if (!s_in) {
			f = count(p1, p2, s);
			if (f == 2) s_in == 2;
			else si += f;
		}
		if (!e_in) {
			f = count(p1, p2, e);
			if (f == 2) e_in == 2;
			else ei += f;
		}
		if (!intersect(s, e, p1, p2)) continue;
		if (intersect(s, e, p1, p2, 1)) cnt++;
		else if (collinear(s, e, p1, p2)) {
			int ccw0 = ccw(p1, p2, p0), ccw3 = ccw(p1, p2, p3);
			assert(ccw0); assert(ccw3);
			if (on_seg_strong(p1, p2, s) && on_seg_strong(p1, p2, e)) { std::cout << "1\n"; return; }
			else if (on_seg_weak(s, e, p1) && on_seg_weak(s, e, p2)) {
				cnt += ccw0 != ccw3;
			}
			else if (on_seg_strong(p1, p2, s)) {
				if (dot(p1, p2, e) > 0 && ccw3 < 0) cnt++;
				else if (dot(p2, p1, e) > 0 && ccw0 < 0) cnt++;
			}
			else if (on_seg_strong(p1, p2, e)) {
				if (dot(p1, p2, s) > 0 && ccw3 < 0) cnt++;
				else if (dot(p2, p1, s) > 0 && ccw0 < 0) cnt++;
			}
		}
		else if (p1 == s) {
			if (collinear(s, e, p1, p0)) continue;
			cnt += inside(p0, p1, p2, e);
		}
		else if (p1 == e) {
			if (collinear(s, e, p1, p0)) continue;
			cnt += inside(p0, p1, p2, s);
		}
		else if (on_seg_weak(s, e, p1)) {
			if (collinear(s, e, p1, p0)) continue;
			int ccw0 = ccw(s, e,p0), ccw2 = ccw(s, e, p2);
			assert(ccw0); assert(ccw2);
			cnt += ccw0 != ccw2;
		}
		else if (on_seg_weak(p1, p2, s)) {
			cnt += ccw(p1, p2, e) > 0;
		}
		else if (on_seg_weak(p1, p2, e)) {
			cnt += ccw(p1, p2, s) > 0;
		}
	}
	if (s_in != 2) s_in = si & 1;
	if (e_in != 2) e_in = ei & 1;
	if (s_in & 1) cnt--;
	if (e_in & 1) cnt--;
	cnt = std::max(cnt, 0);
	std::cout << (cnt >> 1) + 1 << "\n";
}
int main() { solve(); return 0; }//boj1873