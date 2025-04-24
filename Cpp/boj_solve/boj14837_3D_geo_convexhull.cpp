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
//const int LEN = 1e5;
int N, T;
// 3D Convex Hull in O(n log n)
// Very well tested. Good as long as not all points are coplanar
// In case of collinear faces, returns arbitrary triangulation
// Credit: Benq
// refer to Koosaga

bool zero(const ld& x) { return std::abs(x) < TOL; }
//2D========================================================================2D//
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ll& n) const { return { x * n, y * n }; }
	Pos operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
	Pos operator ~ () const { return { -y, x }; }
	ll operator ! () const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pos& p) {
		is >> p.x >> p.y;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
		os << p.x << " " << p.y << "\n";
		return os;
	}
};
const Pos O = { 0, 0 };
std::vector<Pos> C2D, H2D;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
int inner_check_bi_search(std::vector<Pos>& H, const Pos& p) {//convex
	int sz = H.size();
	if (!sz) return -1;
	if (sz == 1) return p == H[0] ? 0 : -1;
	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
	int s = 0, e = sz - 1, m;
	while (s + 1 < e) {
		m = s + e >> 1;
		if (cross(H[0], H[m], p) > 0) s = m;
		else e = m;
	}
	if (cross(H[s], H[e], p) > 0) return 1;
	else if (on_seg_strong(H[s], H[e], p)) return 0;
	else return -1;
}
std::vector<Pos> graham_scan(std::vector<Pos>& C) {
	std::vector<Pos> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	//C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
//3D========================================================================3D//
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
	ld mag() const { return sqrtl(Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
		is >> p.x >> p.y >> p.z;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
		os << p.x << " " << p.y << " " << p.z << "\n";
		return os;
	}
};
const Pos3D O3D = { 0, 0, 0 };
const Pos3D X_axis = { 1, 0, 0 };
const Pos3D Y_axis = { 0, 1, 0 };
const Pos3D Z_axis = { 0, 0, 1 };
const Pos3D MAX3P{ INF, INF, INF };
std::vector<Pos3D> C3D, H3D;
using Face = std::array<int, 3>;
std::vector<Face> Hull3D;
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).Euc() && ret >= 0;
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
	assert(p[0] != O3D);
	for (int i = 1; i < p.size(); i++) {
		assert(p[i] != O3D);
		if (dim == 1) {
			if (p[0] != p[i]) std::swap(p[1], p[i]), ++dim;
		}
		else if (dim == 2) {//collinear
			if (!collinear(p[0], p[1], p[i]))
				std::swap(p[2], p[i]), ++dim;
		}
		else if (dim == 3) {//coplanar
			if (!coplanar(p[0], p[1], p[2], p[i]))
				std::swap(p[3], p[i]), ++dim;
		}
	}
	//assert(dim == 4);
	return dim;
}
bool COL, COP;
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	COL = 0; COP = 0;
	int suf = prep(candi);
	if (suf == 2) { COL = 1; return {}; };
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
int above(const std::vector<Pos3D>& C, const Face& F, const Pos3D& p) {
	Pos3D nrm = cross(C[F[0]], C[F[1]], C[F[2]]);
	//if (on_seg_strong(C[F[0]], C[F[1]], p) ||
	//	on_seg_strong(C[F[1]], C[F[2]], p) ||
	//	on_seg_strong(C[F[2]], C[F[0]], p)
	//	) return 2;
	ll ret = nrm * (p - C[F[0]]);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
int inner_check(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
	bool cop = 0;
	for (const Face& face : F) {
		int f = above(C, face, p);
		if (f > 0) return -1;
		//if (f == 2) return 0;
		if (!f) cop = 1;
	};
	if (cop) return 0;
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("../../../input_data/ts2_input.txt", "r", stdin);
	//freopen("../../../input_data/ts2_boj14837out.txt", "w", stdout);
	std::cin >> T;
	for (int tc = 1; tc <= T; tc++) {
		std::cout << "Case #" << tc << ": ";
		std::cin >> N;
		//std::cout << " N: " << N << " ans: ";
		C3D.resize(N);
		for (int i = 0; i < N; i++) std::cin >> C3D[i];
		Hull3D = convex_hull_3D(C3D);
		if (COL) {
			std::sort(C3D.begin(), C3D.end());
			bool f = on_seg_strong(C3D[0], C3D[N - 1], O3D);
			std::cout << (!f ? "NO\n" : "YES\n");
		}
		else if (COP) {
			Pos3D nrm = cross(C3D[0], C3D[1], C3D[2]);
			ll cop = nrm * (O3D - C3D[0]);
			bool f = 0;
			if (!cop) {
				C2D.resize(N);
				if (!(nrm * Z_axis)) {
					if (!(nrm * Y_axis)) for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].y, C3D[i].z);
					else for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].x, C3D[i].z);
				}
				else for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].x, C3D[i].y);
				H2D = graham_scan(C2D);
				f = (inner_check_bi_search(H2D, O) > -1) * (!cop);

			}
			//bool f = (inner_check_bi_search(H2D, O) > -1) * (!cop);
			std::cout << (!f ? "NO\n" : "YES\n");
		}
		else {
			int f = inner_check(C3D, Hull3D, O3D);
			std::cout << (f < 0 ? "NO\n" : "YES\n");
		}
	}
	return;
}
int main() { solve(); return 0; }//boj14837 Omnicircumnavigation (Large)

/*

1 3
-280000 -450000 0
-437600 259800 148400
275680 452688 1484

*/

//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//#include <cassert>
//#include <random>
//#include <array>
//#include <tuple>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//typedef std::pair<int, int> pi;
//const ll INF = 1e17;
//const ld TOL = 1e-7;
//const int LEN = 1e3;
//int N, M, T, Q;
//bool COL, COP;
//// 3D Convex Hull in O(n log n)
//// Very well tested. Good as long as not all points are coplanar
//// In case of collinear faces, returns arbitrary triangulation
//// Credit: Benq
//// refer to Koosaga
//
//bool zero(const ld& x) { return std::abs(x) < TOL; }
////2D========================================================================2D//
//struct Pos {
//	ll x, y;
//	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ll& n) const { return { x * n, y * n }; }
//	Pos operator / (const ll& n) const { return { x / n, y / n }; }
//	ll operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ll operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
//	Pos operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return x * y; }
//	ll Euc() const { return x * x + y * y; }
//	ll Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) {
//		is >> p.x >> p.y;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) {
//		os << p.x << " " << p.y << "\n";
//		return os;
//	}
//};
//const Pos O = { 0, 0 };
//std::vector<Pos> C2D, H2D;
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
//}
//int inner_check_bi_search(std::vector<Pos>& H, const Pos& p) {//convex
//	int sz = H.size();
//	if (!sz) return -1;
//	if (sz == 1) return p == H[0] ? 0 : -1;
//	if (sz == 2) return on_seg_strong(H[0], H[1], p) ? 0 : -1;
//	if (cross(H[0], H[1], p) < 0 || cross(H[0], H[sz - 1], p) > 0) return -1;
//	if (on_seg_strong(H[0], H[1], p) || on_seg_strong(H[0], H[sz - 1], p)) return 0;
//	int s = 0, e = sz - 1, m;
//	while (s + 1 < e) {
//		m = s + e >> 1;
//		if (cross(H[0], H[m], p) > 0) s = m;
//		else e = m;
//	}
//	if (cross(H[s], H[e], p) > 0) return 1;
//	else if (on_seg_strong(H[s], H[e], p)) return 0;
//	else return -1;
//}
//std::vector<Pos> graham_scan(std::vector<Pos>& C) {
//	std::vector<Pos> H;
//	if (C.size() < 3) {
//		std::sort(C.begin(), C.end());
//		return C;
//	}
//	std::swap(C[0], *min_element(C.begin(), C.end()));
//	std::sort(C.begin() + 1, C.end(), [&](const Pos& p, const Pos& q) -> bool {
//		int ret = ccw(C[0], p, q);
//		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
//		return ret > 0;
//		}
//	);
//	//C.erase(unique(C.begin(), C.end()), C.end());
//	int sz = C.size();
//	for (int i = 0; i < sz; i++) {
//		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
//			H.pop_back();
//		H.push_back(C[i]);
//	}
//	return H;
//}
////3D========================================================================3D//
//struct Pos3D {
//	ll x, y, z;
//	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
//	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
//	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
//	bool operator < (const Pos3D& p) const { return x == p.x ? y == p.y ? z < p.z : y < p.y : x < p.x; }
//	ll operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
//	Pos3D operator / (const Pos3D& p) const {
//		Pos3D ret;
//		ret.x = y * p.z - z * p.y;
//		ret.y = z * p.x - x * p.z;
//		ret.z = x * p.y - y * p.x;
//		return ret;
//	}
//	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
//	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
//	Pos3D operator * (const ll& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
//	Pos3D& operator += (const Pos3D& p) { x += p.x; y += p.y; z += p.z; return *this; }
//	Pos3D& operator *= (const ll& scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
//	ll Euc() const { return x * x + y * y + z * z; }
//	ld mag() const { return sqrtl(Euc()); }
//	friend std::istream& operator >> (std::istream& is, Pos3D& p) {
//		is >> p.x >> p.y >> p.z;
//		return is;
//	}
//	friend std::ostream& operator << (std::ostream& os, const Pos3D& p) {
//		os << p.x << " " << p.y << " " << p.z << "\n";
//		return os;
//	}
//} MAXP{ INF, INF, INF };
//const Pos3D O3D = { 0, 0, 0 };
//const Pos3D X_axis = { 1, 0, 0 };
//const Pos3D Y_axis = { 0, 1, 0 };
//const Pos3D Z_axis = { 0, 0, 1 };
//std::vector<Pos3D> C3D, H3D;
//using Face = std::array<int, 3>;
//std::vector<Face> Hull3D;
//struct Edge {
//	int face_num, edge_num;
//	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
//};
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//ll dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
////int ccw(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3, const Pos3D& norm) {
////	Pos3D CCW = cross(d1, d2, d3);
////	ll ret = CCW * norm;
////	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
////}
//bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
//	ll ret = dot(d1, d3, d2);
//	return !cross(d1, d2, d3).Euc() && ret >= 0;
//}
////bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
////	ll ret = dot(d1, d3, d2);
////	return !cross(d1, d2, d3).Euc() && ret > 0;
////}
//bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
//	return ((b - a) / (c - b)).Euc() == 0;
//}
//bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
//	return cross(a, b, c) * (p - a) == 0;
//}
//bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
//	return cross(a, b, c) * (p - a) > 0;
//}
//int prep(std::vector<Pos3D>& p) {//refer to Koosaga'
//	shuffle(p.begin(), p.end(), std::mt19937(0x14004));
//	int dim = 1;
//	assert(p[0] != O3D);
//	for (int i = 1; i < p.size(); i++) {
//		assert(p[i] != O3D);
//		if (dim == 1) {
//			if (p[0] != p[i]) std::swap(p[1], p[i]), ++dim;
//		}
//		else if (dim == 2) {
//			if (!collinear(p[0], p[1], p[i]))
//				std::swap(p[2], p[i]), ++dim;
//		}
//		else if (dim == 3) {
//			if (!coplanar(p[0], p[1], p[2], p[i]))
//				std::swap(p[3], p[i]), ++dim;
//		}
//	}
//	//assert(dim == 4);
//	return dim;
//}
//std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {
//	// 3D Convex Hull in O(n log n)
//	// Very well tested. Good as long as not all points are coplanar
//	// In case of collinear faces, returns arbitrary triangulation
//	// Credit: Benq
//	// refer to Koosaga'
//	COL = 0; COP = 0;
//	int suf = prep(candi);
//	if (suf == 2) { COL = 1; return {}; };
//	if (suf == 3) { COP = 1; return {}; };
//	int sz = candi.size();
//	//prep(candi);
//	//int sz = candi.size();
//	std::vector<Face> faces;
//	std::vector<int> active;//whether face is active - face faces outside 
//	std::vector<std::vector<int>> vis(sz);//faces visible from each point
//	std::vector<std::vector<int>> rvis;//points visible from each face
//	std::vector<std::array<Edge, 3>> other;//other face adjacent to each edge of face
//	auto ad = [&](const int& a, const int& b, const int& c) -> void {//add face
//		faces.push_back({ a, b, c });
//		active.push_back(1);
//		rvis.emplace_back();
//		other.emplace_back();
//		return;
//		};
//	auto visible = [&](const int& a, const int& b) -> void {
//		vis[b].push_back(a);
//		rvis[a].push_back(b);
//		return;
//		};
//	auto abv = [&](const int& a, const int& b) -> bool {//above
//		Face tri = faces[a];
//		return above(candi[tri[0]], candi[tri[1]], candi[tri[2]], candi[b]);
//		};
//	auto edge = [&](const Edge& e) -> pi {
//		return { faces[e.face_num][e.edge_num], faces[e.face_num][(e.edge_num + 1) % 3] };
//		};
//	auto glue = [&](const Edge& a, const Edge& b) -> void {//link two faces by an edge
//		pi x = edge(a); assert(edge(b) == pi(x.second, x.first));
//		other[a.face_num][a.edge_num] = b;
//		other[b.face_num][b.edge_num] = a;
//		return;
//		};//ensure face 0 is removed when i = 3
//
//	ad(0, 1, 2), ad(0, 2, 1);
//	if (abv(1, 3)) std::swap(candi[1], candi[2]);
//	for (int i = 0; i < 3; i++) glue({ 0, i }, { 1, 2 - i });
//	for (int i = 3; i < N; i++) visible(abv(1, i), i);//coplanar points go in rvis[0]
//
//	std::vector<int> label(sz, -1);
//	for (int i = 3; i < sz; i++) {//incremental construction
//		std::vector<int> rem;
//		for (auto& v : vis[i]) if (active[v]) { active[v] = 0, rem.push_back(v); }
//		if (!rem.size()) continue;//hull unchanged
//
//		int st = -1;//start idx
//		for (const int& v : rem) {
//			for (int j = 0; j < 3; j++) {
//				int o = other[v][j].face_num;
//				if (active[o]) {//create new face!
//					int idx1, idx2;
//					std::tie(idx1, idx2) = edge({ v, j });
//					ad(idx1, idx2, i);
//					st = idx1;
//					int cur = rvis.size() - 1;
//					label[idx1] = cur;
//
//					std::vector<int> tmp;
//					set_union(rvis[v].begin(), rvis[v].end(), rvis[o].begin(), rvis[o].end(), back_inserter(tmp));
//					//merge sorted vectors ignoring duplicates
//
//					for (auto& x : tmp) if (abv(cur, x)) visible(cur, x);
//					//if no rounding errors then guaranteed that only x > i matters
//
//					glue({ cur, 0 }, other[v][j]);//glue old, new face
//				}
//			}
//		}
//		for (int x = st, y; ; x = y) {//glue new faces together
//			int X = label[x];
//			glue({ X, 1 }, { label[y = faces[X][1]], 2 });
//			if (y == st) break;
//		}
//	}
//	std::vector<Face> hull3D;
//	for (int i = 0; i < faces.size(); i++) if (active[i]) hull3D.push_back(faces[i]);
//	return hull3D;
//}
//int above(const std::vector<Pos3D>& C, const Face& F, const Pos3D& p) {
//	Pos3D nrm = cross(C[F[0]], C[F[1]], C[F[2]]);
//	ll ret = nrm * (p - C[F[0]]);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//int inner_check(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
//	bool cop = 0;
//	bool out = 0;
//	for (const Face& face : F) {
//		int f = above(C, face, p);
//		if (f == 0) cop = 1;
//		if (f > 0) out = 1;
//	};
//	if (!cop && out) return -1;
//	if (cop) return 0;
//	return 1;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	//freopen("../../../input_data/ts2_input.txt", "r", stdin);
//	//freopen("../../../input_data/ts2_boj14837out.txt", "w", stdout);
//	std::cin >> T;
//	for (int tc = 1; tc <= T; tc++) {
//		std::cout << "Case #" << tc << ": ";
//		std::cin >> N;
//		C3D.resize(N);
//		for (int i = 0; i < N; i++) std::cin >> C3D[i];
//		//std::cout << "\n";
//		//for (const Pos3D& p : C) std::cout << p;
//		Hull3D = convex_hull_3D(C3D);
//		//for (const Face& face : Hull3D) {
//		//	for (int i = 0; i < 3; i++) std::cout << candi[face[i]];
//		//	std::cout << "\n";
//		//}
//		if (COL) {
//			std::sort(C3D.begin(), C3D.end());
//			bool f = on_seg_strong(C3D[0], C3D[N - 1], O3D);
//			std::cout << (!f ? "NO\n" : "YES\n");
//		}
//		else if (COP) {
//			Pos3D nrm = cross(C3D[0], C3D[1], C3D[2]);
//			ll cop = nrm * (O3D - C3D[0]);
//			C2D.resize(N);
//			if (!(nrm * Z_axis)) {
//				if (!(nrm * Y_axis)) for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].y, C3D[i].z);
//				else for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].x, C3D[i].z);
//			}
//			else for (int i = 0; i < N; i++) C2D[i] = Pos(C3D[i].x, C3D[i].y);
//			H2D = graham_scan(C2D);
//			//std::cout << "DEBUG\n";
//			//for (const Pos& p : C2D) std::cout << p;
//			//std::cout << "DEBUG\n";
//			//for (const Pos& p : H2D) std::cout << p;
//			//std::cout << "DEBUG\n";
//			bool f = (inner_check_bi_search(H2D, O) > -1) * (!cop);
//			std::cout << (!f ? "NO\n" : "YES\n");
//		}
//		else {
//			int f = inner_check(C3D, Hull3D, O3D);
//			std::cout << (f < 0 ? "NO\n" : "YES\n");
//		}
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj14837 Omnicircumnavigation (Large)