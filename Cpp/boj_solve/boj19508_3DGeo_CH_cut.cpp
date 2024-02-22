#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
#include <cassert>
#include <random>
#include <array>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::pair<int, int> pi;
const ld INF = 1e17;
const ld TOL = 1e-7;
const int LEN = 1e3;
int N, M, T, Q;

// 3D Convex Hull in O(n log n)
// Very well tested. Good as long as not all points are coplanar
// In case of collinear faces, returns arbitrary triangulation
// Credit: Benq
// refer to Koosaga'

bool zero(const ld& x) { return std::abs(x) < TOL; }
struct Pos3D {
private:
	ld x, y, z;
public:
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
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	ld Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p);
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p);
} willy, MAXP{ INF, INF, INF };
std::vector<Pos3D> HPI;//2D
std::vector<Pos3D> poses;//3D
std::istream& operator >> (std::istream& is, Pos3D& p) {
	is >> p.x >> p.y >> p.z;
	return is;
}
std::ostream& operator << (std::ostream& os, const Pos3D& p) {
	os << p.x << " " << p.y << " " << p.z << "\n";
	return os;
}
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Plane {
	Pos3D norm, p0;
	Plane(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
};
struct Plane_formula {
	ld a, b, c, d;
	Plane_formula(ld A = 0, ld B = 0, ld C = 0, ld D = 0) : a(A), b(B), c(C), d(D) {}
	Pos3D norm() const { return Pos3D(a, b, c); };
	friend std::istream& operator >> (std::istream& is, Plane_formula& f);
	friend std::ostream& operator << (std::ostream& os, const Plane_formula& f);
} knife;
std::istream& operator >> (std::istream& is, Plane_formula& f) {
	is >> f.a >> f.b >> f.c >> f.d;
	return is;
}
std::ostream& operator << (std::ostream& os, const Plane_formula& f) {
	os << f.a << " " << f.b << " " << f.c << " " << f.d << "\n";
	return os;
}
using Mesh = std::array<int, 3>;
std::vector<Mesh> Hull3D;
struct Edge {
	int mesh_num, edge_num;
	Edge(int t = 0, int v = 0) : mesh_num(t), edge_num(v) {}
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld area(std::vector<Pos3D>& H, const Pos3D& norm) {
	if (H.size() < 3) return .0;
	Pos3D O = H[0];
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt) * norm / norm.mag();
	}
	return std::abs(ret / 2);
}
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && ret > 0;
}
std::vector<Pos3D> monotone_chain(std::vector<Pos3D>& C, const Pos3D& norm) {
	std::vector<Pos3D> H;
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos3D& p : C) H.push_back(p);
		return H;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && ccw(H[H.size() - 2], H[H.size() - 1], C[i], norm) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	int sz = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > sz && ccw(H[H.size() - 2], H[H.size() - 1], C[i], norm) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	H.pop_back();
	return H;
}
std::vector<Pos3D> graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
	if (C.size() < 3) return {};
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		ld ret = ccw(C[0], p, q, norm);
		if (ret != 0) return ret > 0;
		return (C[0] - p).Euc() < (C[0] - q).Euc();
		});
	//std::cout << "DEBUG candidate::\n";
	//for (const Pos3D& c : C)  std::cout << c;
	//std::cout << "DEBUG candidate::\n";
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0) 
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
//Pos3D intersection(const Plane& S, const Line3D& l) {
//	ld det = S.norm * l.dir;
//	if (zero(det)) return { INF, INF, INF };
//	ld t = (S.norm * S.p0 - S.norm * l.p0) / det;
//	return l.p0 + (l.dir * t);
//}
Pos3D intersection(const Plane_formula& S, const Line3D& l) {
	ld det = S.norm() * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = -((S.norm() * l.p0 + S.d) / det);
	return l.p0 + (l.dir * t);
}
Pos3D intersection(const Plane_formula& S, const Pos3D& p1, const Pos3D& p2) {
	Line3D l = L(p1, p2);
	ld det = S.norm() * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = -((S.norm() * l.p0 + S.d) / det);
	Pos3D inx = l.p0 + (l.dir * t);
	//std::cout << "DEBUG intersection::\n";
	//std::cout << "p1: " << p1 << " p2: " << p2;
	//std::cout << "norm: " << S.norm() << "det: " << det << " t: " << t << "\n";
	//std::cout << "p0: " << l.p0;
	//std::cout << "dir: " << l.dir;
	//std::cout << "inx: " << inx;
	//std::cout << "DEBUG intersection::\n";
	if (on_seg_strong(p1, p2, inx)) return inx;
	else return { INF, INF, INF };
}
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return zero(((b - a) / (c - b)).Euc());
}
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return zero(cross(a, b, c) * (p - a));
}
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
void prep(std::vector<Pos3D>& p) {//refer to Koosaga'
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
	return;
}
//ld dist(const std::vector<Pos3D>& C, const Mesh& F, const Pos3D& p) {
//	Pos3D norm = cross(C[F[0]], C[F[1]], C[F[2]]);
//	ll ret = norm * (p - C[F[0]]);
//	return std::abs(ret / (norm.mag()));
//}
//ld get_min_dist(const std::vector<Pos3D>& C, const std::vector<Mesh>& F, const Pos3D& p) {
//	ld MIN = INF;
//	for (const Mesh& face : F) MIN = std::min(MIN, dist(C, face, p));
//	return MIN;
//}
std::vector<Mesh> convex_hull_3D(std::vector<Pos3D>& candi) {
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	prep(candi);
	int sz = candi.size();
	std::vector<Mesh> faces;
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
		Mesh tri = faces[a];
		return above(candi[tri[0]], candi[tri[1]], candi[tri[2]], candi[b]);
		};
	auto edge = [&](const Edge& e) -> pi {
		return { faces[e.mesh_num][e.edge_num], faces[e.mesh_num][(e.edge_num + 1) % 3] };
		};
	auto glue = [&](const Edge& a, const Edge& b) -> void {//link two faces by an edge
		pi x = edge(a); assert(edge(b) == pi(x.second, x.first));
		other[a.mesh_num][a.edge_num] = b;
		other[b.mesh_num][b.edge_num] = a;
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
				int o = other[v][j].mesh_num;
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
	std::vector<Mesh> hull3D;
	for (int i = 0; i < faces.size(); i++) if (active[i]) hull3D.push_back(faces[i]);
	return hull3D;
}
std::vector<Pos3D> cutting_surface(const std::vector<Pos3D>& pos, const std::vector<Mesh>& F, const Plane_formula& S) {
	std::vector<Pos3D> c, h;//2D
	for (const Mesh& face : F) {
		for (int i = 0; i < 3; i++) {
			Pos3D cur = pos[face[i]], nxt = pos[face[(i + 1) % 3]];
			Pos3D inx = intersection(S, cur, nxt);
			//std::cout << inx;
			if (inx == MAXP) continue;
			c.push_back(inx);
		}
	}
/*	std::cout << "DEBUG surface::\n";
	for (const Pos3D& p : c) std::cout << p;
	std::cout << "DEBUG surface::\n"*/;
	return graham_scan(c, S.norm());
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(4);
	std::cin >> N >> Q;
	poses.resize(N);
	for (int i = 0; i < N; i++) std::cin >> poses[i];
	Hull3D = convex_hull_3D(poses);
	//std::cout << "DEBUG Mesh::\nlen:: " << Hull3D.size() << "\n";
	//for (const Mesh& face : Hull3D) {
	//	for (int i = 0; i < 3; i++) std::cout << poses[face[i]];
	//	std::cout << "DEBUG Mesh::\n";
	//}
	while (Q--) {
		std::cin >> knife;
		HPI = cutting_surface(poses, Hull3D, knife);
		//std::cout << "DEBUG HPI::\n";
		//for (const Pos3D& p : HPI) std::cout << p;
		//std::cout << "DEBUG HPI::\n";

		std::cout << area(HPI, knife.norm());
	}
	return;
}
int main() { solve(); return 0; }//boj19508 Convex Hull

/*
6 1
0 0 1
1 0 1
0 1 1
0 0 -1
1 0 -1
0 1 -1
0 0 1 0

8 0
1 1 1
1 -1 1
-1 1 1
-1 -1 1
1 1 -1
1 -1 -1
-1 1 -1
-1 -1 -1
*/