#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 10005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ll sq(const ll& x) { return x * x; }
inline ld sq(const ld& x) { return x * x; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

#define INSIDE 0
#define MEET 1
#define OUTSIDE 2
#define STRONG 0
#define WEAK 1
#define LO x
#define HI y

int T, N, M;
bool F[3];
ld Q[LEN];
int sts[LEN];
struct Pos {
	ld x, y;
	Pos(ld X_ = 0, ld y_ = 0) : x(X_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	ld rad() const { return atan2(y, x); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) < 0) return 0;
	return 1;
}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) return fit(a1, 0, 1);
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
struct Circle {
	Pos c;
	ll r;
	Circle(Pos c_ = Pos(), ll r_ = 0) : c(c_), r(r_) {}
	bool operator > (const Pos& p) const { return sign(r - (c - p).mag()) > 0; }
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
	friend std::istream& operator >> (std::istream& is, Circle& p) { is >> p.c.x >> p.c.y >> p.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& p) { os << p.c.x << " " << p.c.y << " " << p.r; return os; }
} C[3];
struct Arc {
	ld lo, hi;
	int i;
	Arc(ld l_ = 0, ld h_ = 0, int i_ = -1) : lo(l_), hi(h_), i(i_) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline friend std::istream& operator >> (std::istream& is, Arc& a) { is >> a.lo >> a.hi; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Arc& a) { os << a.lo << " " << a.hi; return os; }
};
typedef std::vector<Arc> Arcs;
Arcs AR[3];
Vld intersections(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ld ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	Vld ret = {};
	ret.push_back(norm(rd - h));
	if (zero(h)) return ret;
	ret.push_back(norm(rd + h));
	return ret;
}
void arc_init() {
	for (int i = 0; i < 3; i++) AR[i].clear();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == i) continue;
			Vld inxs = intersections(C[i], C[j]);
			int sz = inxs.size();
			if (sz < 2) continue;
			ld lo = inxs[0], hi = inxs[1];
			if (lo < hi) AR[i].push_back(Arc(lo, hi));
			else {
				AR[i].push_back(Arc(lo, 2 * PI, j));
				AR[i].push_back(Arc(0, hi, j));
			}
		}
		std::sort(AR[i].begin(), AR[i].end());
		AR[i].push_back(Arc(2 * PI, 2 * PI, -2));
	}
	return;
}
ld union_except_x(const int& x) {
	ld ret = 0;
	for (int i = 0; i < 3; i++) {
		if (i == x) continue;
		if (!F[i]) {
			ld hi = 0;
			for (const Arc& a : AR[i]) {
				if (a.i == x) continue;
				if (hi < a.lo) ret += C[i].green(hi, a.lo), hi = a.hi;
				else hi = std::max(hi, a.hi);
			}
		}
	}
	return ret;
}
struct Sphere {
	ll x, y, z, r;
	Sphere(ll x_ = 0, ll y_ = 0, ll z_ = 0, ll r_ = 0) : x(x_), y(y_), z(z_), r(r_) {}
	bool operator < (const Sphere& q) const { return r > q.r; }
	Sphere operator - (const Sphere& q) const { return { x - q.x, y - q.y, z - q.z, 0 }; }
	ll operator * (const Sphere& q) const { return x * q.x + y * q.y + z * q.z; }
	ld vol() const { return (4. / 3) * PI * r * r * r; }
	ld vol(const ld& h) const { return PI * h * h * (3 * r - h) / 3; }
	ld surf() const { return PI * 4 * r * r; }
	ld surf(const ld& h) const { return PI * 2 * r * h; }
} S[3], SP[LEN * 3];
ll Euc(const Sphere& p, const Sphere& q) { return sq(p.x - q.x) + sq(p.y - q.y) + sq(p.z - q.z); }
ld mag(const Sphere& p, const Sphere& q) { return sqrtl(Euc(p, q)); }
ld rad(const Sphere& a, const Sphere& b, const Sphere& c) {
	ld dab = mag(a, b);
	ld dac = mag(a, c);
	ll det = (b - a) * (c - a);
	ld proj = det / dab;
	ld ret = fit(proj / dac, -1, 1);
	return acos(ret);
}
int meet(const Sphere& p, const Sphere& q) {
	ll dist = Euc(p, q);
	ll rout = sq(p.r + q.r);
	if (dist >= rout) return OUTSIDE;
	ll rin = sq(p.r - q.r);
	if (dist <= rin) return INSIDE;
	return MEET;
}
ld cos_2nd(const ld& a, const ld& b, const ld& c) {
	ld num = sq(a) + sq(b) - sq(c);
	ld den = 2 * a * b;
	ld t = num / den;
	return std::abs(acosl(std::min(std::max(t, -(ld)1.0), (ld)1.0)));
}
void spherical_triangle_angles(const ld& a, const ld& b, const ld& c, ld& A_, ld& B_, ld& C_) {
	//std::cout << "a:: " << a << " b:: " << b << " c:: " << c << "\n";
	//std::cout << "sin(a):: " << sin(a) << " sin(b):: " << sin(b) << " sin(c):: " << sin(c) << "\n";
	//std::cout << "acos(" << (cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)) << ")::\n";
	A_ = acos((cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)));
	B_ = acos((cos(b) - cos(a) * cos(c)) / (sin(a) * sin(c)));
	C_ = acos((cos(c) - cos(a) * cos(b)) / (sin(a) * sin(b)));
	//std::cout << "A:: " << A_ << " B:: " << B_ << " C:: " << C_ << "\n";
	return;
}
ld spherical_triangle_angles(const ld& a, const ld& b, const ld& c) {
	//std::cout << "a:: " << a << " b:: " << b << " c:: " << c << "\n";
	//std::cout << "sin(a):: " << sin(a) << " sin(b):: " << sin(b) << " sin(c):: " << sin(c) << "\n";
	//std::cout << "acos(" << (cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)) << ")::\n";
	return acos((cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)));
}
ld area(const ld& a, const ld& b, const ld& c, const ll& r) {
	ld A_, B_, C_;
	spherical_triangle_angles(a, b, c, A_, B_, C_);
	return r * r * (A_ + B_ + C_ - PI);
}
ld get_theta(const ld& a, const ld& A, const ld& b, const ld& d) {
	ld t = asin(fit((sin(A) / sin(a) * sin(b)), -1, 1));
	return t;
}
ld tri_area(const ld& a, const ld& A_, const ld& b, const ld& c, const ll& r, const ld& d) {
	ld B_ = get_theta(a, A_, b, d);
	ld C_ = get_theta(a, A_, c, d);
	return r * r * (A_ + B_ + C_ - PI);
}
ld two_union(const Sphere& a, const Sphere& b) {
	int f = meet(a, b);
	if (f == OUTSIDE) return a.vol() + b.vol();
	if (f == INSIDE) { return a.r >= b.r ? a.vol() : b.vol(); }
	ld d = mag(a, b);
	Pos ca = Pos(0, 0);
	Pos cb = Pos(d, 0);
	Circle aa = Circle(ca, a.r);
	Circle bb = Circle(cb, b.r);
	Vld inxs = intersections(aa, bb);
	assert(inxs.size());
	Pos p = aa.p(inxs[0]);
	ld ha = a.r - p.x;
	ld hb = p.x - d + b.r;
	return a.vol(a.r + a.r - ha) + b.vol(b.r + b.r - hb);
}
ld volume(const ll& r, const Polygon& hp) {
	int sz = hp.size();
	assert(sz);
	if (sz == 1) {
		ld t = norm(hp[0].HI - hp[0].LO) * .5;
		ld d = r * cosl(t);
		ld h = r - d;
		return Sphere(0, 0, 0, r).vol(r + r - h);
	}
	auto inside = [&](const Pos& p, const ld& t) -> bool {
		if (p.LO < p.HI) {
			if (p.LO <= t && t <= p.HI) return 1;
		}
		else {//(p.LO > p.HI)
			if (p.LO <= t || t <= p.HI) return 1;
		}
		return 0;
		};
	auto outer_check = [&](const Pos& p, const Pos& q) -> bool {
		return !inside(p, q.LO) && !inside(p, q.HI);
		};
	auto the = [&](const ld& dd, const ld& rr) -> ld {
		ld w = dd / rr;
		return norm(acosl(fit(w, -1, 1))) * 2;
		};
	auto area_ = [&](const ld& rr, const ld& t) -> ld {
		ld fan = std::abs(rr * rr * (2 * PI - t) * .5);
		ld z = t * .5;
		ld tri = rr * sin(z) * rr * cos(z);
		//ld tri = rr * rr * sin(t) * 5;
		tri = std::abs(tri);
		if (t < PI) return fan + tri;
		return fan - tri;
		};
	auto cone_vol = [&](const ld& rr, const ld& t, const ld& h) -> ld {
		ld rat = area_(rr, t) / (rr * rr * PI);
		ld vol = rr * rr / 3 * rat * PI * h;
		return vol;
		};
	Circle c = Circle(Pos(), r);
	//std::cout << "sz:: " << sz << "\n";
	assert(sz == 2);
	Pos u = hp[0], v = hp[1];
	//std::cout << "u:: " << u.x * 180 / PI << " " << u.y * 180 / PI << "\n";
	//std::cout << "u.t:: " << norm(u.HI - u.LO) << "\n";
	//std::cout << "v:: " << v.x * 180 / PI << " " << v.y * 180 / PI << "\n";
	//std::cout << "v.t:: " << norm(v.HI - v.LO) << "\n";
	ld tu = norm(u.HI - u.LO) * .5;
	ld mu = norm(u.HI + u.LO) * .5;
	//if (u.HI < u.LO) mu = norm(mu + PI);
	if (!inside(u, mu)) mu = norm(mu + PI);
	ld du = r * cosl(tu);
	ld ru = r * sinl(tu);
	ld hu = r - du;
	ld tv = norm(v.HI - v.LO) * .5;
	ld mv = norm(v.HI + v.LO) * .5;
	//if (v.HI < v.LO) mv = norm(mv + PI);
	if (!inside(v, mv)) mv = norm(mv + PI);
	ld dv = r * cosl(tv);
	ld rv = r * sinl(tv);
	ld hv = r - dv;
	if (eq(tu * 2, PI) && eq(tv * 2, PI)) {
		Polygon P;
		for (const Pos& p : hp) {
			if (p.LO < p.HI) P.push_back(p);
			else {
				P.push_back(Pos(0, p.LO));
				P.push_back(Pos(p.HI, 2 * PI));
			}
		}
		std::sort(P.begin(), P.end());
		P.push_back(Pos(2 * PI, 2 * PI));
		ld hi = 0, tt = 0;
		for (const Pos& p : P) {
			if (hi < p.LO) tt += (p.LO - hi), hi = p.HI;
			else hi = std::max(hi, p.HI);
		}
		ld vol = Sphere(0, 0, 0, r).vol();
		return vol * (tt / (2 * PI));
	}
	bool f1 = outer_check(u, v), f2 = outer_check(v, u);
	if (f1 && f2) {
		ld volu = Sphere(0, 0, 0, r).vol(hu);
		ld volv = Sphere(0, 0, 0, r).vol(hv);
		return Sphere(0, 0, 0, r).vol() - volu - volv;
	}
	if (f1) return Sphere(0, 0, 0, r).vol(r + r - hv);
	if (f2) return Sphere(0, 0, 0, r).vol(r + r - hu);
	Pos us = c.p(u.LO), ue = c.p(u.HI);
	Pos vs = c.p(v.LO), ve = c.p(v.HI);
	//if (inside(v, u.LO)) std::swap(us, ue);
	//if (inside(u, v.LO)) std::swap(vs, ve);
	Seg U = Seg(us, ue);
	Seg V = Seg(vs, ve);
	Pos m = intersection(us, ue, vs, ve);
	ld dm = m.mag();
	ld tm = norm(m.rad());
	if (norm(u.HI - u.LO) > PI && norm(v.HI - v.LO) > PI) {
		dm *= -1;
		//tm = norm(tm + PI);
	}
	//std::cout << "dm:: " << dm << "\n";
	//std::cout << "hu:: " << hu << "\n";
	//std::cout << "hv:: " << hv << "\n";
	ld a_ = the(dm, r);// , A;
	//std::cout << "a_:: " << a_ << "\n";
	ld suf = 0;
	ld x;
	x = intersection(U, V);
	//std::cout << "x:: " << x << "\n";
	assert(-TOL < x && x < 1 + TOL);
	//if (inside(v, u.LO)) x = .5 - x;
	//else x = x - .5;
	x = .5 - x;
	if (inside(v, u.HI)) x *= -1;
	//std::cout << "x:: " << x << "\n";
	ld ang_u = the(x, 0.5);
	//std::cout << "ang_u:: " << ang_u * 180 / PI << "\n";
	suf += Sphere(0, 0, 0, r).surf(hu) * ((PI * 2 - ang_u) / (PI * 2));
	x = intersection(V, U);
	//std::cout << "x:: " << x << "\n";
	assert(-TOL < x && x < 1 + TOL);
	//if (inside(u, v.LO)) x = .5 - x;
	//else x = x - .5;
	x = .5 - x;
	if (inside(u, v.HI)) x *= -1;
	//std::cout << "x:: " << x << "\n";
	ld ang_v = the(x, 0.5);
	//std::cout << "ang_v:: " << ang_v * 180 / PI << "\n";
	suf += Sphere(0, 0, 0, r).surf(hv) * ((PI * 2 - ang_v) / (PI * 2));
	//if (ang_u < PI) suf += area(a_, tu, tu, r);
	//else suf -= area(a_, tu, tu, r);
	//if (ang_v < PI) suf += area(a_, tv, tv, r);
	//else suf -= area(a_, tv, tv, r);
	if (ang_u < PI) suf += tri_area(a_, ang_u, tu, tu, r, du);
	else suf -= tri_area(a_, ang_u, tu, tu, r, du);
	if (ang_v < PI) suf += tri_area(a_, ang_v, tv, tv, r, dv);
	else suf -= tri_area(a_, ang_v, tv, tv, r, dv);
	//if (du >= 0) {
	//	if (ang_u < PI) suf += area(a_, tu, tu, r);
	//	else suf -= area(a_, tu, tu, r);
	//}
	//else {
	//	if (ang_u < PI) suf -= area(a_, tu, tu, r);
	//	else suf += area(a_, tu, tu, r);
	//}
	//if (dv >= 0) {
	//	if (ang_v < PI) suf += area(a_, tv, tv, r);
	//	else suf -= area(a_, tv, tv, r);
	//}
	//else {
	//	if (ang_v < PI) suf -= area(a_, tv, tv, r);
	//	else suf += area(a_, tv, tv, r);
	//}
	suf = Sphere(0, 0, 0, r).surf() - suf;
	ld ratio = suf / Sphere(0, 0, 0, r).surf();
	ld total = Sphere(0, 0, 0, r).vol() * ratio;
	//std::cout << "before cone:: " << total << "\n";
	total += cone_vol(ru, ang_u, du);
	//std::cout << "cone1 :: " << total << "\n";
	total += cone_vol(rv, ang_v, dv);
	//std::cout << "cone2 :: " << total << "\n";
	//std::cout << "FUCK::\n";
	return total;
}
void query(const int& q) {
	for (int i = 0; i < 3; i++) {
		std::cin >> S[i].x >> S[i].y >> S[i].z >> S[i].r, F[i] = 0;
		SP[q * 3 + i] = S[i];
	}
	std::sort(S, S + 3);
	assert(S[0].r >= S[1].r && S[1].r >= S[2].r);
	int f01 = meet(S[0], S[1]);
	int f02 = meet(S[0], S[2]);
	int f12 = meet(S[1], S[2]);
	//std::cout << "f01:: " << f01 << " ";
	//std::cout << "f02:: " << f02 << " ";
	//std::cout << "f12:: " << f12 << "\n";
	if (f01 == INSIDE) F[1] = 1;
	if (f02 == INSIDE || f12 == INSIDE) F[2] = 1;
	//if (F[1] && F[2]) { std::cout << S[0].vol() << "\n"; return; }
	//if (F[1]) { std::cout << two_union(S[0], S[2]) << "\n"; return; }
	//if (F[2]) { std::cout << two_union(S[0], S[1]) << "\n"; return; }
	//if (f01 == OUTSIDE && f02 == OUTSIDE) {
	//	ld ret = S[0].vol() + two_union(S[1], S[2]);
	//	std::cout << ret << "\n";
	//	return;
	//}
	//if (f01 == OUTSIDE && f12 == OUTSIDE) {
	//	ld ret = S[1].vol() + two_union(S[0], S[2]);
	//	std::cout << ret << "\n";
	//	return;
	//}
	//if (f02 == OUTSIDE && f12 == OUTSIDE) {
	//	ld ret = S[2].vol() + two_union(S[0], S[1]);
	//	std::cout << ret << "\n";
	//	return;
	//}
	if (F[1] && F[2]) { Q[q] = S[0].vol(); sts[q] = 1; return; }
	if (F[1]) { Q[q] = two_union(S[0], S[2]); sts[q] = 2; return; }
	if (F[2]) { Q[q] = two_union(S[0], S[1]); sts[q] = 2; return; }
	if (f01 == OUTSIDE && f02 == OUTSIDE) {
		ld ret = S[0].vol() + two_union(S[1], S[2]);
		Q[q] = ret;
		sts[q] = 2;
		return;
	}
	if (f01 == OUTSIDE && f12 == OUTSIDE) {
		ld ret = S[1].vol() + two_union(S[0], S[2]);
		Q[q] = ret;
		sts[q] = 2;
		return;
	}
	if (f02 == OUTSIDE && f12 == OUTSIDE) {
		ld ret = S[2].vol() + two_union(S[0], S[1]);
		Q[q] = ret;
		sts[q] = 2;
		return;
	}
	ld dab = mag(S[0], S[1]);
	ld dac = mag(S[0], S[2]);
	//std::cout << "dab:: " << dab << " dac:: " << dac << "\n";
	Pos ca = Pos(0, 0);
	Pos cb = Pos(dab, 0);
	ld t = rad(S[0], S[1], S[2]);
	//std::cout << "t:: " << t << "\n";
	Pos cc = Pos(dac, 0).rot(t);
	C[0] = Circle(ca, S[0].r);
	C[1] = Circle(cb, S[1].r);
	C[2] = Circle(cc, S[2].r);
	//std::cout << "C[0] = " << C[0] << "\n";
	//std::cout << "C[1] = " << C[1] << "\n";
	//std::cout << "C[2] = " << C[2] << "\n";
	//arc_init();
	//ld c_union = union_except_x(-1);
	////std::cout << "circle union:: " << c_union << "\n";
	//for (int i = 2; i >= 0; i--) {
	//	ld tmp = union_except_x(i);
	//	//std::cout << "tmp:: " << tmp << "\n";
	//	ld diff = (c_union - tmp) / c_union;
	//	//std::cout << "diff:: " << diff << "\n";
	//	if (diff < 0.01) {
	//		Q[q] = two_union(S[(i + 1) % 3], S[(i + 2) % 3]);
	//		sts[q] = 2;
	//		return;
	//	}
	//}
	ld ret = 0;
	for (int i = 0; i < 3; i++) {
		Polygon arc, hp;
		for (int j = 0; j < 3; j++) {
			if (i == j) continue;
			Vld inxs = intersections(C[i], C[j]);
			int sz = inxs.size();
			if (sz == 0) continue;
			//std::cout << "sz:: " << sz << "\n";
			//std::cout << "meet:: " << meet(S[i], S[j]) << "\n";
			assert(sz == 2);
			ld lo = inxs[0], hi = inxs[1];
			hp.push_back(Pos(lo, hi));
			if (lo < hi) arc.push_back(Pos(lo, hi));
			else {
				arc.push_back(Pos(lo, 2 * PI));
				arc.push_back(Pos(0, hi));
			}
		}
		std::sort(arc.begin(), arc.end());
		arc.push_back(Pos(2 * PI, 2 * PI));
		ld hi = 0, rnd = 0;
		for (const Pos& p : arc) {
			if (hi < p.LO) rnd += (p.LO - hi), hi = p.HI;
			else hi = std::max(hi, p.HI);
		}
		//std::cout << "rnd:: " << rnd << "\n";
		//if (rnd < TOL) { std::cout << two_union(S[(i + 1) % 3], S[(i + 2) % 3]) << " ::TWO 3\n"; return; }
		if (rnd < TOL) {
			Q[q] = two_union(S[(i + 1) % 3], S[(i + 2) % 3]);
			sts[q] = 2;
			return;
		}
		ret += volume(C[i].r, hp);
		//std::cout << "S[" << i << "]:: " << S[i].x << " " << S[i].y << " " << S[i].z << " " << S[i].r << "\n";
		//std::cout << "vol:: " << volume(C[i].r, hp) << "\n";
	}
	//std::cout << ret << "\n";
	Q[q] = ret;
	sts[q] = 3;
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	freopen("../../../input_data/e/e000.in", "r", stdin);
	freopen("../../../input_data/e/ret.txt", "w", stdout);
	std::cin >> T;
	//while (T--) query();
	for (int q = 0; q < T; q++) query(q);
	//for (int q = 0; q < T; q++) std::cout << Q[q] << "\n";
	int cnt = 0;
	for (int q = 0; q < T; q++) {
		ld ans; std::cin >> ans;
		ld err = (Q[q] - ans) / ans;
		//std::cout << Q[q] << "\n";
		if (err < -1e-6 || 1e-6 < err) {
			cnt++;
			std::cout << "WHAT THE FUCK:: " << cnt << "\n";
			std::cout << "tc:: " << q + 1 << " ::\n";
			std::cout << 1 << "\n";
			std::cout << SP[q * 3 + 0].x << " " << SP[q * 3 + 0].y << " " << SP[q * 3 + 0].z << " " << SP[q * 3 + 0].r << "\n";
			std::cout << SP[q * 3 + 1].x << " " << SP[q * 3 + 1].y << " " << SP[q * 3 + 1].z << " " << SP[q * 3 + 1].r << "\n";
			std::cout << SP[q * 3 + 2].x << " " << SP[q * 3 + 2].y << " " << SP[q * 3 + 2].z << " " << SP[q * 3 + 2].r << "\n";
			std::cout << ans << "\n";
			std::cout << Q[q] << " | err:: ";
			std::cout << ((Q[q] - ans) / ans) << " | state:: ";
			std::cout << sts[q] << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj7874

/*

5
3 7 5 9
1 7 0 9
8 8 1 5
8 4 0 6
4 4 3 10
8 4 1 6
5 1 2 7
5 6 1 10
4 1 3 6
4 9 8 10
5 0 4 4
5 2 5 4
1 4 5 4
1 2 1 7
1 8 0 10
4473.948436313
4225.908317979
4376.564524088
4338.065543957
4514.865527247

1
3 7 5 9
1 7 0 9
8 8 1 5
4473.948436313

1
3 7 5 9
1 7 0 9
8 8 1 5
dm:: 7.580781415
a_:: 1.138486702
x:: 0.912591015
x:: 0.412591015
ang_u:: 68.786098731
x:: 0.443755634
x:: 0.056244366
ang_v:: 167.082399925
dm:: 7.580781415
a_:: 1.138486702
x:: 0.087408985
x:: 0.412591015
ang_u:: 68.786098731
x:: 0.383710409
x:: -0.116289591
ang_v:: 206.897933418
dm:: -1.211712371
a_:: 3.631151799
x:: 0.556244366
x:: 0.056244366
ang_u:: 167.082399925
x:: 0.616289591
x:: -0.116289591
ang_v:: 206.897933418
4473.948436313
4607.828127280
WHAT THE FUCK:: 1
tc:: 1 ::
1
3 7 5 9
1 7 0 9
8 8 1 5
4473.948436313
4607.828127280 | err:: 0.029924281 | state:: 3
*/