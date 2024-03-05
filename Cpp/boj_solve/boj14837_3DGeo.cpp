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
const ll INF = 1e17;
const ld TOL = 1e-7;
const int LEN = 1e3;
int N, M, T, Q;
bool COL, COP;
// 3D Convex Hull in O(n log n)
// Very well tested. Good as long as not all points are coplanar
// In case of collinear faces, returns arbitrary triangulation
// Credit: Benq
// refer to Koosaga

bool zero(const ld& x) { return std::abs(x) < TOL; }
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
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator *= (const ll& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
		is >> p.x >> p.y >> p.z;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
		os << p.x << " " << p.y << " " << p.z << "\n";
		return os;
	}
} willy, MAXP{ INF, INF, INF };
const Pos3D O3D = { 0, 0, 0 };
std::vector<Pos3D> C, H;
using Face = std::array<int, 3>;
std::vector<Face> Hull3D;
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
std::vector<Pos3D> tri1, tri2;
struct Line3D {
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : dir(DIR), p0(P0) {}
};
struct Plane {
	Pos3D norm, p0;
	Plane(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)) : norm(NORM), p0(P0) {}
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
	Pos3D CCW = cross(d1, d2, d3);
	ld ret = CCW * norm;
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
Pos3D norm(const std::vector<Pos3D>& H, const Face& F) {
	return cross(H[F[0]], H[F[1]], H[F[2]]);
}
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret >= 0;
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret > 0;
}
int inner_check(const std::vector<Pos3D>& H, const Pos3D& norm, const Pos3D& p) {
	int sz = H.size();
	bool col = 0;
	for (int i = 0; i < sz; i++) {
		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		if (ccw(cur, nxt, p, norm) < 0) return -1;
		if (!ccw(cur, nxt, p, norm)) col = 1;
	}
	if (col) return 0;
	return 1;
}
std::vector<Pos3D> graham_scan(std::vector<Pos3D>& C, const Pos3D& norm) {
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		int ret = ccw(C[0], p, q, norm);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
bool graham_scan(std::vector<Pos3D>& C, const Pos3D& norm, const Pos3D& p) {
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return on_seg_strong(C[0], C[C.size() - 1], p);
	}
	std::vector<Pos3D> H;
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos3D& p, const Pos3D& q) -> bool {
		int ret = ccw(C[0], p, q, norm);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i], norm) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return inner_check(H, norm, p) > -1;
}
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return ((b - a) / (c - b)).Euc() == 0;
}
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a) == 0;
}
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
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
	//assert(dim == 4);
	return dim;
}
int above(const std::vector<Pos3D>& C, const Face& F, const Pos3D& p) {
	Pos3D nrm = cross(C[F[0]], C[F[1]], C[F[2]]);
	//for (int i = 0; i < 3; i++) {
	//	//if (on_seg_strong(C[F[i]], C[F[(i + 1) % 3]], p)) return 2;
	//	if (on_seg_strong(C[F[i]], C[F[(i + 1) % 3]], p)) return 0;
	//}
	ll ret = nrm * (p - C[F[0]]);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int inner_check(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
	bool cop = 0;
	for (const Face& face : F) {
		int f = above(C, face, p);
		if (f == 0) cop = 1;
		if (f > 0) return 0;
	};
	if (cop) return 0;
	return 1;
}
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	COL = 0; COP = 0;
	int suf = prep(candi);
	if (suf <= 2) { COL = 1; return {}; };
	if (suf == 3) { COP = 1; return {}; };
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
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//std::cout << std::fixed;
	//std::cout.precision(4);
	std::cin >> T;
	for (int tc = 1; tc <= T; tc++) {
		std::cout << "Case #" << tc << ": ";
		std::cin >> N;
		if (!N) return;
		C.resize(N);
		for (int i = 0; i < N; i++) std::cin >> C[i];
		Hull3D = convex_hull_3D(C);
		//for (const Face& face : Hull3D) {
		//	for (int i = 0; i < 3; i++) std::cout << candi[face[i]];
		//	std::cout << "\n";
		//}
		if (COL) {
			std::sort(C.begin(), C.end());
			int f = on_seg_strong(C[0], C[N - 1], O3D);
			std::cout << (f < 1 ? "NO\n" : "YES\n");
		}
		else if (COP) {
			int i = 2;
			while (!cross(C[0], C[1], C[i]).Euc()) i++;
			Pos3D nrm = cross(C[0], C[1], C[i]);
			//H = graham_scan(C, nrm);
			//int f = inner_check(H, nrm, O3D);
			//std::cout << (f < 0 ? "NO\n" : "YES\n");
			bool f = graham_scan(C, nrm, O3D);
			std::cout << (f < 1 ? "NO\n" : "YES\n");
		}
		else {
			int f = inner_check(C, Hull3D, O3D);
			std::cout << (f < 0 ? "NO\n" : "YES\n");
		}
	}
	return;
}
int main() { solve(); return 0; }//boj14837 Omnicircumnavigation (Large)