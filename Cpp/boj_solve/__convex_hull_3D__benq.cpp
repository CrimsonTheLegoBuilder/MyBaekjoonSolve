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
	while (sign(th) < 0) th += 2 * PI;
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

//geometry-struct
struct Pos3D {
	ll x, y, z;
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D operator / (const ll& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ll& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3D& operator /= (const ll& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrt(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z; return os; }
} candi[LEN];
struct Pos3Df {
	ld x, y, z;
	Pos3Df(ld X = 0, ld Y = 0, ld Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3Df& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3Df& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3Df& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	ld operator * (const Pos3Df& p) const { return x * p.x + y * p.y + z * p.z; }
	Pos3Df operator / (const Pos3Df& p) const {
		Pos3Df ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	Pos3Df operator + (const Pos3Df& p) const { return { x + p.x, y + p.y, z + p.z }; }
	Pos3Df operator - (const Pos3Df& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3Df operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3Df operator / (const ld& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3Df& operator += (const Pos3Df& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3Df& operator -= (const Pos3Df& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3Df& operator *= (const ld& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3Df& operator /= (const ld& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
	Pos3Df unit() const { return *this / mag(); }
	Pos3Df norm(const Pos3Df& p) const { return (*this / p).unit(); }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3Df& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3Df& p) { os << p.x << " " << p.y << " " << p.z; return os; }
};
typedef std::vector<Pos3D> Polyhedron;
typedef std::vector<Pos3Df> Polyhedronf;
const Pos3D O3D = { 0, 0, 0 };
const Pos3Df O3Df = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
//const Pos3D MAXP3D = { INF, INF, INF };
std::vector<Pos3D> C3D;//3D
std::vector<Pos3Df> C3Df;//3D double
//fn
inline Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
inline Pos3Df cross(const Pos3Df& d1, const Pos3Df& d2, const Pos3Df& d3) { return (d2 - d1) / (d3 - d2); }
inline ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
inline bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return !((b - a) / (c - b)).Euc();
}
inline bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return !(cross(a, b, c) * (p - a));
}
inline bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
inline int prep(std::vector<Pos3D>& p) {//refer to Koosaga'
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
	//assert(dim == 4);
	return dim;
}
struct Planar {
	Pos3Df norm, p0;
	//Planar(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
	Planar(Pos3Df a = Pos3Df(0, 0, 0), Pos3Df b = Pos3Df(0, 0, 0), Pos3Df c = Pos3Df(0, 0, 0)) {
		norm = cross(a, b, c);
		p0 = a;
	}
	inline bool coplanar(const Pos3Df p) const { return zero(norm * (p - p0)); }
	friend std::istream& operator >> (std::istream& is, Planar& P) { is >> P.norm >> P.p0; return is; }
	friend std::ostream& operator << (std::ostream& os, const Planar& P) { os << P.norm << " " << P.p0; return os; }
};
struct Face {
	int v[3];
	Face(int a = 0, int b = 0, int c = 0) { v[0] = a; v[1] = b; v[2] = c; }
	inline Pos3D norm(std::vector<Pos3D>& C) const { return cross(C[v[0]], C[v[1]], C[v[2]]); }
	Planar P(const Polyhedronf& C) const { return Planar(C[v[0]], C[v[1]], C[v[2]]); }
	inline ld sph_tri_area(const Polyhedronf& C) const {
		ld ret = -PI;
		Planar s1 = Planar(C[v[0]], C[v[1]], O3Df);
		Planar s2 = Planar(C[v[1]], C[v[2]], O3Df);
		Planar s3 = Planar(C[v[2]], C[v[0]], O3Df);
		ret += PI - atan2l((s1.norm / s2.norm).mag(), s1.norm * s2.norm);
		ret += PI - atan2l((s2.norm / s3.norm).mag(), s2.norm * s3.norm);
		ret += PI - atan2l((s3.norm / s1.norm).mag(), s3.norm * s1.norm);
		return ret;
	}
	ll above(const Polyhedron& C, const Pos3D& p) const {
		return cross(C[v[0]], C[v[1]], C[v[2]]) * (p - C[v[0]]);
	}
};
bool strictly_inner_check(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
	for (const Face& f : F) {
		if (f.above(C, p) >= 0) return 0;
	}
	return 1;
}
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
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(17);
	std::cin >> N;
	C3D.resize(N);
	C3Df.resize(N + 1);
	for (int i = 0; i < N; i++) {
		std::cin >> C3D[i];
	}
	C3D.push_back(Pos3D(0, 0, 0));
	Hull3D = convex_hull_3D(C3D);
	//for (Pos3D& p : C3D) std::cout << p << "\n";
	if (col || cop) { std::cout << "1.0000000\n"; return; }
	Pos3Df p;
	for (int i = 0; i < N + 1; i++) {
		p = Pos3Df(C3D[i].x, C3D[i].y, C3D[i].z);
		ld x = p.mag();
		if (p.mag() > x) p *= 1000, p /= x;
		C3Df[i] = p;
	}
	ld suf = 0;
	for (const Face& F : Hull3D) {
		if (!F.P(C3Df).coplanar(O3Df)) {
			ld a = F.sph_tri_area(C3Df);
			suf += a;
		}
	}
	if (strictly_inner_check(C3D, Hull3D, O3D)) std::cout << "0.0000000\n";
	else std::cout << (1 - suf / (4 * PI)) << "\n";
	return;
}
int main() { solve(); return 0; }//boj23459 - convex_hull_3D refer to Benq
