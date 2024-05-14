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
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 25;
int N, M, T, Q;
ld A;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
ld norm(ld th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
	return th;
}

//#define DEBUG
//#define ASSERT

int P[LEN * LEN];
struct Info {
	int u, v;
	ld c;
	Info(int u = 0, int v = 0, ld c = 0) : u(u), v(v), c(c) {}
	bool operator < (const Info& x) const { return c > x.c; }
};
int find(int P[], int v) {
	if (P[v] < 0) return v;
	int p = find(P, P[v]);
	return P[v] = p;
}
int join(int P[], int u, int v) {
	int i, j;
	i = find(P, u);
	j = find(P, v);
	if (i == j) return 0;
	if (P[i] < P[j]) {
		P[i] += P[j];
		P[j] = i;
	}
	else {
		P[j] += P[i];
		P[i] = j;
	}
	return 1;
}

struct Pos {
	ld x, y;
	ld t;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { t = norm(atan2(y, x)); }
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
const Pos O = { 0, 0 };
const Pos X_norm = { 1000, 0 };
typedef std::vector<Pos> Polygon;
bool cmpt(const Pos& p, const Pos& q) { return p.t < q.t; }
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
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
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return Pos(
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det
	);
}
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, -1 };
bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	Circle cen;
	Arc(ld LO = 0, ld HI = 0, Circle CEN = Circle(Pos(0, 0), 0)) : lo(LO), hi(HI), cen(CEN) {}
	bool operator < (const Arc& a) const {
		return !sign(hi - a.hi) ?
			lo < a.lo :
			hi < a.hi;
	}
	ld area() const { return (hi - lo) * cen.r * cen.r; }
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.cen.r; return os; }
};
typedef std::vector<Arc> Fan;
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;
	ld J = b * b - 4 * a * c;
	if (J < TOL) return {};
	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);
	if (hi < 0 || 1 < lo) return {};
	return { { lo, hi } };//ratio, ratio
}
ld circle_cutting(const Pos& p1, const Pos& p2, const ld& r) {
	std::vector<Pos> inx = circle_line_intersections(p1, p2, O, r);
	if (inx.empty()) return cross(O, p1, p2);
	ld s = inx[0].x, e = inx[0].y;
	Pos vec = p2 - p1;
	if (0 < s && e < 1) {
		Pos m1 = p1 + vec * s, m2 = p1 + vec * e;
		return cross(O, p1, p2) * (s + 1 - e) + r * r * rad(m1, m2);
	}
	else if (0 < s) {
		Pos m = p1 + vec * s;
		return cross(O, p1, p2) * s + r * r * rad(m, p2);
	}
	else if (e < 1) {
		Pos m = p1 + vec * e;
		return cross(O, p1, p2) * (1 - e) + r * r * rad(p1, m);
	}
	else return r * r * rad(p1, p2);
}
std::vector<Arc> arcs[LEN], valid_arcs[LEN];
Circle disks[LEN];
struct Seg {
	Pos s, e;
	int i;
	Seg(Pos x, Pos y, int I = 0) : s(x), e(y), i(I) {}
	bool operator < (const Seg& S) const {
		return
			!sign((s - S.s).Euc()) ?
			!sign((e - S.e).Euc()) ?
			i < S.i :
			e < S.e :
			s < S.s;
	}
	friend std::ostream& operator << (std::ostream& os, const Seg& S) {
		os << "Seg[" << S.i + 1 << "] :: Seg.s : " << S.s << " Seg.e : " << S.e;
		return os;
	}
	bool connectable(const Seg& S) const {
		if (s == S.e && e == S.s) return 0;
		return s == S.e || e == S.s;
	}
};
Seg make_seg(const ld& lo, const ld& hi, const Circle& c, const int& i) {
	Pos lo = Pos(1, 0).rot(lo) * c.r + c.c;
	Pos hi = Pos(1, 0).rot(hi) * c.r + c.c;
	return Seg(lo, hi, i);
}
typedef std::vector<Seg> Segs;
bool polygon_area_check(Segs& SS) {
	int sz = SS.size();
	if (sz <= 2) return 0;
	std::deque<Seg> dq;
	std::vector<bool> V(sz, 0);
	for (int j = 0; j < sz; j++) {
		for (int i = 0; i < sz; i++) {
			if (V[i]) continue;
			if (!dq.size()) {
				dq.push_back(SS[i]);
				V[i] = 1;
				break;
			}
			else if (dq[0].s == SS[i].e) {
				V[i] = 1;
				dq.push_front(SS[i]);
				break;
			}
			else if (dq.back().e == SS[i].s) {
				V[i] = 1;
				dq.push_back(SS[i]);
				break;
			}
		}
	}
	int t = 0;
	for (int i = 0; i < sz; i++) t += V[i];
	assert(sz == t);
	if (dq[0].s != dq.back().e) return 0;
	ld ret = 0;
	for (Seg& S : dq) ret += cross(O, S.s, S.e);
	return sign(ret) < 0;
}
void query() {
	for (int i = 0; i < LEN; i++)
		arcs[i].clear(), valid_arcs[i].clear();
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> disks[i];
	Segs segs;
	T = 0;
	for (int i = 0; i < N; i++) {
		Circle& disk = disks[i];
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos& ca = disks[i].c, cb = disks[j].c;
			ll ra = disks[i].r, rb = disks[j].r;
			Pos vec = cb - ca;//vec a -> b
			ld distance = vec.mag();
			ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance);
			Pos w = ca + vec * X / distance;
			if (X > 1) continue;
			ld ratio = sqrt(ra * ra - X * X);
			Pos h = ~vec.unit() * X;
			Pos LO = ca + w - h;
			Pos HI = ca + w + h;
			ld lo = rad(ca, LO);
			ld hi = rad(ca, HI);
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
			valid_arcs[i].push_back(Arc(lo, hi, disks[i]));
		}
		std::sort(valid_arcs[i].begin(), valid_arcs[i].end());
		int sz = valid_arcs[i].size();
		for (int k = 0; k < sz; k++) {
			Arc& cur = valid_arcs[i][k], nxt = valid_arcs[i][(k + 1) % sz];
			if (sign(cur.hi - nxt.lo))
				segs.push_back(make_seg(cur.hi, nxt.lo, disk, T++));
		}
	}

	memset(P, -1, sizeof P);
	int sz = segs.size();
	for (int i = 0; i < sz; i++) 
		for (int j = i + 1; j < sz; j++) 
			if (segs[i].connectable(segs[j])) join(P, i, j);

	std::vector<Segs> V_segs(sz);
	for (int i = 0; i < sz; i++) {
		if (P[i] < 0) V_segs[i].push_back(segs[i]);
		else V_segs[P[i]].push_back(segs[i]);
	}

	int cnt = 0;
	for (int i = 0; i < sz; i++) 
		cnt += polygon_area_check(V_segs[i]);
	
	std::cout << cnt << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj7333
