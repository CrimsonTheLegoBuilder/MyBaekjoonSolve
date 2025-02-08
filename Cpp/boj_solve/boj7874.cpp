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
ld spherical_triangle_angles(const ld& a, const ld& b, const ld& c) {
	//std::cout << "a:: " << a << " b:: " << b << " c:: " << c << "\n";
	//std::cout << "sin(a):: " << sin(a) << " sin(b):: " << sin(b) << " sin(c):: " << sin(c) << "\n";
	//std::cout << "acos(" << (cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)) << ")::\n";
	return acos((cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)));
}
void spherical_triangle_angles(const ld& a, const ld& b, const ld& c, ld& A_, ld& B_, ld& C_) {
	//std::cout << "a:: " << a << " b:: " << b << " c:: " << c << "\n";
	//std::cout << "sin(a):: " << sin(a) << " sin(b):: " << sin(b) << " sin(c):: " << sin(c) << "\n";
	//std::cout << "acos(" << (cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)) << ")::\n";
	A_ = acos(fit((cos(a) - cos(b) * cos(c)) / (sin(b) * sin(c)), -1, 1));
	B_ = acos(fit((cos(b) - cos(a) * cos(c)) / (sin(a) * sin(c)), -1, 1));
	C_ = acos(fit((cos(c) - cos(a) * cos(b)) / (sin(a) * sin(b)), -1, 1));
	//std::cout << "A:: " << A_ << " B:: " << B_ << " C:: " << C_ << "\n";
	return;
}
ld area(const ld& a, const ld& b, const ld& c, const ll& r, const ld& t) {
	ld A_, B_, C_;
	if (a >= PI) {
		spherical_triangle_angles(a * .5, t, c, A_, B_, C_);
		return r * r * (A_ + B_ + C_ - PI) * 2;
	}
	spherical_triangle_angles(a, b, c, A_, B_, C_);
	return r * r * (A_ + B_ + C_ - PI);
}
//ld get_theta(const ld& a, const ld& A, const ld& b, const ld& d) {
//	ld t = asin(fit((sin(A) / sin(a) * sin(b)), -1, 1));
//	return t;
//}
//ld tri_area(const ld& a, const ld& A_, const ld& b, const ld& c, const ll& r, const ld& d) {
//	ld B_ = get_theta(a, A_, b, d);
//	ld C_ = get_theta(a, A_, c, d);
//	return r * r * (A_ + B_ + C_ - PI);
//}
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
			if (p.LO < t && t < p.HI) return 1;
		}
		else {//(p.LO > p.HI)
			if (p.LO < t || t < p.HI) return 1;
		}
		return 0;
		};
	auto outer_check = [&](const Pos& p, const Pos& q) -> bool {
		return !inside(p, q.LO) && !inside(p, q.HI);
		};
	auto the = [&](const ld& dd, const ld& rr) -> ld {
		ld w = dd / rr;
		assert(std::abs(dd) <= std::abs(rr));
		return norm(acosl(fit(w, -1, 1))) * 2;
		};
	auto area_ = [&](const ld& rr, const ld& t) -> ld {
		ld fan = std::abs(rr * rr * (2 * PI - t) * .5);
		ld z = t * .5;
		ld tri = rr * sin(z) * rr * cos(z);
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
	std::cout << "u:: " << u.x * 180 / PI << " " << u.y * 180 / PI << "\n";
	std::cout << "u.t:: " << norm(u.HI - u.LO) * 180 / PI << "\n";
	std::cout << "v:: " << v.x * 180 / PI << " " << v.y * 180 / PI << "\n";
	std::cout << "v.t:: " << norm(v.HI - v.LO) * 180 / PI << "\n";
	ld tu = norm(u.HI - u.LO) * .5;
	ld mu = norm(u.HI + u.LO) * .5;
	if (!inside(u, mu)) mu = norm(mu + PI);
	ld du = r * cosl(tu);
	ld ru = r * sinl(tu);
	ld hu = r - du;
	ld tv = norm(v.HI - v.LO) * .5;
	ld mv = norm(v.HI + v.LO) * .5;
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
	if (eq(tu * 2, PI) || eq(tv * 2, PI)) {
		//std::cout << "FUCK::\n";
		bool f = 1;
		Pos u_ = eq(tu * 2, PI) ? u : v;
		Pos v_ = eq(tu * 2, PI) ? v : u;
		assert(u_ != v_);
		//ld suf = Sphere(0, 0, 0, r).surf() * .5, x;
		ld suf = 0, x;
		mv = norm(v_.HI + v_.LO) * .5;
		if (!inside(v_, mv)) mv = norm(mv + PI);
		if (eq(u_.LO, mv) || eq(u_.HI, mv)) {
			//std::cout << "FUCK1::\n";
			tv = norm(v_.HI - v_.LO) * .5;
			dv = r * cosl(tv);
			rv = r * sinl(tv);
			hv = r - dv;
			return Sphere(0, 0, 0, r).vol(r + r - hv) * .5;
		}
		if (inside(u_, mv)) {
			//std::cout << "FUCK2::\n";
			f = 0;
			std::swap(v_.LO, v_.HI);
		}
		mv = norm(v_.HI + v_.LO) * .5;
		if (!inside(v_, mv)) mv = norm(mv + PI);
		Pos us = c.p(u_.LO), ue = c.p(u_.HI);
		Pos vs = c.p(v_.LO), ve = c.p(v_.HI);
		Seg U = Seg(us, ue);
		Seg V = Seg(vs, ve);
		assert(!inside(u_, mv));
		tv = norm(v_.HI - v_.LO) * .5;
		std::cout << "tv||PI:: " << tv * 2 * 180 / PI << "\n";
		dv = r * cosl(tv);
		rv = r * sinl(tv);
		hv = r - dv;
		x = intersection(U, V);
		assert(-TOL < x && x < 1 + TOL);
		x = .5 - x;
		if (inside(v_, u_.HI)) x *= -1;
		//if (dv < 0) x *= -1;
		//x *= -1;
		assert(x != 0);
		ld a_ = the(x, r);
		x = intersection(V, U);
		assert(-TOL < x && x < 1 + TOL);
		x = .5 - x;
		if (inside(u_, v_.HI)) x *= -1;
		ld ang_v = the(x, 0.5);
		suf += Sphere(0, 0, 0, r).surf(hv) * ((PI * 2 - ang_v) / (PI * 2));
		//ld ttv = 0;
		//if (inside(u_, v_.HI)) ttv = std::min(norm(u_.LO - mv), norm(mv - u_.LO));
		//else ttv = std::min(norm(u_.HI - mv), norm(mv - u_.HI));
		ld ttv = std::min(norm(u_.HI - mv), norm(mv - u_.HI));
		ttv = std::min({ ttv, norm(u_.LO - mv), norm(mv - u_.LO) });
		ld tri = area(a_, tv, tv, r, ttv);
		suf += tri;
		//suf = Sphere(0, 0, 0, r).surf() - suf;
		//ld ratio = suf / Sphere(0, 0, 0, r).surf();
		//ld total = Sphere(0, 0, 0, r).vol() * ratio;
		suf = (Sphere(0, 0, 0, r).surf() * .5) - suf;
		ld ratio = suf / (Sphere(0, 0, 0, r).surf() * .5);
		ld total = Sphere(0, 0, 0, r).vol() * .5 * ratio;
		total += cone_vol(rv, ang_v, dv);
		return f ? total : (Sphere(0, 0, 0, r).vol() * .5 - total);
		//return total;
	}
	Pos us = c.p(u.LO), ue = c.p(u.HI);
	Pos vs = c.p(v.LO), ve = c.p(v.HI);
	Seg U = Seg(us, ue);
	Seg V = Seg(vs, ve);
	Pos m = intersection(us, ue, vs, ve);
	ld dm = m.mag();
	ld tm = norm(m.rad());
	//std::cout << "m - O " << m - O << "\n";
	//std::cout << "tm:: " << tm * 180 / PI << "\n";
	//if (du <= 0 && dv <= 0) {
	if (du < TOL && dv < TOL) {
		dm *= -1;
		tm = norm(tm + PI);
	}
	//std::cout << "tm:: " << tm * 180 / PI << "\n";
	//ld ttu = std::abs(norm(tm - u.LO) - tu);
	ld ttu = std::min(norm(tm - mu), norm(mu - tm));
	//ld ttv = std::abs(norm(tm - v.LO) - tv);
	ld ttv = std::min(norm(tm - mv), norm(mv - tm));
	//std::cout << "ttu:: " << ttu * 180 / PI << "\n";
	//std::cout << "ttv:: " << ttv * 180 / PI << "\n";
	//std::cout << "dm:: " << dm << "\n";
	//std::cout << "hu:: " << hu << "\n";
	//std::cout << "hv:: " << hv << "\n";
	//std::cout << "du:: " << du << "\n";
	//std::cout << "dv:: " << dv << "\n";
	ld a_ = the(dm, r);
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
	//if (eq(du, 0)) suf += Sphere(0, 0, 0, r).surf() * .5;
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
	//if (eq(dv, 0)) suf += Sphere(0, 0, 0, r).surf() * .5;
	suf += Sphere(0, 0, 0, r).surf(hv) * ((PI * 2 - ang_v) / (PI * 2));
	int su = 0;
	if ((inside(Pos(u.LO, mu), tm) && inside(v, u.HI)) ||
		(inside(Pos(mu, u.HI), tm) && inside(v, u.LO))) su = -1;
	else su = 1;
	if (eq(tm, mu)) su = 0;
	int sv = 0;
	if ((inside(Pos(v.LO, mv), tm) && inside(u, v.HI)) ||
		(inside(Pos(mv, v.HI), tm) && inside(u, v.LO))) sv = -1;
	else sv = 1;
	if (eq(tm, mv)) sv = 0;
	x = intersection(U, V);
	if (du < 0 && dv > 0 && (
		(inside(v, u.LO) && x > .5) ||
		(inside(v, u.HI) && x < .5)
		)) suf += (Sphere(0, 0, 0, r).surf() - area(a_, tu, tu, r, ttu)) * su;
	else suf += area(a_, tu, tu, r, ttu) * su;
	x = intersection(V, U);
	if (dv < 0 && du > 0 && (
		(inside(u, v.LO) && x > .5) ||
		(inside(u, v.HI) && x < .5)
		)) suf += (Sphere(0, 0, 0, r).surf() - area(a_, tv, tv, r, ttv)) * sv;
	else suf += area(a_, tv, tv, r, ttv) * sv;
	suf = Sphere(0, 0, 0, r).surf() - suf;
	ld ratio = suf / Sphere(0, 0, 0, r).surf();
	ld total = Sphere(0, 0, 0, r).vol() * ratio;
	total += cone_vol(ru, ang_u, du);
	total += cone_vol(rv, ang_v, dv);
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
		ld vol = volume(C[i].r, hp);
		std::cout << "vol:: " << vol << "\n";
		ret += vol;
		//std::cout << "S[" << i << "]:: " << S[i].x << " " << S[i].y << " " << S[i].z << " " << S[i].r << "\n";
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
	std::cout.precision(15);
	//freopen("../../../input_data/e/e000.in", "r", stdin);
	//freopen("../../../input_data/e/ret.txt", "w", stdout);
	std::cin >> T;
	//while (T--) query();
	for (int q = 0; q < T; q++) query(q);
	//for (int q = 0; q < T; q++) std::cout << Q[q] << "\n";
	int cnt = 0;
	for (int q = 0; q < T; q++) {
		ld ans; std::cin >> ans;
		ld err = (Q[q] - ans) / Q[q];
		//std::cout << std::abs(Q[q]) << "\n";
		bool f = std::isnan(Q[q]);
		if (f || err <= -1e-6 || 1e-6 <= err) {
			cnt++;
			std::cout << "WHAT THE FUCK:: " << cnt << "\n";
			std::cout << "tc:: " << q + 1 << " ::\n";
			std::cout << 1 << "\n";
			std::cout << SP[q * 3 + 0].x << " " << SP[q * 3 + 0].y << " " << SP[q * 3 + 0].z << " " << SP[q * 3 + 0].r << "\n";
			std::cout << SP[q * 3 + 1].x << " " << SP[q * 3 + 1].y << " " << SP[q * 3 + 1].z << " " << SP[q * 3 + 1].r << "\n";
			std::cout << SP[q * 3 + 2].x << " " << SP[q * 3 + 2].y << " " << SP[q * 3 + 2].z << " " << SP[q * 3 + 2].r << "\n";
			std::cout << "" << ans << "\n";
			std::cout << "" << Q[q] << " | err:: ";
			std::cout << ((Q[q] - ans) / ans) << " | state:: ";
			std::cout << sts[q] << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj7874

/*

10

0 5 3 10
1 2 8 10
1 8 6 9

3 0 7 8
0 7 3 4
4 6 7 7

5 3 9 6
3 5 4 6
5 0 3 9

6 2 8 6
6 6 6 4
5 7 3 6

8 6 7 4
2 8 2 9
4 6 9 5

5 4 9 8
5 8 8 9
6 8 4 9

4 5 4 9
0 4 8 10
6 7 7 8

0 3 6 6
2 1 4 7
1 8 3 10

3 7 4 9
1 2 8 6
2 3 3 6

7 0 0 7
5 5 2 4
7 9 4 7
6495.663204638
2936.823069518
3528.088552436
1619.919989260
3339.454073934
4566.371994545
5622.480587850
4706.541063698
3418.846912320
2708.071106011


WHAT THE FUCK:: 1
tc:: 1016 ::
1
6 8 3 2
4 8 4 2
2 0 7 10
4206.885955961700347
4203.771553956073149 | err:: -0.000740310538063 | state:: 3
WHAT THE FUCK:: 2
tc:: 1040 ::
1
0 5 3 10
1 2 8 10
1 8 6 9
6495.663204638300158
6451.960261148306017 | err:: -0.006728018696965 | state:: 3
WHAT THE FUCK:: 3
tc:: 1088 ::
1
3 0 7 8
0 7 3 4
4 6 7 7
2936.823069518000011
2920.902707154424661 | err:: -0.005420947052894 | state:: 3
WHAT THE FUCK:: 4
tc:: 1097 ::
1
5 3 9 6
3 5 4 6
5 0 3 9
3528.088552436000100
3447.326403559420669 | err:: -0.022891190988054 | state:: 3
WHAT THE FUCK:: 5
tc:: 1164 ::
1
7 7 1 7
3 6 3 9
8 3 5 4
3353.812564650500008
3341.285795882780803 | err:: -0.003735083140827 | state:: 3
WHAT THE FUCK:: 6
tc:: 1176 ::
1
5 1 4 4
2 4 6 8
8 1 4 5
2393.646578056299859
2463.061632972864572 | err:: 0.028999709294148 | state:: 3
WHAT THE FUCK:: 7
tc:: 1295 ::
1
6 2 8 6
6 6 6 4
5 7 3 6
1619.919989260100010
1632.227082465124795 | err:: 0.007597346342177 | state:: 3
WHAT THE FUCK:: 8
tc:: 1312 ::
1
3 0 2 9
1 5 6 6
4 1 3 9
3709.538298286299778
3611.688557913360455 | err:: -0.026377875763715 | state:: 3
WHAT THE FUCK:: 9
tc:: 1313 ::
1
8 6 7 4
2 8 2 9
4 6 9 5
3339.454073933799918
3330.706324155044058 | err:: -0.002619514922225 | state:: 3
WHAT THE FUCK:: 10
tc:: 1392 ::
1
1 9 9 6
9 2 9 9
0 8 6 5
3906.080642653800169
3847.740004168937503 | err:: -0.014935851003124 | state:: 3
WHAT THE FUCK:: 11
tc:: 1394 ::
1
5 4 9 8
5 8 8 9
6 8 4 9
4566.371994545100279
4481.080125371411668 | err:: -0.018678256890936 | state:: 3
WHAT THE FUCK:: 12
tc:: 1405 ::
1
4 5 4 9
0 4 8 10
6 7 7 8
5622.480587849699987
5609.292779231411259 | err:: -0.002345549871135 | state:: 3
WHAT THE FUCK:: 13
tc:: 1440 ::
1
6 4 9 2
0 6 9 7
4 9 5 7
2360.846498980400156
2358.527468468942516 | err:: -0.000982287714368 | state:: 3
WHAT THE FUCK:: 14
tc:: 1550 ::
1
3 3 9 8
4 4 4 8
7 6 2 9
4561.000097865899988
4728.270916267221764 | err:: 0.036674153653184 | state:: 3
WHAT THE FUCK:: 15
tc:: 1562 ::
1
9 6 4 8
3 5 6 9
9 5 9 6
4181.454725219599823
4118.344581040793855 | err:: -0.015092867991173 | state:: 3
WHAT THE FUCK:: 16
tc:: 1642 ::
1
0 3 6 6
2 1 4 7
1 8 3 10
4706.541063697900427
4551.078591725339720 | err:: -0.033031151724493 | state:: 3
WHAT THE FUCK:: 17
tc:: 1653 ::
1
7 8 4 5
5 6 8 7
9 3 8 9
3493.548477125400041
3443.921533595303117 | err:: -0.014205311263043 | state:: 3
WHAT THE FUCK:: 18
tc:: 1674 ::
1
4 6 7 9
6 8 4 8
7 5 2 6
3667.225829590800004
3685.612849759834262 | err:: 0.005013877253119 | state:: 3
WHAT THE FUCK:: 19
tc:: 1752 ::
1
3 7 4 9
1 2 8 6
2 3 3 6
3418.846912320499996
3330.954515530726439 | err:: -0.025708199005061 | state:: 3
WHAT THE FUCK:: 20
tc:: 1793 ::
1
2 1 7 5
2 3 4 8
6 3 3 9
3586.203059618199859
3600.377858674823983 | err:: 0.003952592427416 | state:: 3
WHAT THE FUCK:: 21
tc:: 1815 ::
1
0 1 0 8
9 4 5 9
8 1 2 10
5982.687673596199602
5032.547338065868644 | err:: -0.158814965341355 | state:: 3
WHAT THE FUCK:: 22
tc:: 1845 ::
1
7 0 0 7
5 5 2 4
7 9 4 7
2708.071106011199845
2715.408181974607487 | err:: 0.002709336526327 | state:: 3


*/
