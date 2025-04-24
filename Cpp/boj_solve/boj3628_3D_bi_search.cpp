#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acos(-1);
const int LEN = 25;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::max(lo, std::min(hi, x)); }

#define LO x
#define HI y

int N;
struct Pos {
	ld x, y;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
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
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
struct Pos3D {
	ld x, y, z;
	Pos3D(ld x_ = 0, ld y_ = 0, ld z_ = 0) : x(x_), y(y_), z(z_) {}
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
	Pos3D operator * (const ld& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const ld& n) const { return { x / n, y / n, z / n }; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	Pos3D unit() const { return *this / mag(); }
}; const Pos3D O = { 0, 0, 0 };
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
Pos3D project(const Pos3D& p, const Pos3D& q) {
	Pos3D v = (q / p).unit();
	return (p / v).unit();
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
	for (int i = 0; i < sz; i++) {
		Polygon R = { Pos(0, 0) };
		const Pos3D& p = P[i];
		update_sc(P[i]);
		bool f = 0;
		for (int j = 0; j < sz; j++) {
			if (j == i) continue;
			const Pos3D& q = P[j];
			ld t = rad(p, q);
			if (inner_check(r, t)) { f = 1; break; }
			Polyhedron inxs;
			circle_intersection(p, q, r, inxs);
			if (inxs.size() == 2) {
				Pos3D s = project(p, inxs[0]);
				Pos3D e = project(p, inxs[1]);
				Pos3D m_ = project(p, q);
				Pos u = convert(s);
				Pos v = convert(e);
				Pos m = convert(m_);
				if (ccw(m, u, v) == 0) continue;
				if (ccw(m, u, v) > 0) std::swap(u, v);
				ld lo = u.rad();
				ld hi = v.rad();
				if (hi < lo) {
					R.push_back(Pos(lo, 2 * PI));
					R.push_back(Pos(0, hi));
				}
				else R.push_back(Pos(lo, hi));
				
			}
		}
		if (f) continue;
		std::sort(R.begin(), R.end());
		R.push_back(Pos(2 * PI, 2 * PI));
		ld hi = 0;
		for (const Pos& p : R) {
			if (hi < p.LO) return 0;
			else hi = std::max(hi, p.HI);
		}
	}
	return 1;
}
ld bi_search(const Polyhedron& P) {
	ld s = TOL, e = PI - TOL;
	int cnt = 25;
	while (cnt--) {
		ld m = (s + e) * .5;
		if (check(P, m)) e = m;
		else s = m;
	}
	return (s + e) * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	//freopen("knockdown.in", "r", stdin);
	//freopen("knockdown.out", "w", stdout);
	std::cin >> N;
	Polyhedron P(N);
	int lon, lat;
	for (Pos3D& p : P) { std::cin >> lat >> lon; p = s2c(lon, lat); }//unit
	std::cout << bi_search(P) << "\n";
	return;
}
int main() { solve(); return 0; }//boj3628
