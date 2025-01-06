#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <random>
#include <cassert>
#include <array>
#include <tuple>
#include <deque>
#include <queue>
#include <iomanip>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<size_t> Vidx;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const ld ERAD = 6370;
const int LEN = 25 * 25 * 25 * 5;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
#define LINE 1
#define CIRCLE 2
#define STRONG 0
#define WEAK 1
#define LO x
#define HI y
ld sc[4];

int N, T, q;
struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = -1; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return norm(atan2(y, x)); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Polygon ROT[LEN];
bool cmpt(const Pos& p, const Pos& q) {
	bool f1 = O < p;
	bool f2 = O < q;
	if (f1 != f2) return f1;
	return p / q > 0;
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }

int R; ld TH;
ld COST[LEN];
Vint G[LEN];
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Info& x) const { return zero(c - x.c) ? i < x.i : c > x.c; }
};
std::priority_queue<Info> Q;
ld dijkstra(const int& v, const int& g, const int& sz, const ld& limit) {
	for (int i = 0; i < sz; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int i = 0; i < sz; i++) {
			ld w = G[p.i][i];
			if (w > limit) continue;
			ld cost = p.c + w;
			if (COST[i] > cost) {
				COST[i] = cost;
				Q.push({ i, cost });
			}
		}
	}
	return COST[g];
}
struct Pos3D {
	ld x, y, z;
	int r;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) { r = 0; }
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D operator * (const ld& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const ld& n) const { return { x / n, y / n, z / n }; }
	Pos3D& operator *= (const ld& n) { x *= n; y *= n; z *= n; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	Pos3D unit() const { return *this / mag(); }
	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
	Pos3D rodrigues_rotate(const ld& th, const Pos3D& axis) const {
		ld s = sin(th), c = cos(th);
		Pos3D u = axis.unit();
		return u * (*this * u) * (1 - c) + (*this * c) - (*this / u) * s;
	}
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }
};
const Pos3D O3D = { 0, 0, 0 };
const Pos3D MAXP3D = { INF, INF, INF };
typedef std::vector<Pos3D> Polyhedron;
std::vector<Pos3D> pos;
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Plane {
	ld a, b, c, d;
	Plane(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
	Plane& operator *= (const ld& s) { a *= s; b *= s; c *= s; d *= s; return *this; }
	Pos3D norm() const { return Pos3D(a, b, c); };
	Plane operator + (const ld& n) const { return { a, b, c, d + n }; }
	Plane operator - (const ld& n) const { return { a, b, c, d - n }; }
	Plane& operator += (const ld& n) { d += n; return *this; }
	Plane& operator -= (const ld& n) { d -= n; return *this; }
	friend std::istream& operator >> (std::istream& is, Plane& f) { is >> f.a >> f.b >> f.c >> f.d; return is; }
	friend std::ostream& operator << (std::ostream& os, const Plane& f) { os << f.a << " " << f.b << " " << f.c << " " << f.d; return os; }
} knife;
typedef std::vector<Plane> Surfaces;
Plane plane(const Pos3D& p, const ld& n) { return Plane(p.x, p.y, p.z, n); }
void update_sc(const Plane& p) {
	ld angle1 = -atan2l(p.b, p.a);
	ld dx = sqrtl(p.a * p.a + p.b * p.b);
	ld angle2 = -atan2l(dx, p.c);
	sc[0] = sinl(angle1);
	sc[1] = cosl(angle1);
	sc[2] = sinl(angle2);
	sc[3] = cosl(angle2);
	return;
}
void update_sc(const Pos3D& p) {
	ld angle1 = -atan2l(p.y, p.x);
	ld dx = sqrtl(p.x * p.x + p.y * p.y);
	ld angle2 = -atan2l(dx, p.z);
	sc[0] = sinl(angle1);
	sc[1] = cosl(angle1);
	sc[2] = sinl(angle2);
	sc[3] = cosl(angle2);
	return;
}
Pos3D rotate(const Pos3D& p) {
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	return Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
}
Pos convert(Pos3D p, const Pos3D& v) {
	p -= v;
	p = rotate(p);
	return Pos(p.x, p.y);
}
Pos3D recover(const Pos& p2D, const Pos3D& v) {
	ld x = p2D.x * -sc[3];
	ld y = p2D.y;
	ld z = p2D.x * sc[2];
	Pos3D p = Pos3D(x * -sc[1] + y * sc[0], x * sc[0] + y * sc[1], z);
	return p + v;
}
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) { return sign(cross(d1, d2, d3) * norm); }
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return zero(cross(d1, d2, d3).mag()) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return zero(cross(d1, d2, d3).mag()) && sign(dot(d1, d3, d2)) > 0; }
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
ld dist(const Plane& s, const Pos3D& p) { return (s.norm() * p + s.d) / s.norm().mag(); }
Pos3D offset(const Plane& s, const Pos3D& p) { ld d = dist(s, p); return s.norm().unit() * -d; }
Pos3D intersection(const Plane& S, const Line3D& l) {
	ld det = S.norm() * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = -((S.norm() * l.p0 + S.d) / det);
	return l.p0 + (l.dir * t);
}
Pos3D intersection(const Plane& S, const Pos3D& p1, const Pos3D& p2, const bool& f = 0) {
	Line3D l = L(p1, p2);
	Pos3D inx = intersection(S, l);
	if (f && !on_seg_strong(p1, p2, inx)) return { INF, INF, INF };
	return inx;
}
int intersection(const Plane& p1, const Plane& p2, Line3D& l) {
	Pos3D n1 = p1.norm();
	Pos3D n2 = p2.norm();
	Pos3D dir = n2 / n1;
	dir = dir.unit();
	if (zero(dir.mag())) {
		ld f = n1 * n2;
		ld d1 = dist(p1, O3D);
		ld d2 = dist(p2, O3D);
		if (sign(f) > 0) return sign(d2 - d1) >= 0 ? 0 : -1;
		else return sign(d2 + d1) >= 0 ? 0 : -2;
	}
	Pos3D q1 = intersection(p1, Line3D(n1, O3D));
	Pos3D v1 = n1 / dir;
	Pos3D p0 = intersection(p2, Line3D(v1, q1));
	l = Line3D(dir, p0);
	return 1;
}
Pos3D s2c(const ld& phi, const ld& psi) {//Spherical to Cartesian
	ld lon = phi * PI / 180;
	ld lat = psi * PI / 180;
	return Pos3D(cos(lon) * cos(lat), sin(lon) * cos(lat), sin(lat));
}
bool circle_intersection(const Pos3D& a, const Pos3D& b, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D mid = (a + b) * .5;
	if (zero(mid.mag())) return 0;
	ld x = cos(th) / mid.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = mid.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (mid / (b - a)).unit() * ratio;
	inxs.push_back(w + h);
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D vec = a - (perp * (perp * a));
	if (zero(vec.mag())) return 0;
	ld th = (ld)a.r / R;
	ld x = cos(th) / vec.mag();
	if (x < -1 || 1 < x) return 0;
	Pos3D w = vec.unit() * x;
	ld ratio = sqrtl(1 - x * x);
	Pos3D h = (vec.unit() / perp) * ratio;
	inxs.push_back(w + h);
	if (!zero(ratio)) inxs.push_back(w - h);
	return 1;
}
Pos3D point(const Pos3D Xaxis, const Pos3D Yaxis, const ld& th) {
	return Xaxis * cos(th) + Yaxis * sin(th);
}
ld angle(const Pos3D Xaxis, const Pos3D Yaxis, const Pos3D& p) {
	ld X = Xaxis * p;
	ld Y = Yaxis * p;
	ld th = atan2(Y, X);
	return th;
}
bool inner_check(const Pos3D& d1, const Pos3D& d2, const Pos3D& t, const Pos3D& nrm) {
	return ccw(O3D, d1, t, nrm) >= 0 && ccw(O3D, d2, t, nrm) <= 0;
}
bool connectable(const Polyhedron& P, const Pos3D& a, const Pos3D& b, const int& i, const int& j) {
	if (a == b) return 1;
	Pos3D perp = (a / b).unit();
	Pos3D X = a.unit();//X-axis
	Pos3D Y = (perp / a).unit();//Y-axis
	ld ang = angle(X, Y, b);
	std::vector<Info> tmp = { { 0, 0 }, { 0, ang } };
	std::vector<Pos3D> inxs;
	for (int k = 0; k < N; k++) {//sweeping
		if (k == i || k == j) continue;
		if (plane_circle_intersection(P[k], perp, inxs)) {
			for (Pos3D& p : inxs) {
				Pos3D axis = (P[k] / perp).unit();
				Pos3D mid = (perp / axis).unit();
				ld th = angle(X, Y, p);
				if (0 <= th && th <= ang) return 0;
			}
		}
	}
	return 1;
}
Pos3D VX[LEN]; int vp;
Info ND[LEN]; int np;
void connect(const Polygon& H, const int& r) {
	assert(vp == np);
	for (int i = 0; i < np; i++) {
		for (int j = 0; j < np; j++) {
			if (i == j) continue;

		}
	}
	for (int i = 0; i < N; i++) {
		Pos cen = H[i];
		if (ccw(H[(i - 1 + N) % N], H[i], H[(i + 1) % N]) > 0) continue;
		for (Pos& p : ROT[i]) p -= cen;
		std::sort(ROT[i].begin(), ROT[i].end(), cmpt);
		for (Pos& p : ROT[i]) p += cen;
		int sz = ROT[i].size();
		for (int j = 0; j < sz; j++) {
			const Pos& s = ROT[i][j], & e = ROT[i][(j + 1) % sz];
		}
	}
	return;
}
bool inner_check(const Pos3D& p, const Pos3D& q) {
	ld ang = angle(p, q, p);
	ld t1 = (ld)p.r / R;
	ld t2 = (ld)q.r / R;
	assert(t1 <= PI * .5); assert(t2 <= PI * .5);
	if (p.r > q.r && std::abs(t1 - t2) > ang) return 1;
	return 0;
}
Polyhedron intersections(const Pos3D& p, const Pos3D& q) {
	ld ang = angle(p, q, p);
	ld t1 = (ld)p.r / R;
	ld t2 = (ld)q.r / R;
	assert(t1 <= PI * .5); assert(t2 <= PI * .5);
	if (t1 + t2 < ang || std::abs(t1 - t2) > ang) return {};
	ld d1 = cos(t1);
	Pos3D n1 = p * d1;
	Plane p1 = plane(p, d1);
	ld d2 = cos(t2);
	Pos3D n2 = q * d2;
	Plane p2 = plane(q, d2);
	Line3D inx;
	if (intersection(p1, p2, inx) != 1) return {};
	ld w = (n1 - inx.p0).mag();
	ld h = sqrt(sq(sin(t1)) - sq(w));
	Pos3D v = inx.dir;
	return { inx.p0 + v * h, inx.p0 - v * h };
}
Polyhedron tangents(const Pos3D& p, const Pos3D& q) {
	Polyhedron ret = {};
	ld A = angle(p, q, p);
	ld t1 = (ld)p.r / R;
	ld t2 = (ld)q.r / R;
	if (std::abs(t1 - t2) > A) return {};
	ld d1 = cos(t1);
	Pos3D n1 = p * d1;
	Plane p1 = plane(p, d1);
	ld d2 = cos(t2);
	Pos3D n2 = q * d2;
	Plane p2 = plane(q, d2);
	Pos3D perp = n2 / n1;
	Pos3D v1 = (n1 / perp).unit() * sin(t1);
	Pos3D pp = n1 + v1;
	Pos3D v2 = (perp / n2).unit() * sin(t2);
	Pos3D qq = n2 + v2;
	if (t1 + t2 < A) {
		ld B = t1 + t2;
		ld a = PI * .5;
		ld det = sin(a) / sin(A);
		ld b = asin(det * sin(B));
		b = PI * .5 - b;
		Pos3D u1 = pp.rodrigues_rotate(b, n1);
		Pos3D w1 = qq.rodrigues_rotate(b, n2);
		ret.push_back(u1); ret.push_back(w1);
		Pos3D u2 = pp.rodrigues_rotate(-b, n1);
		Pos3D w2 = qq.rodrigues_rotate(-b, n2);
		ret.push_back(u2); ret.push_back(w2);
	}
	ld B = std::abs(t1 - t2);
	ld a = PI * .5;
	ld det = sin(a) / sin(A);
	ld b = asin(det * sin(B));
	Pos3D u1, w1, u2, w2;
	if (p.r == q.r) {
		Pos3D u1 = pp.rodrigues_rotate(PI * .5, n1);
		Pos3D w1 = qq.rodrigues_rotate(PI * -.5, n2);
		ret.push_back(u1); ret.push_back(w1);
		Pos3D u2 = pp.rodrigues_rotate(PI * -.5, n1);
		Pos3D w2 = qq.rodrigues_rotate(PI * .5, n2);
		ret.push_back(u2); ret.push_back(w2);
	}
	else if (p.r < q.r) {
		Pos3D u1 = pp.rodrigues_rotate(PI * .5 + b, n1);
		Pos3D w1 = qq.rodrigues_rotate(PI * -.5 + b, n2);
		ret.push_back(u1); ret.push_back(w1);
		Pos3D u2 = pp.rodrigues_rotate(PI * -.5 - b, n1);
		Pos3D w2 = qq.rodrigues_rotate(PI * .5 - b, n2);
		ret.push_back(u2); ret.push_back(w2);
	}
	else {
		Pos3D u1 = pp.rodrigues_rotate(PI * .5 - b, n1);
		Pos3D w1 = qq.rodrigues_rotate(PI * -.5 - b, n2);
		ret.push_back(u1); ret.push_back(w1);
		Pos3D u2 = pp.rodrigues_rotate(PI * -.5 + b, n1);
		Pos3D w2 = qq.rodrigues_rotate(PI * .5 + b, n2);
		ret.push_back(u2); ret.push_back(w2);
	}
	return ret;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> R >> N;
	Polyhedron P(N);
	Pos3D s, e;
	int phi, psi, r;
	for (Pos3D& p : P) {
		std::cin >> phi >> psi >> r;
		p = s2c(phi, psi); p.r = r;
	}
	std::cin >> phi >> psi;
	s = s2c(phi, psi); s.r = -1;
	std::cin >> phi >> psi;
	e = s2c(phi, psi); e.r = -1;
	std::sort(P.begin(), P.end(), [&](const Pos3D& p, const Pos3D& q) -> bool { return p.r > q.r; });
	Vbool F(N);
	for (int i = 0; i < N; i++) 
		for (int j = i + 1; j < N; j++) 
			if (inner_check(P[i], P[j]) || P[i] == P[j]) F[j] = 1;
	Polyhedron tmp;
	for (int i = 0; i < N; i++) if (!F[i]) tmp.push_back(P[i]);
	P = tmp;
	Polyhedron inxs;
	for (int i = 0; i < N; i++) {
		ld t1 = (ld)P[i].r / R;
		ld d1 = cos(t1);
		Pos3D n1 = P[i] * d1;
		for (int j = i + 1; j < N; j++) {
			ld t2 = (ld)P[j].r / R;
			ld d2 = cos(t2);
			Pos3D n2 = P[j] * d2;
			Polyhedron inxs = intersections(P[i], P[j]);
			if (inxs.size()) {
				Pos3D l_ = inxs[0], h_ = inxs[1];
				update_sc(P[i]);
				Pos cl = convert(l_, n1);
				Pos ch = convert(h_, n1);
				ld lo = cl.rad();
				ld hi = ch.rad();
				if (eq(lo, hi)) { lo = norm(lo - TOL); hi = norm(hi + TOL); }
				update_sc(P[j]);
				cl = convert(h_, n2);
				ch = convert(l_, n2);
				lo = cl.rad();
				hi = ch.rad();
				if (eq(lo, hi)) { lo = norm(lo - TOL); hi = norm(hi + TOL); }
			}
			Polyhedron tans = tangents(P[i], P[j]);
			if (tans.size()) {
				int sz = tans.size();
				for (int k = 0; k < sz; k += 2) {
					Pos3D I = inxs[k + 0], J = inxs[k + 1];
					if (connectable(P, P[i], P[j], i, j)) {
						update_sc(P[i]);
						Pos ii = convert(I, n1);
						ld ti = ii.rad();
						update_sc(P[j]);
						Pos jj = convert(J, n2);
						ld tj = jj.rad();
					}
				}
			}
		}
	}


	return;
}
int main() { solve(); return 0; }//boj23623 In search of the chair
