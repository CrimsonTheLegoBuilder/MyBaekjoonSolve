#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-10;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

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
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	Vec operator ~ () const { return { -vx, vy }; }
	ld mag() const { return hypot(vy, vx); }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
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
	bool same_dir(const Line& l) const { return zero(*this / l) && *this * l > 0; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld dist(const Pos& p) const { return above(p) / mag(); }
	ld mag() const { return s.mag(); }
	bool include(const Pos& p) const { return above(p) < 0; }
	friend inline ld rad(const Line& b, const Line& l) { return atan2(b / l, b * l); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
typedef std::vector<Line> Lines;
struct Linear {//ps[0] -> ps[1] :: refer to bulijiojiodibuliduo
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
	Linear push() const {//push eps outward
		const double eps = 1e-8;
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator + (const double eps) const {//push eps outward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] + delta, ps[1] + delta);
	}
	Linear operator - (const double eps) const {//pull eps inward
		Pos delta = ~(ps[1] - ps[0]).unit() * eps;
		return Linear(ps[0] - delta, ps[1] - delta);
	}
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> Linears;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return sign(ret);
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Pos intersection(Linear& l1, Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {//refer to bulijiojiodibuliduo
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
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
void norm(Polygon& H, const bool& f = 1) {
	if (f && sign(area(H)) < 0) std::reverse(H.begin(), H.end());
	if (!f && sign(area(H)) > 0) std::reverse(H.begin(), H.end());
	return;
}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
//	auto cw = [&](const Line& l1, const Line& l2, const Line& target) -> bool {
//		if (l1.s / l2.s < TOL) return 0;
//		Pos p = intersection(l1, l2);
//		return sign(target.above(p)) >= 0;
//		};
//	std::deque<Line> dq;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!dq.empty() && zero(dq.back() / l)) continue;
//		while (dq.size() >= 2 && cw(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
//		while (dq.size() >= 2 && cw(l, dq.front(), dq[1])) dq.pop_front();
//		dq.push_back(l);
//	}
//	while (dq.size() >= 3 && cw(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
//	while (dq.size() >= 3 && cw(dq.back(), dq.front(), dq[1])) dq.pop_front();
//	for (int i = 0; i < (int)dq.size(); i++) {
//		Line cur = dq[i], nxt = dq[(i + 1) % (int)dq.size()];
//		if (cur / nxt < TOL) {
//			hull.clear();
//			return 0;
//		}
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	auto check = [&](Line& u, Line& v, Line& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Line> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && HP[i].same_dir(HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return 0;
	for (int i = 0; i < sz; ++i) hull.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return 1;
}
bool polygon_intersection(const Polygon& H1, const Polygon& H2, Polygon& hull) {
	int sz;
	Lines hp;
	sz = H1.size();
	for (int i = 0; i < sz; i++) hp.push_back(L(H1[i], H1[(i + 1) % sz]));
	sz = H2.size();
	for (int i = 0; i < sz; i++) hp.push_back(L(H2[i], H2[(i + 1) % sz]));
	return half_plane_intersection(hp, hull);
}
Polygon polygon_intersection(const Polygon& H1, const Polygon& H2) {
	int sz;
	Linears hp;
	sz = H1.size();
	for (int i = 0; i < sz; i++) hp.push_back(Linear(H1[i], H1[(i + 1) % sz]));
	sz = H2.size();
	for (int i = 0; i < sz; i++) hp.push_back(Linear(H2[i], H2[(i + 1) % sz]));
	return half_plane_intersection(hp);
}
Pos centroid(const Polygon& H) {
	Pos cen = Pos(0, 0);
	ld A = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		ld a = H[i] / H[(i + 1) % sz];
		cen += (H[i] + H[(i + 1) % sz]) * a;
		A += a;
	}
	A *= .5;
	cen /= 6;
	if (!zero(A)) cen /= A;
	return cen;
}
void query() {
	//std::cout << ++M << "\n";
	std::cin >> N;
	Polygon H0(N), H1, H2;
	for (Pos& p : H0) std::cin >> p;
	//std::cout << N << "\n";
	//for (Pos& p : H0) std::cout << "(" << p.x << ", " << p.y << "),\n";
	norm(H0);
	ld a, b, c;
	std::cin >> a >> b >> c;
	//std::cout << a << " " << b << " " << c << "\n";
	Line l = { { a, b }, -c };
	Lines hp1, hp2;
	for (int i = 0; i < N; i++) hp1.push_back(L(H0[i], H0[(i + 1) % N]));
	hp2 = hp1;
	hp1.push_back(l);
	hp2.push_back(l * -1);

	ld v = 0;
	bool f1, f2;
	f1 = half_plane_intersection(hp1, H1);
	f2 = half_plane_intersection(hp2, H2);
	if (f1 && f2) {
		//std::cout << "zzz\n";
		Polygon H3, tmp;
		//std::cout << "A = [\n";
		//for (Pos& p : H1) std::cout << "(" << p.x << ", " << p.y << "),\n";
		//std::cout << "]\n";
		Pos vec = Pos(l.s.vy, l.s.vx).unit();
		int sz = H1.size();
		for (int i = 0; i < sz; i++) {
			//std::cout << "DEBUG:: " << H1[i] << " " << l.dist(H1[i]) << "\n";
			Pos mr = H1[i] + vec * -l.dist(H1[i]) * 2;
			tmp.push_back(mr);
		}
		norm(tmp);
 	//	bool f3 = polygon_intersection(tmp, H2, H3);
		//if (!f3) { std::cout << "err\n"; return; }
		H3 = polygon_intersection(tmp, H2);
		//std::cout << tmp.size() << " " << H2.size() << " " << H3.size() << "\n";
		//for (Pos& p : tmp) std::cout << p << " ";
		//std::cout << "\n";
		//std::cout << "B = [\n";
		//for (Pos& p : H2) std::cout << "(" << p.x << ", " << p.y << "),\n";
		//std::cout << "]\n";
		//std::cout << "\n";
		//for (Pos& p : H2) std::cout << p << " ";
		//for (Pos& p : H3) std::cout << p << " ";
		//std::cout << "C = [\n";
		//for (Pos& p : H3) std::cout << "(" << p.x << ", " << p.y << "),\n";
		//std::cout << "]\n";
		//std::cout << "\n";
		Pos cen;
		cen = centroid(H1);
		v += 2 * PI * area(H1) * std::abs(l.dist(cen));
		//std::cout << "v::" << v << "\n";
		cen = centroid(H2);
		v += 2 * PI * area(H2) * std::abs(l.dist(cen));
		//std::cout << "v::" << v << "\n";
		cen = centroid(H3);
		v -= 2 * PI * area(H3) * std::abs(l.dist(cen));
		//std::cout << "v::" << v << "\n";
	}
	else {
		Polygon& H3 = f1 ? H1 : H2;
		Pos cen = centroid(H3);
		v = 2 * PI * area(H3) * std::abs(l.dist(cen));
	}
	std::cout << v << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(1);
	//freopen("pol.in", "r", stdin);
	//freopen("pol_my.out", "w", stdout);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj22785 Polygon Revolution

/*

1
5
10 15
14 13
12 9
8 9
6 13
0 1 -10

1
10
10000.0 5000.0
9045.1 2061.1
6545.1 244.7
3454.9 244.7
954.9 2061.1
0.0 5000.0
954.9 7938.9
3454.9 9755.3
6545.1 9755.3
9045.1 7938.9
2.5 9.7 -60822.0

503210627676.1

488259039917.1

1
10
10000.0000000000 5000.0000000000
9045.0849718747 2061.0737385376
6545.0849718747 244.7174185242
3454.9150281253 244.7174185242
954.9150281253 2061.0737385376
0.0000000000 5000.0000000000
954.9150281253 7938.9262614624
3454.9150281253 9755.2825814758
6545.0849718747 9755.2825814758
9045.0849718747 7938.9262614624
2.5477706749 9.7139154346 -60821.9840097030

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

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
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	Vec operator ~ () const { return { -vx, vy }; }
	ld mag() const { return hypot(vy, vx); }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
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
	bool same_dir(const Line& l) const { return zero(*this / l) && *this * l > 0; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld dist(const Pos& p) const { return above(p) / mag(); }
	ld mag() const { return s.mag(); }
	bool include(const Pos& p) const { return above(p) < 0; }
	friend inline ld rad(const Line& b, const Line& l) { return atan2(b / l, b * l); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
typedef std::vector<Line> Lines;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ld ret = cross(d1, d2, d3); return sign(ret); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld area(const Polygon& H) {
	ld a = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) a += H[i] / H[(i + 1) % sz];
	return a * .5;
}
void norm(Polygon& H, const bool& f = 1) {
	if (f && sign(area(H)) < 0) std::reverse(H.begin(), H.end());
	if (!f && sign(area(H)) > 0) std::reverse(H.begin(), H.end());
	return;
}
bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
	auto check = [&](Line& u, Line& v, Line& w) -> bool {
		return w.include(intersection(u, v));
		};
	std::sort(HP.begin(), HP.end());
	std::deque<Line> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && HP[i].same_dir(HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return 0;
	for (int i = 0; i < sz; ++i) hull.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	return 1;
}
bool polygon_intersection(const Polygon& H1, const Polygon& H2, Polygon& hull) {
	int sz;
	Lines hp;
	sz = H1.size();
	for (int i = 0; i < sz; i++) hp.push_back(L(H1[i], H1[(i + 1) % sz]));
	sz = H2.size();
	for (int i = 0; i < sz; i++) hp.push_back(L(H2[i], H2[(i + 1) % sz]));
	return half_plane_intersection(hp, hull);
}
Pos centroid(const Polygon& H) {
	Pos cen = Pos(0, 0);
	ld A = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		ld a = H[i] / H[(i + 1) % sz];
		cen += (H[i] + H[(i + 1) % sz]) * a;
		A += a;
	}
	A *= .5;
	cen /= 6;
	if (!zero(A)) cen /= A;
	return cen;
}
void query() {
	std::cin >> N;
	Polygon H0(N), H1, H2;
	for (Pos& p : H0) std::cin >> p;
	norm(H0);
	ld a, b, c;
	std::cin >> a >> b >> c;
	Line l = { { a, b }, -c };
	Lines hp1, hp2;
	for (int i = 0; i < N; i++) hp1.push_back(L(H0[i], H0[(i + 1) % N]));
	hp2 = hp1;
	hp1.push_back(l);
	hp2.push_back(l * -1);

	ld v = 0;
	bool f1, f2;
	f1 = half_plane_intersection(hp1, H1);
	f2 = half_plane_intersection(hp2, H2);
	if (f1 && f2) {
		Polygon H3, tmp;
		Pos vec = Pos(l.s.vy, l.s.vx).unit();
		int sz = H1.size();
		for (int i = 0; i < sz; i++) {
			Pos mr = H1[i] + vec * -l.dist(H1[i]) * 2;
			tmp.push_back(mr);
		}
		norm(tmp);
		bool f3 = polygon_intersection(tmp, H2, H3);
		if (!f3) { std::cout << "err\n"; return; }
		Pos cen;
		cen = centroid(H1);
		v += 2 * PI * area(H1) * std::abs(l.dist(cen));
		cen = centroid(H2);
		v += 2 * PI * area(H2) * std::abs(l.dist(cen));
		cen = centroid(H3);
		v -= 2 * PI * area(H3) * std::abs(l.dist(cen));
	}
	else {
		Polygon& H3 = f1 ? H1 : H2;
		Pos cen = centroid(H3);
		v = 2 * PI * area(H3) * std::abs(l.dist(cen));
	}
	std::cout << v << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(1);
	//freopen("pol.in", "r", stdin);
	//freopen("pol_my.out", "w", stdout);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }//boj22785 Polygon Revolution

/*

1
5
10 15
14 13
12 9
8 9
6 13
0 1 -10

1
10
10000.0 5000.0
9045.1 2061.1
6545.1 244.7
3454.9 244.7
954.9 2061.1
0.0 5000.0
954.9 7938.9
3454.9 9755.3
6545.1 9755.3
9045.1 7938.9
2.5 9.7 -60822.0

503210627676.1

488259039917.1

1
10
10000.0000000000 5000.0000000000
9045.0849718747 2061.0737385376
6545.0849718747 244.7174185242
3454.9150281253 244.7174185242
954.9150281253 2061.0737385376
0.0000000000 5000.0000000000
954.9150281253 7938.9262614624
3454.9150281253 9755.2825814758
6545.0849718747 9755.2825814758
9045.0849718747 7938.9262614624
2.5477706749 9.7139154346 -60821.9840097030

*/


