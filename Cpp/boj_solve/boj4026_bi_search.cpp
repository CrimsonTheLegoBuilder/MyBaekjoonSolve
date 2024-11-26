#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }

Vld ans;
int T, N, M;
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
	//bool operator < (const Pos& r) const { return zero(x - r.x) ? zero(y - r.y) ? d < r.d : y < r.y : x < r.x; }
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
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
ld bi_search(const Polygon& A, const Polygon& B, ld s, ld e) {
	int cnt = 30; while (cnt--) {
		ld m = (s + e) * .5;
		//std::cout << "m:: " << m << "\n";

	}
	return (s + e) * .5;
}
ld ratio(const Pos& p1, const Pos& p2, const ld& t, ld& h, ld& w) {
	Pos ho = Pos(1, 0).rot(t);
	Pos ve = ~ho;
	Pos b1 = intersection(O, ho, p1, p1 + ve);
	Pos b2 = intersection(p2, p2 + ho, b1, b1 + ve);
	if (ccw(p2, b2, p1) >= 0) return -1;
	h = b1.mag();
	w = (b1 - b2).mag();
	return w / h;
}
void query(int tc) {
	std::cout << "Case " << tc << ": ";
	Polygon P(4);
	for (Pos& p : P) std::cin >> p;
	P = graham_scan(P);
	if (P.size() < 4) { std::cout << "no solution\n"; return; }
	Pos p1 = P[0], p2 = P[2], q1 = P[1], q2 = P[3];
	if (dot(p1, p2, q1) >= 0 || dot(p2, p1, q1) >= 0 ||
		dot(p1, p2, q2) >= 0 || dot(p2, p1, q2) >= 0 || 
		dot(q1, q2, p1) >= 0 || dot(q2, q1, p1) >= 0 ||
		dot(q1, q2, p2) >= 0 || dot(q2, q1, p2) >= 0) {
		std::cout << "no solution\n"; return;
	}
	Pos v1 = p2 - p1, v2 = q2 - q1;
	if (zero(v1 * v2) && eq(v1.mag(), v2.mag())) { std::cout << v1.mag() << "\n"; return; }
	ld d1 = v1.Euc(), d2 = v2.Euc();
	if (d2 < d1) std::swap(d1, d2);
	if (sign(d1 - (d1 + d1)) >= 0) { std::cout << "no solution\n"; return; }
	ld t = std::abs(rad(v1, v2));
	t = std::min(t, PI - t);
	ld l1 = v1.mag(), l2 = v2.mag();
	if (l1 < l2) std::swap(l1, l2);
	p1 = Pos(l1, 0);
	p2 = Pos(l2, 0).rot(t);
	ld s = 0, e = -(PI * .5 - t), m;
	ld h = -1, w = -2;
	int cnt = 30;
	while (cnt--) {
		m = (s + e) * .5;
		if (ratio(p1, p2, m, h, w) > 1) s = m;
		else e = m;
	}
	if (eq(h, w)) std::cout << w << "\n";
	else std::cout << "no solution\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	for (int tc = 1; tc <= T; tc++) query(tc);
	return;
}
int main() { solve(); return 0; }//boj20231
