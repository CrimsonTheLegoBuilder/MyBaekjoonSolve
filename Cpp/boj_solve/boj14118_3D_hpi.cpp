#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld EPS = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
int gcd(int a, int b) { while (b) { int tmp = a % b; a = b; b = tmp; } return a; }
int gcd(int a, int b, int c) { int x = gcd(a, b); return gcd(x, c); }
int gcd(int a, int b, int c, int d) { a = std::abs(a); b = std::abs(b); c = std::abs(c); d = std::abs(d); int x = gcd(a, b, c); return gcd(x, c); }

int N, M, T, Q;
ld sc[4];
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
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
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret * .5;
}
struct Linear {//ps[0] -> ps[1]
	Pos ps[2];
	Pos dir_;
	Pos& operator[](int i) { return ps[i]; }
	Pos dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		dir_ = (ps[1] - ps[0]).unit();
	}
	bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> Planes;
ld dist(const Pos& d1, const Pos& d2, const Pos& t) { return cross(d1, d2, t) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
Pos projection(const Pos& s1, const Pos& s2, const Pos& p) { return intersection(s1, s2, p, p + ~(s2 - s1)); }
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Pos> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return HPI;
}
Planes make_hp(const Polygon& H) {
	Planes V;
	int sz = H.size();
	for (int i = 0; i < sz; i++) V.push_back(Linear(H[i], H[(i + 1) % sz]));
	return V;
}
struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
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
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x - p.x; y - p.y; z - p.z; return *this; }
	Pos3D operator * (const ld& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const ld& n) const { return { x / n, y / n, z / n }; }
	Pos3D& operator *= (const ld& n) { x * n; y * n; z * n; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	Pos3D unit() const { return *this / mag(); }
	Pos3D norm(const Pos3D& p) const { return (*this / p).unit(); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }

};
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
const Pos3D INVAL3D = { INF, INF, INF };
typedef std::vector<Pos3D> Polygon3D;
typedef std::vector<Polygon3D> Polyhedron;
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Plane {
	ld a, b, c, d;
	Plane(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
	Plane& operator *= (const ld& s) { a *= s; b *= s; c *= s; d *= s; return *this; }
	Pos3D norm() const { return Pos3D(a, b, c); };
	Plane& operator += (const ld& n) { d += n; return *this; }
	friend std::istream& operator >> (std::istream& is, Plane& f) { is >> f.a >> f.b >> f.c >> f.d; return is; }
	friend std::ostream& operator << (std::ostream& os, const Plane& f) { os << f.a << " " << f.b << " " << f.c << " " << f.d; return os; }
} knife;
typedef std::vector<Plane> Surfaces;
int above(const Plane& S, const Pos3D& p) { return sign(p * S.norm() + S.d); }
//ld randTOL() {
//	ld rand01 = rand() / (ld)RAND_MAX;
//	ld err = (rand01 - .5) * TOL;
//	return err;
//}
//Pos3D add_noise(const Pos3D& p) {
//	ld rand01 = rand() / (ld)RAND_MAX;
//	ld err = (rand01 - .5) * TOL;
//	return p + Pos3D(randTOL(), randTOL(), randTOL());
//}
void update_sc(const Plane& p) {
	ld angle1 = -atan2(p.b, p.a);
	ld dx = sqrtl(p.a * p.a + p.b * p.b);
	ld angle2 = -atan2(dx, p.c);
	sc[0] = sin(angle1);
	sc[1] = cos(angle1);
	sc[2] = sin(angle2);
	sc[3] = cos(angle2);
	return;
}
Pos3D rotate(const Pos3D& p) {
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	return Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
}
Pos convert(Pos3D p, const Pos3D& v) {
	std::cout << "pppp:: " << p << "\n";
	p = rotate(p - v);
	std::cout << "conv:: " << p << "\n";
	return Pos(p.x, p.y);
}
Pos3D recover(const Pos& p2D, const Pos3D& v) {
	ld x = p2D.x / sc[3];
	ld y = p2D.y;
	ld z = p2D.x / -sc[2];
	Pos3D p = Pos3D(x * -sc[1] + y * sc[0], x * -sc[0] + y * -sc[1], z);
	return p + v;
}
typedef std::vector<Pos3D> Polygon3D;
typedef std::vector<Polygon3D> Polyhedron;
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
	//std::cout << "intersection::\n";
	//std::cout << "t:: " << t << "\n";
	//std::cout << "S:: " << S << "\n";
	//std::cout << "l:: dir:: " << l.dir << " p0:: " << l.p0 << "\n";
	//std::cout << "inx:: " << l.p0 + (l.dir * t) << "\n";
 	//std::cout << "intersection::\n";
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
		//ld d1 = p1.d;
		//ld d2 = p2.d;
		//std::cout << "d1:: " << d1 << " d2:: " << d2 << "\n";
		if (sign(f) > 0) return sign(d2 - d1) >= 0 ? 0 : -1;
		else {
			//if (zero(d1)) return sign(d2) >= 0 ? 0 : -1;
			//if (zero(d2)) return sign(d1) >= 0 ? 0 : -1;
			//if (sign(d1) < 0 && sign(d2) < 0) return -2;
			//if (sign(d1) > 0 && sign(d2) > 0) return 0;
			return sign(d2 + d1) >= 0 ? 0 : -2;
		}
	}
	Pos3D q1 = intersection(p1, Line3D(n1, O3D));
	Pos3D v1 = n1 / dir;
	Pos3D p0 = intersection(p2, Line3D(v1, q1));
	//std::cout << "p1:: " << p1 << "\n";
	//std::cout << "p2:: " << p2 << "\n";
	//std::cout << "q1:: " << q1 << "\n";
	//std::cout << "v1:: " << v1 << "\n";
	//std::cout << "dir:: " << dir << "\n";
	//std::cout << "p0:: " << p0 << "\n";
	l = Line3D(dir, p0);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	Surfaces S(N);
	for (Plane& p : S) {
		int a, b, c, d;
		std::cin >> a >> b >> c >> d;
		int x = gcd(a, b, c, d);
		a /= x; b /= x; c /= x; d /= x;
		a *= -1; b *= -1; c *= -1;// d *= -1;
		p = Plane(a, b, c, d);
		p += EPS;
	}
	ld bnd = 1e7;
	Pos p1 = Pos(-bnd, -bnd),
		p2 = Pos(bnd, -bnd),
		p3 = Pos(bnd, bnd),
		p4 = Pos(-bnd, bnd);
	Planes B = { 
		Linear(p1, p2),
		Linear(p2, p3),
		Linear(p3, p4),
		Linear(p4, p1)
	};
	Pos3D q;
	bool f0 = 0;
	for (int i = 0; i < N; i++) {
		//std::cout << "\nLOOP_START:::\n";
		//std::cout << "LOOP_START:::\n";
		//std::cout << "LOOP_START:::\n";
		update_sc(S[i]);
		Pos3D v = offset(S[i], O3D);
		//std::cout << "dist:: " << dist(S[i], O3D) << "\n";
		std::cout << "v:: " << v << "\n";
		Pos3D w = { 1, 1, 1 };
		Pos w1 = convert(w, v);
		std::cout << w1 << "\n";
		std::cout << recover(w1, v) << " recover:: { 1, 1, 1 } \n";
		Line3D l;
		int f = 1;
		Planes hp = B;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			f = intersection(S[i], S[j], l);
			//std::cout << f << " ::f ::\n";
			//std::cout << S[i].norm() * S[j].norm() << " ::n1 * n2 ::\n";
			if (f == -2) { std::cout << "banana\n"; return; }
			if (f < 0) break;
			if (f == 0) continue;
			//std::cout << "S[" << j << "]:: " << S[j] << "\n";
			//std::cout << "l.p0:: " << l.p0 << "\nl.dir:: " << l.dir << "\n";
			std::cout << "dist:: i:: " << dist(S[i], l.p0) << "\n";
			std::cout << "dist:: j:: " << dist(S[j], l.p0) << "\n";
			std::cout << "dot:: i:: " << S[i].norm() * l.dir << "\n";
			std::cout << "dot:: j:: " << S[j].norm() * l.dir << "\n";
			std::cout << "mag:: l:: " << sign(l.dir.mag()) << "\n";
			//std::cout << "v:: " << v << "\n";
			Pos s = convert(l.p0, v);
			Pos e = convert(l.p0 + l.dir, v);
			//std::cout << "s:: " << s << "\ne:: " << e << "\n";
			//std::cout << "vec:: " << e - s << "\n";
			hp.push_back(Linear(s, e));
		}
		if (f == -1) continue;
		//std::cout << "hpi\n";
		Polygon hpi = half_plane_intersection(hp);
		if (!hpi.size()) continue;
		//std::cout << "hpii\n";
		//std::cout << recover(hpi[0], S[i], v) << "\n";
		//std::cout << hpi[0] << "\n";
		//std::cout << "hpi:: \n";
		//for (const Pos& p : hpi) std::cout << p << "\n";
		//std::cout << "hpi:: \n";
		q = recover(hpi[0], v);
		f0 = 1;
		break;
		//return;
	}
	if (f0) {
		std::cout << q << "\n";
		//q = Pos3D(3.0, 5.0, 2.0);
		//q = Pos3D(1.5, 2.0, -5.0);
		for (const Plane& p : S) {
			std::cout << "dist:: " << dist(p, q) << "\n";
		}
	}
	else std::cout << "banana\n";
	//q = Pos3D(2, 0, 2);
	//for (const Plane& p : S) {
	//	std::cout << "dist:: " << dist(p, q) << "\n";
	//}
	return;
}
int main() { solve(); return 0; }//boj19508 
