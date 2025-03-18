#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <random>
#include <cassert>
#include <array>
#include <tuple>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
const ld INF = 1e17;
const ld TOL = 1e-15;
const int LEN = 1e6;
const ld PI = acos(-1);

int N, M, T, Q;
inline bool zero(const ld& x) { return std::abs(x) < TOL; }
inline int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
struct Pos3D {
	ll x, y, z;
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
	inline ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
	inline Pos3D operator / (const Pos3D& p) const {
		Pos3D ret;
		ret.x = y * p.z - z * p.y;
		ret.y = z * p.x - x * p.z;
		ret.z = x * p.y - y * p.x;
		return ret;
	}
	inline Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
	inline Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	inline Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	inline Pos3D operator / (const ll& scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
	Pos3D& operator -= (const Pos3D& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
	Pos3D& operator *= (const ll& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
	Pos3D& operator /= (const ll& scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
	inline ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrt(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) { is >> p.x >> p.y >> p.z; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) { os << p.x << " " << p.y << " " << p.z << "\n"; return os; }
};
typedef std::vector<Pos3D> Polyhedron;
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
std::vector<Pos3D> C3D;//3D
inline Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
inline ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
inline bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) { return !((b - a) / (c - b)).Euc(); }
inline bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return !(cross(a, b, c) * (p - a)); }
inline bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) { return cross(a, b, c) * (p - a) > 0; }
inline int prep(std::vector<Pos3D>& p) {//refer to Koosaga'
	shuffle(p.begin(), p.end(), std::mt19937(0x14004));
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
	ll above(const Polyhedron& C, const Pos3D& p) const {
		return cross(C[v[0]], C[v[1]], C[v[2]]) * (p - C[v[0]]);
	}
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
void query() {
	int x1, y1, x2, y2, h;
	std::cin >> x1 >> y1 >> x2 >> y2;
	std::cin >> N;
	ld G = 1. * (x2 - x1) * (y2 - y1);
	if (!N) {
		std::cout << G << "\n";
		return;
	}
	C3D.resize(N * 4 + 4);
	C3D[N * 4 + 0] = Pos3D(x1, y1, 0);
	C3D[N * 4 + 1] = Pos3D(x1, y2, 0);
	C3D[N * 4 + 2] = Pos3D(x2, y2, 0);
	C3D[N * 4 + 3] = Pos3D(x2, y1, 0);
	for (int i = 0; i < N; i++) {
		std::cin >> x1 >> y1 >> x2 >> y2 >> h;
		C3D[i * 4 + 0] = Pos3D(x1, y1, h);
		C3D[i * 4 + 1] = Pos3D(x1, y2, h);
		C3D[i * 4 + 2] = Pos3D(x2, y2, h);
		C3D[i * 4 + 3] = Pos3D(x2, y1, h);
	}
	Hull3D = convex_hull_3D(C3D);
	if (col || cop) { std::cout << "err\n"; return; }
	ld suf = 0;
	for (const Face& F : Hull3D) suf += F.norm(C3D).mag() * .5;
	std::cout << suf - G << "\n";
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(4);
	std::cin >> Q;
	while (Q--) query();
	return;
}
int main() { solve(); return 0; }//boj5418 Giant Cover