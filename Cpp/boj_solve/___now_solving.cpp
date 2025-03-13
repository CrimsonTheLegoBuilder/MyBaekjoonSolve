#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
//#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acos(-1);
const ld ERAD = 6370;
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
ld flip(ld lat) {
	if (zero(lat - PI * .5) || zero(lat + PI * .5)) return 0;
	if (zero(lat)) return PI * .5;
	if (lat > 0) return PI * .5 - lat;
	if (lat < 0) return -(PI * .5) - lat;
	return INF;
}
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::max(lo, std::min(hi, x)); }

#define LO x
#define HI y

int N, T, q;
ld R, TH;
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
};
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
struct Pos3D {
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	//bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	//bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
	//bool operator < (const Pos3D& p) const { return zero(x - p.x) ? zero(y - p.y) ? z < p.z : y < p.y : x < p.x; }
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
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	//Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	//Pos3D& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	Pos3D unit() const { return *this / mag(); }
};
const Pos3D O = { 0, 0, 0 };
const Pos3D MAXP3D = { INF, INF, INF };
typedef std::vector<Pos3D> Polyhedron;
Pos3D s2c(const ld& lon, const ld& lat) {//Spherical to Cartesian
	ld phi = lon * PI / 180;
	ld the = lat * PI / 180;
	return Pos3D(cos(phi) * cos(the), sin(phi) * cos(the), sin(the));
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
bool plane_circle_intersection(const Pos3D& a, const Pos3D& perp, const ld& th, std::vector<Pos3D>& inxs) {
	inxs.clear();
	Pos3D vec = a - (perp * (perp * a));
	if (zero(vec.mag())) return 0;
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
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool inner_check(const Pos3D& d1, const Pos3D& d2, const Pos3D& t, const Pos3D& nrm) {
	return ccw(O, d1, t, nrm) >= 0 && ccw(O, d2, t, nrm) <= 0;
}
bool connectable(const std::vector<Pos3D>& P, const Pos3D& a, const Pos3D& b, const ld& th) {
	if (zero((a - b).mag())) return 1;
	if (zero((a + b).mag()) && R > ERAD * PI * .5 - TOL) return 1;
	Pos3D perp = (a / b).unit();
	Pos3D X = a.unit();//X-axis
	Pos3D Y = (perp / a).unit();//Y-axis
	ld ang = angle(X, Y, b);
	return 1;
}
ld sc[4];
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
Pos convert(Pos3D p, const Pos3D& v = Pos3D(0, 0, 0)) { p = rotate(p - v); return Pos(p.x, p.y); }
Pos convert(Pos3D q, const Pos3D& p, const Pos3D& v) { update_sc(p); return convert(q, v); }
Pos3D recover(const Pos& p2D, const Pos3D& v) {
	ld x = p2D.x * -sc[3];
	ld y = p2D.y;
	ld z = p2D.x * sc[2];
	Pos3D p = Pos3D(x * -sc[1] + y * sc[0], x * sc[0] + y * sc[1], z);
	return p + v;
}
Pos3D project(const Pos3D& p, const Pos3D& q) {
	Pos3D v = (q / p).unit();
	Pos3D prj = (p / v).unit();
	return prj;
}
ld rad(const Pos3D& p, const Pos3D& q) { return norm(acos(fit((p * q) / q.mag(), -1, 1))); }
bool inner_check(const ld& r, const ld& t) {
	if (r < PI * .5) return 0;
	ld r_ = PI - r;
	return t >= r_ * 2;
}
bool check(const Polyhedron& P, const ld& r) {
	int sz = P.size();
	ld d = cos(r);
	//std::cout << "\nr:: " << r << " ";
	//std::cout << "d:: " << d << "\n";
	for (int i = 0; i < sz; i++) {
		Polygon R = { Pos(0, 0) };
		const Pos3D& p = P[i];
		//Pos3D ci = p * d;
		update_sc(P[i]);
		bool f = 0;
		//std::cout << "i:: " << i << "\n";
		for (int j = 0; j < sz; j++) {
			if (j == i) continue;
			const Pos3D& q = P[j];    
			ld t = rad(p, q);
			//std::cout << "t:: " << t << "\n";
			//std::cout << "p*q:: " << p * q << "\n";
			//std::cout << "q.mag:: " << q.mag() << "\n";
			//std::cout << "cos:: " << (p * q) / q.mag() << "\n";
			//std::cout << "acos:: " << acos((p * q) / q.mag()) << "\n";
			//std::cout << "rad(p, q):: " << rad(p, q) * 180 / PI << "\n";
			if (inner_check(r, t)) { f = 1; break; }
			//Pos3D cj = q * d;
			Polyhedron inxs;
			bool f0 = circle_intersection(p, q, r, inxs);
			if (inxs.size() == 2) {
				//std::cout << "2:: FUCK::\n";
				Pos3D s = project(p, inxs[0]);
				Pos3D e = project(p, inxs[1]);
				Pos3D m_ = project(p, q);
				Pos u = convert(s);
				Pos v = convert(e);
				Pos m = convert(m_);
				//std::cout << "cross:: " << cross(m, u, v) << "\n";
				if (ccw(m, u, v) == 0) continue;
				if (ccw(m, u, v) > 0) std::swap(u, v);
				ld lo = u.rad();
				ld hi = v.rad();
				//std::cout << "lo:: " << lo << " hi:: " << hi << " ";
				//std::cout << "m:: " << m.rad() << "\n";
				if (hi < lo) {
					//std::cout << "rvs::\n";
					R.push_back(Pos(lo, 2 * PI));
					R.push_back(Pos(0, hi));
				}
				else {
					R.push_back(Pos(lo, hi));
				}
			}
			//std::cout << "FUCK::\n";
		}
		//std::cout << "SWEEP::\n";
		//std::cout << "f:: " << f << "\n";
		if (f) continue;
		std::sort(R.begin(), R.end());
		R.push_back(Pos(2 * PI, 2 * PI));
		ld hi = 0;
		for (const Pos& p : R) {
			if (hi < p.LO) return 0;
			else hi = std::max(hi, p.HI);
		}
		//std::cout << "good::\n";
	}
	return 1;
}
ld bi_search(const Polyhedron& P) {
	ld s = TOL, e = PI - TOL;
	int cnt = 50;
	while (cnt--) {
		ld m = (s + e) * .5;
		if (check(P, m)) e = m;
		else s = m;
		//std::cout << "FUCK::\n";
	}
	return (s + e) * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(30);
	std::cin >> N;
	Polyhedron P(N);
	int lon, lat;
	for (Pos3D& p : P) { std::cin >> lat >> lon; p = s2c(lon, lat); }//unit
	std::cout << bi_search(P) << "\n";
	return;
}
int main() { solve(); return 0; }//boj3628

/*

2
0 0
0 45
2.356194490

2
0 0
0 90
2.356194490

*/