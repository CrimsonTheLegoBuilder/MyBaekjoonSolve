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
typedef long long ll;
//typedef __int128 int128;
typedef long long int128;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<size_t> Vidx;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e17;
const ld TOL = 1e-10;
const int LEN = 1e5 + 5;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline int128 sq(const int& x) { return (int128)x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
#define LINE 1
#define CIRCLE 2

int N, M, T, Q;
struct Pii {
	ll x, y;
	Pii(ll x_ = 0, ll y_ = 0) : x(x_), y(y_) {}
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const ll& n) const { return { x * n, y * n }; }
	Pii operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pii& p) const { return { x * p.y - y * p.x }; }
	ll Euc() const { return x * x + y * y; }
	ld mag() const { return sqrtl(Euc()); }
};
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
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
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return dot(d1, d2, d3) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
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
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
bool collinear(const Seg& p, const Seg& q) { return collinear(p.s, p.e, q.s, q.e); }
struct Circle {
	Pos c;
	int r;
	Circle(Pos c_ = Pos(), int r_ = 0) : c(c_), r(r_) {}
	bool operator == (const Circle& q) const { return c == q.c && r == q.r; }
	bool operator < (const Circle& q) const { return c == q.c ? r < q.r : c < q.c; }
	bool operator < (const Pos& p) const { return sign(r - (c - p).mag()) < 0; }
	bool outside(const Circle& q) const { return sign((c - q.c).Euc() - sq((ll)r + q.r)) >= 0; }
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
	inline friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
};
typedef std::vector<Circle> Circles;
Vld circle_line_intersections(const Circle& q, const Seg& l, const int& t = LINE) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos s = l.s, e = l.e;
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - (ll)q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	Vld ret;
	if (t == LINE) { ret.push_back(lo); ret.push_back(hi); }
	//if (t == LINE) {
	//	if (0 < lo && lo < 1) ret.push_back(lo);
	//	ret.push_back(lo);
	//	if (zero(det)) return ret;
	//	if (0 < hi && hi < 1) ret.push_back(hi);
	//	ret.push_back(hi);
	//}
	else {//circle
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
		if (zero(det)) return ret;
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
	}
	return ret;
}
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
typedef std::vector<Linear> Vlinear;
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
struct Pos3D {
	int128 x, y, z;
	int r;
	Pos3D(int128 X = 0, int128 Y = 0, int128 Z = 0, int R = 0) : x(X), y(Y), z(Z), r(R) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	int128 operator * (const Pos3D& p) const {
		ld det = (ld)x * p.x + (ld)y * p.y + (ld)z * p.z;
		if (fabs(det) > 1e17) return sign(det);
		return x * p.x + y * p.y + z * p.z;
	}
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const int128& n) const { return { x * n, y * n, z * n }; }
	Pos3D operator / (const int128& n) const { return { x / n, y / n, z / n }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const int128& n) { x *= n; y *= n; z *= n; return *this; }
	Pos3D& operator /= (const int128& n) { x /= n; y /= n; z /= n; return *this; }
	int128 Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrt(Euc()); }
	Pii pii() const {
		assert(x < 1e7);
		assert(y < 1e7);
		return { x, y };
	}
	Pos p() const {
		assert(x < 1e7);
		assert(y < 1e7);
		return { (ld)x, (ld)y };
	}
	Circle c() const { return Circle(Pos(x, y), r); }
	bool operator >= (const Pos3D& d) const {
		bool f1 = r >= d.r;
		bool f2 = sq((ll)x - d.x) + sq((ll)y - d.y) <= sq((ll)r - d.r);
		return f1 && f2;
	}
	//friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	//friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
};
typedef std::vector<Pos3D> Polyhedron;
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
std::vector<Pos3D> C3D;//3D
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
int128 dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) { return !((b - a) / (c - b)).Euc(); }
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return !(cross(a, b, c) * (p - a)); }
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return cross(a, b, c) * (p - a) > 0; }
int prep(std::vector<Pos3D>& p) {//refer to Koosaga'
	//shuffle(p.begin(), p.end(), std::mt19937(0x14004));
	int dim = 1;
	for (int i = 1; i < p.size(); i++) {
		if (dim == 1) {
			if (p[0] != p[i])
				std::swap(p[1], p[i]), ++dim;
		}
		else if (dim == 2) {
			if (!collinear(p[0], p[1], p[i]))
				std::swap(p[2], p[i]), ++dim;
		}
		else if (dim == 3) {
			if (!coplanar(p[0], p[1], p[2], p[i]))
				std::swap(p[3], p[i]), ++dim;
		}
	}
	//assert(dim == 4);
	return dim;
}
struct Face {
	int v[3];
	Face(int a = 0, int b = 0, int c = 0) { v[0] = a; v[1] = b; v[2] = c; }
	inline Pos3D norm(std::vector<Pos3D>& C) const { return cross(C[v[0]], C[v[1]], C[v[2]]); }
};
std::vector<Face> Hull3D;
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
bool col = 0, cop = 0;
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {//incremental construction
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	col = 0, cop = 0;
	int suf = prep(candi);
	if (suf <= 2) { col = 1; return {}; };
	if (suf == 3) { cop = 1; return {}; };
	int sz = candi.size();
	std::vector<Face> faces;
	std::vector<int> active;//whether face is active - face faces outside 
	std::vector<std::vector<int>> vis(sz);//faces visible from each point
	std::vector<std::vector<int>> rvis;//points visible from each face
	std::vector<std::array<Edge, 3>> other;//other face adjacent to each edge of face
	auto ad = [&](const int& a, const int& b, const int& c) -> void {//add face
		faces.push_back(Face(a, b, c));
		active.push_back(1);
		rvis.emplace_back();
		other.emplace_back();
		return;
		};
	auto visible = [&](const int& a, const int& b) -> void {
		vis[b].push_back(a);
		rvis[a].push_back(b);
		return;
		};
	auto abv = [&](const int& a, const int& b) -> bool {//above
		Face tri = faces[a];
		return above(candi[tri.v[0]], candi[tri.v[1]], candi[tri.v[2]], candi[b]);
		};
	auto edge = [&](const Edge& e) -> pi {
		return { faces[e.face_num].v[e.edge_num], faces[e.face_num].v[(e.edge_num + 1) % 3] };
		};
	auto glue = [&](const Edge& a, const Edge& b) -> void {//link two faces by an edge
		pi x = edge(a); assert(edge(b) == pi(x.second, x.first));
		other[a.face_num][a.edge_num] = b;
		other[b.face_num][b.edge_num] = a;
		return;
		};//ensure face 0 is removed when i = 3
	ad(0, 1, 2), ad(0, 2, 1);
	if (abv(1, 3)) std::swap(candi[1], candi[2]);
	for (int i = 0; i < 3; i++) glue({ 0, i }, { 1, 2 - i });
	for (int i = 3; i < sz; i++) visible(abv(1, i), i);//coplanar points go in rvis[0]
	std::vector<int> label(sz, -1);
	for (int i = 3; i < sz; i++) {//incremental construction
		std::vector<int> rem;
		for (auto& v : vis[i]) if (active[v]) { active[v] = 0, rem.push_back(v); }
		if (!rem.size()) continue;//hull unchanged
		int st = -1;//start idx
		for (const int& v : rem) {
			for (int j = 0; j < 3; j++) {
				int o = other[v][j].face_num;
				if (active[o]) {//create new face!
					int idx1, idx2;
					std::tie(idx1, idx2) = edge({ v, j });
					ad(idx1, idx2, i);
					st = idx1;
					int cur = rvis.size() - 1;
					label[idx1] = cur;
					std::vector<int> tmp;
					set_union(rvis[v].begin(), rvis[v].end(), rvis[o].begin(), rvis[o].end(), back_inserter(tmp));
					//merge sorted vectors ignoring duplicates
					for (auto& x : tmp) if (abv(cur, x)) visible(cur, x);
					//if no rounding errors then guaranteed that only x > i matters
					glue({ cur, 0 }, other[v][j]);//glue old, new face
				}
			}
		}
		for (int x = st, y; ; x = y) {//glue new faces together
			int X = label[x];
			glue({ X, 1 }, { label[y = faces[X].v[1]], 2 });
			if (y == st) break;
		}
	}
	std::vector<Face> hull3D;
	for (int i = 0; i < faces.size(); i++) if (active[i]) hull3D.push_back(faces[i]);
	return hull3D;
}
ll ccw(const Face& f) {
	Pii d1 = C3D[f.v[0]].pii();
	Pii d2 = C3D[f.v[1]].pii();
	Pii d3 = C3D[f.v[2]].pii();
	return sign(cross(d1, d2, d3));
}
struct Disk {
	int x, y, r;
	Disk(int x_ = 0, int y_ = 0, int r_ = 0) : x(x_), y(y_), r(r_) {}
	bool operator < (const Disk& d) const { return x == d.x ? y == d.y ? r < d.r : y < d.y : x < d.x; }
	bool operator == (const Disk& d) const { return x == d.x && y == d.y && r == d.r; }
	bool operator >= (const Disk& d) const {
		bool f1 = r >= d.r;
		bool f2 = sq((ll)x - d.x) + sq((ll)y - d.y) <= sq((ll)r - d.r);
		return f1 && f2;
	}
	Pii pii() const { return Pii(x, y); }
	Circle c() const { return Circle(Pos(x, y), r); }
	Pos3D p3d() const { return Pos3D(x, y, sq(x) + sq(y) - sq(r), r); }
	friend std::istream& operator >> (std::istream& is, Disk& p) { is >> p.x >> p.y >> p.r; return is; }
};
ld circle_cut(const Circle& c, const Seg& s) {
	Pos v1 = s.s - c.c, v2 = s.e - c.c;
	ll r = c.r;
	Vld inx = circle_line_intersections(c, s, LINE);
	if (inx.empty()) return r * r * rad(v1, v2) * .5;
	Pos m1, m2;
	if (inx.size() == 2) m1 = s.p(inx[0]), m2 = s.p(inx[1]);
	else m1 = m2 = c.p(inx[0]);
	m1 -= c.c; m2 -= c.c;
	bool d1 = dot(m1, v1, m2) > -TOL, d2 = dot(m1, v2, m2) > -TOL;
	if (d1 && d2) return (v1 / v2) * .5;
	else if (d1) return (v1 / m2 + r * r * rad(m2, v2)) * .5;
	else if (d2) return (r * r * rad(v1, m1) + m1 / v2) * .5;
	else if (dot(v1, m1, v2) > 0 && dot(v1, m2, v2) > 0)
		return (r * r * (rad(v1, m1) + rad(m2, v2)) + m1 / m2) * .5;
	else return (r * r * rad(v1, v2)) * .5;
}
ld green(const Circle& c, const Polygon& h) {
	int sz = h.size();
	ld a = 0;
	for (int i = 0; i < sz; i++) {
		int j = (i + 1) % sz;
		const Pos& p1 = h[i], & p2 = h[j];
		a += circle_cut(c, Seg(p1, p2));
	}
	return a;
}
typedef std::vector<Disk> Disks;
Circle seed[LEN];
Vint I, ID[LEN];//power diagram (Laguerre-Voronoi diagram)
ld A;
void solve() {
	A = 0;
	int bnd = 2e4 + 10;
	//int bnd = 2e6 + 1;
	//std::cin.tie(0)->sync_with_stdio(0);
	//std::cout.tie(0);
	//std::cout << std::fixed;
	//std::cout.precision(15);
	std::cin >> N;
	Disks D(N), V;
	for (Disk& d : D) std::cin >> d;
	std::sort(D.rbegin(), D.rend());
	D.erase(unique(D.begin(), D.end()), D.end());
	N = D.size();
	Vbool F(N, 1);
	for (int i = 0, j; i < N; i = j) {
		j = i + 1;
		while (j < N && (D[i].x == D[j].x && D[i].y == D[j].y)) {
			F[j] = 0;
			j++;
		}
	}
	for (int i = 0; i < N; i++) if (F[i]) V.push_back(D[i]);
	std::swap(D, V);
	D.push_back(Disk(bnd, bnd));
	D.push_back(Disk(-bnd, bnd));
	D.push_back(Disk(-bnd, -bnd));
	D.push_back(Disk(bnd, -bnd));
	N = D.size();
	shuffle(D.begin(), D.end(), std::mt19937(0x14004));
	C3D.resize(N);
	for (int i = 0; i < N; i++) C3D[i] = D[i].p3d();
	Hull3D = convex_hull_3D(C3D);
	for (const Face& f : Hull3D) {
		if (ccw(f) < 0) {
			ID[f.v[0]].push_back(f.v[1]);
			ID[f.v[0]].push_back(f.v[2]);
			ID[f.v[1]].push_back(f.v[0]);
			ID[f.v[1]].push_back(f.v[2]);
			ID[f.v[2]].push_back(f.v[0]);
			ID[f.v[2]].push_back(f.v[1]);
			I.push_back(f.v[0]);
			I.push_back(f.v[1]);
			I.push_back(f.v[2]);
		}
	}
	std::sort(I.begin(), I.end());
	I.erase(unique(I.begin(), I.end()), I.end());
	for (const int s : I) {
		const Pii ca = C3D[s].pii();
		const ll ra = C3D[s].r;
		const Circle a = Circle(Pos(ca.x, ca.y), ra);
		if (!ra) continue;
		std::sort(ID[s].begin(), ID[s].end());
		ID[s].erase(unique(ID[s].begin(), ID[s].end()), ID[s].end());
		Vlinear HP;
		HP.push_back(Linear(Pos(bnd, bnd), Pos(-bnd, bnd)));
		HP.push_back(Linear(Pos(-bnd, bnd), Pos(-bnd, -bnd)));
		HP.push_back(Linear(Pos(-bnd, -bnd), Pos(bnd, -bnd)));
		HP.push_back(Linear(Pos(bnd, -bnd), Pos(bnd, bnd)));
		bool f = 1;
		for (const int& i : ID[s]) {
			if (C3D[i] >= C3D[s]) { f = 0; break; }
			if (C3D[s] >= C3D[i]) continue;
			const Pii cb = C3D[i].pii();
			const ll rb = C3D[i].r;
			if (!rb) continue;
			Pii v = cb - ca;//vec a -> b
			ld distance = v.mag();
			ld X = (ra * ra - rb * rb + v.Euc()) / (2 * distance);
			Pos m = Pos(ca.x, ca.y) + Pos(v.x, v.y) * X / distance;
			HP.push_back(Linear(m, m + ~Pos(v.x, v.y)));
		}
		if (!f) continue;
		Polygon HPI = half_plane_intersection(HP);
		if (!HPI.size()) continue;
		ld cut = green(a, HPI);
		A += cut;
	}
	std::cout << A << "\n";
	return;
}
//int main() { solve(); return 0; }//boj29432
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	std::cin >> Q;
	while (Q--) {
		I.clear();
		for (int i = 0; i < 3001; i++) ID[i].clear();
		solve();
	}
	return 0;
}