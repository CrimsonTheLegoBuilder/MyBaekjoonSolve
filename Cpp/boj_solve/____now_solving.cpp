#define _CRT_SECURE_NO_WARNINGS
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
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo = 0, const ld& hi = 1) { return std::min(hi, std::max(lo, x)); }

//#define DEBUG
//#define ASSERT

int B, N, M, T, Q;
ld A;
struct Pos {
	ld x, y;
	ld t;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) { t = norm(atan2(y, x)); }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	//ld mag() const { return hypotl(x, y); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2l(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
bool cmpt(const Pos& p, const Pos& q) { return p.t < q.t; }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d1, d3) / (d2 - d1).Euc(); }
//bool inside(const Pos& p0, const Pos& p1, const Pos& p2, const Pos& q, const int& f = STRONG) {
//	if (ccw(p0, p1, p2) < 0) return ccw(p0, p1, q) >= f || ccw(p1, p2, q) >= f;
//	return ccw(p0, p1, q) >= f && ccw(p1, p2, q) >= f;
//}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
bool inner_check_concave(const std::vector<Pos>& H, const Pos& p) {
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		//if (on_seg_strong(cur, nxt, p)) {
		//	assert(collinear(cur, nxt, s, e));
		//	return dot(cur, nxt, s, e) > 0 ? 1 : 0;
		//}
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(0, 0), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	//bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return sign(r - (c - p).mag()) >= 0; }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	//ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
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
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	Circle cen;
	Arc(ld l_ = 0, ld h_ = 0, Circle c_ = Circle(Pos(0, 0), 0)) : lo(l_), hi(h_), cen(c_) {}
	bool operator < (const Arc& a) const {
		return !sign(hi - a.hi) ?
			lo < a.lo :
			hi < a.hi;
	}
	ld area() const { return (hi - lo) * cen.r * cen.r; }
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.cen.r; return os; }
};
std::vector<Arc> arcs[LEN], valid_arcs[LEN];
//Circle disks[LEN];
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos s_, Pos e_, int i_ = 0) : s(s_), e(e_), i(i_) {}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "Seg[" << S.i + 1 << "] :: Seg.s : " << S.s << " Seg.e : " << S.e;
		return os;
	}
};
Seg make_seg(const ld& lo, const ld& hi, const Circle& c, const int& i) {
	Pos LO = Pos(1, 0).rot(lo) * c.r + c.c;
	Pos HI = Pos(1, 0).rot(hi) * c.r + c.c;
	return Seg(LO, HI, i);
}
typedef std::vector<Seg> Segs;
Polygon TRAP[LEN * LEN];
bool query() {//brute O(N^4)
	for (int i = 0; i < LEN; i++) {
		arcs[i].clear();
		valid_arcs[i].clear();
	}
	std::cin >> B;
	Polygon P(B); for (Pos& p : P) std::cin >> p;
	std::cin >> N;
	Polygon I(N); for (Pos& p : I) std::cin >> p;
	std::cin >> M;
	Disks C(M); for (Circle& c : C) std::cin >> c;
	if (!B && !N && !M) return 0;
	Segs segs;
	Vbool F(N, 1);
	for (int i = 0; i < N; i++) {
		for (Circle& c : C) {
			if (c >= I[i]) { F[i] = 0; break; }
		}
	}
	for (int i = 0; i < M; i++) {
		Circle& disk = C[i];
		for (int j = 0; j < M; j++) {
			if (i == j) continue;
			Pos& ca = C[i].c, cb = C[j].c;
			ll ra = C[i].r, rb = C[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			if (X < 0 || X > ra) continue;
			Pos w = vec * X / distance;
			ld ratio = sqrt(ra * ra - X * X);
			Pos h = ~vec.unit() * ratio;
			Pos LO = w - h;
			Pos HI = w + h;
			ld lo = LO.rad();
			ld hi = HI.rad();
			Arc a1, a2;
			if (lo <= hi) {
				a1 = Arc(lo, hi, disk);
				arcs[i].push_back(a1);
			}
			else {
				a1 = Arc(lo, PI * 2, disk);
				a2 = Arc(0, hi, disk);
				arcs[i].push_back(a1);
				arcs[i].push_back(a2);
			}
		}
		std::sort(arcs[i].begin(), arcs[i].end());
		for (Arc& a : arcs[i]) {//sweep circle
			ld lo = a.lo;
			ld hi = a.hi;
			while (valid_arcs[i].size()) {
				if (sign(valid_arcs[i].back().hi - a.lo) <= 0) break;
				else {
					lo = valid_arcs[i].back().hi;
					break;
				}
			}
			valid_arcs[i].push_back(Arc(lo, hi, C[i]));
		}
		int sz = valid_arcs[i].size();
		for (int k = 0; k < sz; k++) {
			Arc& cur = valid_arcs[i][k], nxt = valid_arcs[i][(k + 1) % sz];
			segs.push_back(make_seg(cur.hi, nxt.lo, disk, i));
		}
	}

	int sz = segs.size();
	T = 0;
	int n = 0;
	bool f = 0;
	std::deque<Seg> dq;
	std::vector<bool> V(sz, 0);
	M = 0;
	while (T < sz) {
		f = 0;
		for (int i = 0; i < sz; i++) {
			if (V[i]) continue;
			if (!dq.size()) {
				dq.push_back(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
			else if (dq[0].s == segs[i].e && dq[0].i != segs[i].i) {
				dq.push_front(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
			else if (dq.back().e == segs[i].s && dq.back().i != segs[i].i) {
				dq.push_back(segs[i]);
				V[i] = 1;
				f = 1;
				T++;
				break;
			}
		}
		if (dq[0].s == dq.back().e) {
			ld ret = 0;
			TRAP[n].clear();
			for (Seg& S : dq) {
				TRAP[n].push_back(S.s);
				ret += cross(O, S.s, S.e);
			}
			n++;
			dq.clear();
		}
		if (f == 0) dq.clear();
	}
	Vint alive;
	for (int j = 0; j < n; j++) {
		for (const int& i : alive) {
			if (inner_check_concave(TRAP[j], P[i])) {

			}
		}
	}
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	while (query());//total O(N^5)
	return;
}
int main() { solve(); return 0; }//5752