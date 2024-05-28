#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
typedef double ld;
//typedef long double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5 + 1;
int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += PI * 2;
	while (th > PI * 2 - TOL) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

struct Pii {
	int x, y;
	Pii(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	inline Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	inline Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	inline Pii operator * (const int& n) const { return { x * n, y * n }; }
	inline Pii operator / (const int& n) const { return { x / n, y / n }; }
	inline ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
	inline ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	inline Pii operator - () const { return { -x, -y }; }
	inline Pii operator ~ () const { return { -y, x }; }
	inline Pii operator ! () const { return { y, x }; }
	inline ll xy() const { return (ll)x * y; }
	inline ll Euc() const { return (ll)x * x + (ll)y * y; }
	inline ld rad() const { return norm(atan2(y, x)); }
	int Man() const { return std::abs(x) + std::abs(y); }
	inline ld mag() const { return hypot(x, y); }
	inline friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (int)INF, (int)INF };
typedef std::vector<Pii> Hull;
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
inline ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
inline ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
inline ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
inline int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
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
	ld xy() const { return x * y; }
	inline Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	inline ld rad() const { return norm(atan2(y, x)); }
	inline friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
inline Pos P(const Pii& p) { return Pos(p.x, p.y); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline Pos intersection(const Pii& p1, const Pii& p2, const Pii& q1, const Pii& q2) {
	ll a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	ll x = (ll)p1.x * a2 + (ll)p2.x * a1;
	ll y = (ll)p1.y * a2 + (ll)p2.y * a1;
	ll num = a1 + a2;
	return Pos(1. * x / num, 1. * y / num);
}
struct Seg {
	Pii s, e;
	Seg(Pii s = Pii(0, 0), Pii e = Pii(0, 0)) : s(s), e(e) {}
	Pii S() const { return e - s; }
	inline ll operator / (const Seg& p) const { return S() / p.S(); }
	inline int ccw(const Pos& p) const { return sign((P(e) - P(s)) / (p - P(e))); }
};
inline Pos intersection(const Seg& u, const Seg& v) { return intersection(u.s, u.e, v.s, v.e); }
std::vector<int> B[LEN];//blocked
std::vector<Seg> segs, VH;
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N >> M;
	Hull H(N);
	for (Pii& p : H) std::cin >> p;
	int s, e;
	while (M--) {
		std::cin >> s >> e;
		s--, e--;
		B[s].push_back(e);
	}
	e = -1;
	for (s = 0; s < N - 1; s++) {
		std::sort(B[s].begin(), B[s].end(), [&](const int& u, const int& v) -> bool {
			return u > v;
			});
		int X = N - 1;
		for (const int& j : B[s]) {
			if (X > j) break;
			if (X == j) X--;
		}
		if (e < X) {
			e = X;
			segs.push_back(Seg(H[s], H[e]));
			assert(s < e);
		}
		if (e == N - 1) break;
	}
	for (const Seg& seg : segs) {
		while (VH.size() >= 2
			&& VH[VH.size() - 2] / seg < 0
			&& VH.back().ccw(intersection(VH[VH.size() - 2], seg)) <= 0)
			VH.pop_back();
		VH.push_back(seg);
	}
	Polygon ret;
	int sz;
	sz = VH.size();
	ret.push_back(P(H[0]));
	for (int i = 0; i < sz - 1; i++) ret.push_back(intersection(VH[i], VH[i + 1]));
	ret.push_back(P(H[N - 1]));
	ld ans = 0;
	sz = ret.size();
	for (int i = 0; i < sz - 1; i++) ans += (ret[i] - ret[i + 1]).mag();
	std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj8182 Island refer to koosaga
