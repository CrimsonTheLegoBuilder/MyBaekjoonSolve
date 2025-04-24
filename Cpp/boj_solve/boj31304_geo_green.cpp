#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
typedef double ld;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acosl(-1);
const int LEN = 25;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& u, const ld& v) { return zero(u - v); }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
#define LINE 1
#define CIRCLE 2

struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos rot(const ld& the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return norm(atan2(y, x)); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		int i1 = (i + 1) % sz;
		if (ccw(H[i], H[i1], p) < 0) return 0;
	}
	return 1;
}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo, const ld& hi) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
	ld rad(const Pos& p) const { return (p - c).rad(); }
	ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
	ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld tz = (cos(lo) - cos(hi)) * m.y * r;
		return fan + tz - (s / e) * r * r * (ld).5;
	}
};
typedef std::vector<Circle> Disks;
Vld circle_line_intersections(const Seg& l, const Circle& q, const int& t = LINE) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos s = l.s, e = l.e;
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	if (t == LINE) {
		if (0 < lo && lo < 1) ret.push_back(lo);
		if (zero(det)) return ret;
		if (0 < hi && hi < 1) ret.push_back(hi);
	}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		if (zero(det)) return ret;
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
	}
	return ret;
}
struct Arc {
	ld lo, hi;
	Arc(ld l_ = 0, ld h_ = 0) : lo(l_), hi(h_) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
};
typedef std::vector<Arc> Arcs;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	int N, V[LEN], R, W, H;
	std::cin >> N >> R >> W >> H;
	Polygon P(N); for (int i = 0; i < N; i++) std::cin >> P[i] >> V[i];
	int SQ = W * H, sz;
	ld ret = 0;
	Pos p0 = O, p1 = Pos(W, 0), p2 = Pos(W, H), p3 = Pos(0, H);
	Polygon B = { p0, p1, p2, p3 };
	for (int i = 0; i < N; i++) {
		ld A = 0;
		Circle c = Circle(P[i], R);
		Vld tmp = { 0 };
		for (int j = 0; j < 4; j++) {
			Seg s = Seg(B[j], B[(j + 1) % 4]);
			Vld xx = circle_line_intersections(s, c, CIRCLE);
			for (const ld& x : xx) tmp.push_back(x);
			xx = circle_line_intersections(s, c, LINE);
			Vld v = { 0, 1 };
			for (const ld& x : xx) v.push_back(x);
			std::sort(v.begin(), v.end());
			sz = v.size();
			for (int k = 0; k < sz - 1; k++) {
				if (eq(v[k], v[k + 1])) continue;
				ld m = (v[k] + v[k + 1]) * .5;
				Pos mid = s.p(m);
				if (c >= mid) A += s.green(v[k], v[k + 1]);
			}
		}
		std::sort(tmp.begin(), tmp.end());
		tmp.push_back(2 * PI);
		sz = tmp.size();
		for (int j = 0; j < sz - 1; j++) {
			if (eq(tmp[j], tmp[j + 1])) continue;
			ld m = (tmp[j] + tmp[j + 1]) * .5;
			Pos mid = c.p(m);
			if (inner_check(B, mid)) A += c.green(tmp[j], tmp[j + 1]);
		}
		ret += A / SQ * V[i];
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj31304
