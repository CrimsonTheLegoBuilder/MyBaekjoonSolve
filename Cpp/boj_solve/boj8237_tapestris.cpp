#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::vector<ld> vld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acosl(-1);
const int LEN = 1005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

int T, N;
bool V[LEN];
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
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
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
struct Seg {
	Pos s, e;
	inline Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	inline ld green(const Pos& m, const ld& v) const { return m.y * v * (s.x - e.x); }
};
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
inline ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2 - d1) * (d4 - d3) / (d2 - d1).mag();
}
inline ld intersection(const Seg& s1, const Seg& s2) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	//return f1 && f2;
	bool f3 = on_seg_strong(s1, s2, d1) ||
		on_seg_strong(s1, s2, d2) ||
		on_seg_strong(d1, d2, s1) ||
		on_seg_strong(d1, d2, s2);
	return (f1 && f2) || f3;
}
ll area(std::vector<Pos>& H) {
	ll ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
inline void query() {
	std::cin >> N;
	Polygon H(N);
	for (Pos& p : H) std::cin >> p;
	norm(H);
	//assert(inner_check(H, O));
	for (int i = 0; i < N; i++) {
		char b;
		std::cin >> b;
		V[i] = (b == 'C' ? 0 : 1);
	}
	auto inner = [&](const Pos& a, const Pos& b, const Pos& c, const Pos& t) {
		return ccw(a, b, t) > 0 && ccw(b, c, t) > 0 && ccw(c, a, t) > 0;
		};
	for (int i = 0; i < N; i++) {
		Pos& s = H[i], e = H[(i + 1) % N];
		Pos cur = e;
		if (V[i] && on_seg_strong(s, e, O)) continue;
		if (!V[i] && on_seg_strong(s, e, O)) { std::cout << "NIE\n"; return; }
		if (V[i] && cross(s, e, O) <= 0) { std::cout << "NIE\n"; return; }
		if (!V[i] && cross(s, e, O) <= 0) continue;
		for (int j = i + 1; j < i + N; j++) {
			Pos u = H[j % N], v = H[(j + 1) % N];
			if (on_seg_strong(u, v, O)) continue;
			ll CCW = ccw(O, u, v);
			if (V[j % N] && CCW <= 0) { std::cout << "NIE\n"; return; }
			if (CCW == 0) continue;
			if (CCW < 0) std::swap(u, v);
			if (intersect(O, s, u, v) && intersect(O, e, u, v)) {
				if (V[i]) { std::cout << "NIE\n"; return; }
				if (!V[i]) break;
			}
			if (inner(O, s, e, u) || inner(O, s, e, v)) {
				if (V[i]) { std::cout << "NIE\n"; return; }
				if (!V[i]) {
					if (ccw(O, cur, v) >= 0) {
						if (ccw(O, cur, u) < 0) cur = u;
					}
				}
			}
		}
		if (!V[i] && ccw(O, s, cur) > 0) { std::cout << "NIE\n"; return; }
	}
	std::cout << "TAK\n";
	return;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj8237 Tapestries
