#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
const ld INF = 1e17;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e3;
int N, M, T, Q;
bool col, cop;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
ld norm(ld& th) {
	while (th < -TOL) th += PI * 2;
	while (th > PI * 2) th -= PI * 2;
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

struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
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
	ld operator / (const Line& l) const { return s / l.s; }
	ld operator * (const Line& l) const { return s * l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	friend std::ostream& operator << (std::ostream& os, const Line& l) {
		os << l.s.vy << " " << l.s.vx << " " << l.c;
		return os;
	}
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { {c.x + C.c.x, c.y + C.c.y}, r + C.r }; }
	Circle operator - (const Circle& C) const { return { {c.x - C.c.x, c.y - C.c.y}, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
};
Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Pos B = v - u, C = w - u;
	Line B_ = Line({ B.x, B.y }, B.Euc() / 2);
	Line C_ = Line({ C.x, C.y }, C.Euc() / 2);
	if (zero(B_ / C_)) return { { 0, 0 }, -1 };
	Pos inx = intersection(B_, C_);
	return Circle(inx + u, inx.mag());
}
//Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
//	Line l1 = rotate90(L(u, v), (u + v) * .5);
//	Line l2 = rotate90(L(v, w), (v + w) * .5);
//	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
//	Pos c = intersection(l1, l2);
//	ld r = (c - u).mag();
//	return Circle(c, r);
//}
bool valid_check(const Circle& c, const std::vector<Pos>& P) {
	for (const Pos& p : P) if (c < p) return 0;
	return 1;
}
Circle get_min_circle(std::vector<Pos>& P) {
	int sz = P.size();
	assert(sz <= 3);
	if (!sz) return Circle(Pos(0, 0), 0);
	if (sz == 1) return Circle(P[0], 0);
	if (sz == 2) return enclose_circle(P[0], P[1]);
	for (int i = 0; i < 2; i++) {
		for (int j = i + 1; j < 3; j++) {
			Circle ec = enclose_circle(P[i], P[j]);
			if (valid_check(ec, P)) return ec;
		}
	}
	return enclose_circle(P[0], P[1], P[2]);
}
Circle welzl(std::vector<Pos>& P, std::vector<Pos> R, int sz) {
	if (!sz || R.size() == 3) return get_min_circle(R);
	int idx = rand() % sz;
	Pos p = P[idx];
	std::swap(P[idx], P[sz - 1]);
	Circle mec = welzl(P, R, sz - 1);
	if (mec >= p) return mec;
	R.push_back(p);
	return welzl(P, R, sz - 1);
}
Circle welzl(std::vector<Pos>& P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	return welzl(P, {}, P.size());
}
//=======================================//
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
	//Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	//Pos3D& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }
}; const Pos3D INF3D = { INF, INF, INF };
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Planar {
	Pos3D norm, p0;
	Planar(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
	friend std::istream& operator >> (std::istream& is, Planar& P) { is >> P.norm >> P.p0 ; return is; }
	friend std::ostream& operator << (std::ostream& os, const Planar& P) { os << P.norm << " " << P.p0; return os; }
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D torque = cross(d1, d2, d3);
	ld ret = torque * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Planar P(const Pos3D& p1, const Pos3D& p2, const Pos3D& p3) {
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Planar(norm, p1);
}
Planar P(std::vector<Pos3D>& tri) {
	Pos3D p1 = tri[0], p2 = tri[1], p3 = tri[2];
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Planar(norm, p1);
}
Pos3D intersection(const Planar& S, const Line3D& l) {
	ld det = S.norm * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = (S.norm * S.p0 - S.norm * l.p0) / det;
	//ld t = (S.norm * (S.p0 - l.p0)) / det;
	return l.p0 + (l.dir * t);
}
ld above(const Planar& P, const Pos3D& t) { return ((t - P.p0) * P.norm) / P.norm.mag(); }
//int above(const Plane& S, const Pos3D& p) { ld ret = p * S.norm() + S.d; return dcmp(ret); }
ld sc[4];
void get_angle(ld sc[], const Pos3D& norm) {
	ld a = norm.x, b = norm.y, c = norm.z;
	ld angle1 = -atan2(b, a);
	ld dx = sqrtl(a * a + b * b);
	ld angle2 = -atan2(dx, c);
	sc[0] = sin(angle1);
	sc[1] = cos(angle1);
	sc[2] = sin(angle2);
	sc[3] = cos(angle2);
	return;
}
Pos3D rotate(ld sc[], const Pos3D& p) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	return Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
}
void rotate(ld sc[], const Pos3D& p, std::vector<Pos>& C) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	Pos3D q = Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
	C.push_back(Pos(q.x, q.y));
	return;
}
Pos projecting2D(ld sc[], const Pos3D& p) {//project to xy_plane
	ld x = p.x * sc[1] - p.y * sc[0], y = p.x * sc[0] + p.y * sc[1], z = p.z;
	Pos3D q = Pos3D(z * sc[2] + x * sc[3], y, z * sc[3] - x * sc[2]);
	return Pos(q.x, q.y);
}
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && ret > 0;
}
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) { return zero(((b - a) / (c - b)).Euc()); }
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return zero(cross(a, b, c) * (p - a)); }
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return cross(a, b, c) * (p - a) > 0; }
int prep(std::vector<Pos3D>& p) {//refer to Koosaga'
	shuffle(p.begin(), p.end(), std::mt19937(0x14004));
	int dim = 1;
	for (int i = 1; i < p.size(); i++) {
		if (dim == 1) {
			if (p[0] != p[i]) std::swap(p[1], p[i]), ++dim;
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
	assert(dim == 4);
	return dim;
}
using Face = std::array<int, 3>;
Planar P(std::vector<Pos3D> C, const Face& f) { return P(C[f[0]], C[f[1]], C[f[2]]); }
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {//incremental construction
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	int suf = prep(candi);
	//if (suf <= 2) { col = 1; return {}; };
	//if (suf == 3) { cop = 1; return {}; };
	int sz = candi.size();
	std::vector<Face> faces;
	std::vector<int> active;//whether face is active - face faces outside 
	std::vector<std::vector<int>> vis(sz);//faces visible from each point
	std::vector<std::vector<int>> rvis;//points visible from each face
	std::vector<std::array<Edge, 3>> other;//other face adjacent to each edge of face
	auto ad = [&](const int& a, const int& b, const int& c) -> void {//add face
		faces.push_back({ a, b, c });
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
		return above(candi[tri[0]], candi[tri[1]], candi[tri[2]], candi[b]);
		};
	auto edge = [&](const Edge& e) -> pi {
		return { faces[e.face_num][e.edge_num], faces[e.face_num][(e.edge_num + 1) % 3] };
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
	for (int i = 3; i < N; i++) visible(abv(1, i), i);//coplanar points go in rvis[0]

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
			glue({ X, 1 }, { label[y = faces[X][1]], 2 });
			if (y == st) break;
		}
	}
	std::vector<Face> hull3D;
	for (int i = 0; i < faces.size(); i++) if (active[i]) hull3D.push_back(faces[i]);
	return hull3D;
}
std::vector<Pos> C2D;//2D container
std::vector<Pos3D> C3D;//3D container
std::vector<Face> H3D;//3D convex hull
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	C3D.resize(N);
	for (int i = 0; i < N; i++) std::cin >> C3D[i];
	H3D = convex_hull_3D(C3D);

	ld MAX = -INF;
	for (const Face& F : H3D) {
		C2D.clear();
		ld h = 0;
		Planar S = P(C3D, F);
		get_angle(sc, S.norm);
		for (const Pos3D& p : C3D) {
			h = std::max(h, std::abs(above(S, p)));
			Line3D l = Line3D(S.norm, p);
			Pos3D inx = intersection(S, l);
			if (inx == INF3D) continue;
			C2D.push_back(projecting2D(sc, inx));
		}
		Circle mec = welzl(C2D);
		MAX = std::max(MAX, h * mec.A());
	}

	std::cout << MAX << "\n";
	return;
}
int main() { solve(); return 0; }//boj14507 Stars in a Can
