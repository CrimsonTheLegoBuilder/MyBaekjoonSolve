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
const ll INF = 1e17;
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
	ll x, y, z;
public:
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	//bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	//bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
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
	//Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator *= (const ll& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	//Pos3D& operator *= (const ld& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	friend std::ostream& operator << (std::ostream& os, const Pos3D& p);
	friend std::istream& operator >> (std::istream& is, Pos3D& p);
} willy, MAXP{ INF, INF, INF };
std::ostream& operator << (std::ostream& os, const Pos3D& p) {
	os << p.x << " " << p.y << " " << p.z << "\n";
	return os;
}
std::istream& operator >> (std::istream& is, Pos3D& p) {
	is >> p.x >> p.y >> p.z;
	return is;
}
std::vector<Pos3D> candi;
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
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret >= 0;
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret > 0;
}
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return ((b - a) / (c - b)).Euc() == 0;
}
int dot_cross(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a);
}
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a) == 0;
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
ld dist(const std::vector<Pos3D>& C, const Face& F, const Pos3D& p) {
	Pos3D norm = cross(C[F[0]], C[F[1]], C[F[2]]);
	ll ret = norm * (p - C[F[0]]);
	return std::abs(ret / (norm.mag()));
}
ld get_min_dist(const std::vector<Pos3D>& C, const std::vector<Face>& F, const Pos3D& p) {
	ld MIN = INF;
	for (const Face& face : F) MIN = std::min(MIN, dist(C, face, p));
	return MIN;
}
std::vector<Face> convex_hull_3D(std::vector<Pos3D>& candi) {
	// 3D Convex Hull in O(n log n)
	// Very well tested. Good as long as not all points are coplanar
	// In case of collinear faces, returns arbitrary triangulation
	// Credit: Benq
	// refer to Koosaga'
	prep(candi);
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
	std::cout << std::fixed;
	std::cout.precision(4);
	while (1) {
		std::cin >> N;
		if (!N) return;
		candi.resize(N);
		for (int i = 0; i < N; i++) std::cin >> candi[i];
		Hull3D = convex_hull_3D(candi);
		//for (const Face& face : Hull3D) {
		//	for (int i = 0; i < 3; i++) std::cout << candi[face[i]];
		//	std::cout << "\n";
		//}
		std::cin >> Q;
		while (Q--) {
			std::cin >> willy;
			std::cout << get_min_dist(candi, Hull3D, willy) << "\n";
		}
	}
	return;
}
int main() { solve(); return 0; }//boj4795 The Worm in the Apple - refer to koosaga

/*

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
const ll INF = 1e17;
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
	ll x, y, z;
	Pos3D(ll X = 0, ll Y = 0, ll Z = 0) : x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Pos3D& p) const { return x != p.x || y != p.y || z != p.z; }
	//bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
	//bool operator != (const Pos3D& p) const { return !zero(x - p.x) || !zero(y - p.y) || !zero(z - p.z); }
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
	//Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator *= (const ll& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	//Pos3D& operator *= (const ld& scalar) { x* scalar; y* scalar; z* scalar; return *this; }
	ll Euc() const { return x * x + y * y + z * z; }
	ld mag() const { return sqrtl(Euc()); }
	void print() const { std::cout << x << " " << y << " " << z << "\n"; return; }
	void scan() { std::cin >> x >> y >> z; return; }
} candi[LEN], willy, MAXP{ INF, INF, INF };
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
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret >= 0;
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ll ret = dot(d1, d3, d2);
	return !cross(d1, d2, d3).mag() && ret > 0;
}
bool collinear(const Pos3D& a, const Pos3D& b, const Pos3D& c) {
	return ((b - a) / (c - b)).Euc() == 0;
	//return zero(((b - a) / (c - b)).Euc());
}
int dot_cross(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a);
}
bool coplanar(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {
	return cross(a, b, c) * (p - a) == 0;
	//return zero(cross(a, b, c) * (p - a));
}
bool above(const Pos3D& a, const Pos3D& b, const Pos3D& c, const Pos3D& p) {// is p strictly above plane
	return cross(a, b, c) * (p - a) > 0;
}
//refer to Koosaga'
void prep(std::vector<Pos3D>& p) {
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
}
//refer to Koosaga'
using Face = std::array<int, 3>;
typedef std::pair<int, int> pi;// <tri-face.num, edge_num> - Edge
struct Edge {
	int face_num, edge_num;
	Edge(int t = 0, int v = 0) : face_num(t), edge_num(v) {}
};
std::vector<Face> Fast_convex_hull_3D(std::vector<Pos3D>& p) {
	prep(p);
	int N = p.size();
	std::vector<Face> hull;
	std::vector<int> active;//whether face is active - face faces outside
	std::vector<std::vector<int>> rvis;//points visible from each face
	std::vector<std::array<pi, 3>> other;//other face adjacent to each edge of face
	std::vector<std::vector<int>> vis(N);//faces visible from each point
	auto ad = [&](int a, int b, int c) -> void {//add face
		hull.push_back({ a, b, c });
		active.push_back(1);
		rvis.emplace_back();
		other.emplace_back();
	};
	auto ae = [&](int a, int b) -> void {//visible check
		vis[b].push_back(a);
		rvis[a].push_back(b);
	};
	auto abv = [&](int a, int b) -> bool {//above
		Face f = hull[a];
		return above(p[f[0]], p[f[1]], p[f[2]], p[b]);
	};
	auto edge = [&](pi e) -> pi {
		return { hull[e.first][e.second], hull[e.first][(e.second + 1) % 3] };
	};
	auto glue = [&](pi a, pi b) -> void {//link two faces by an edge
		pi x = edge(a); assert(edge(b) == pi(x.second, x.first));
		other[a.first][a.second] = b;
		other[b.first][b.second] = a;
	};//ensure face 0 is removed when i=3

	ad(0, 1, 2), ad(0, 2, 1);

	if (abv(1, 3)) std::swap(p[1], p[2]);

	for (int i = 0; i < 3; i++) glue({ 0, i }, { 1, 2 - i });
	for (int i = 3; i < N; i++) ae(abv(1, i), i);//coplanar points go in rvis[0]
	std::vector<int> label(N, -1);
	for (int i = 3; i < N; i++) {//incremental construction
		std::vector<int> rem;
		for (auto& t : vis[i]) if (active[t]) { active[t] = 0, rem.push_back(t); }

		if (!rem.size()) continue;//hull unchanged

		int st = -1;//start idx
		for (const int& r : rem) {
			for (int j = 0; j < 3; j++) {
				int o = other[r][j].first;
				if (active[o]) {//create new face!
					int a, b; std::tie(a, b) = edge({ r, j }); ad(a, b, i); st = a;
					int cur = rvis.size() - 1; label[a] = cur;
					std::vector<int> tmp;
					set_union(rvis[r].begin(), rvis[r].end(), rvis[o].begin(), rvis[o].end(), back_inserter(tmp));
					//merge sorted vectors ignoring duplicates
					for (auto& x : tmp) if (abv(cur, x)) ae(cur, x);
					//if no rounding errors then guaranteed that only x > i matters
					glue({ cur, 0 }, other[r][j]);//glue old, new face
				}
			}
		}
		for (int x = st, y;; x = y) {//glue new faces together
			int X = label[x]; glue({ X, 1 }, { label[y = hull[X][1]], 2 });
			if (y == st) break;
		}
	}
	std::vector<Face> H3D;
	for (int i = 0; i < hull.size(); i++) if (active[i]) H3D.push_back(hull[i]);
	return H3D;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	//for (int i = 0; i < N; i++) std::cin >> candi[i].x >> candi[i].y >> candi[i].z;
	for (int i = 0; i < N; i++) candi[i].scan();
	std::cin >> Q;
	while (Q--) {
		willy.scan();

	}
	std::cin >> T;
	return;
}
int main() { solve(); return 0; }//boj4795 The Worm in the Apple

//Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
//Plane P(const Pos3D& p1, const Pos3D& p2, const Pos3D& p3) {
//	Pos3D norm = (p2 - p1) / (p3 - p2);
//	return Plane(norm, p1);
//}
//Plane P(std::vector<Pos3D>& tri) {
//	Pos3D p1 = tri[0], p2 = tri[1], p3 = tri[2];
//	Pos3D norm = (p2 - p1) / (p3 - p2);
//	return Plane(norm, p1);
//}
//Pos3D intersection(const Plane& S, const Line3D& l) {
//	ld det = S.norm * l.dir;
//	if (zero(det)) return { INF, INF, INF };
//	ld t = (S.norm * S.p0 - S.norm * l.p0) / det;
//	return l.p0 + (l.dir * t);
//}
//int inner_check(std::vector<Pos3D>& H, const Pos3D& p) {
//	int sz = H.size();
//	if (sz <= 1) return -1;
//	if (sz == 2) {
//		if (on_seg_strong(H[0], H[1], p)) return 0;
//		else return -1;
//	}
//	Pos3D torque0 = cross(H[0], H[1], p);
//	for (int i = 1; i < sz; i++) {
//		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
//		Pos3D torqueI = cross(cur, nxt, p);
//		if (zero(torqueI.mag())) {
//			if (on_seg_strong(cur, nxt, p)) return 0;
//			else return -1;
//		}
//		if (torque0 * torqueI < 0) return -1;
//	}
//	return 1;
//}
//bool brute() {
//	bool IN = 0, OUT = 0;
//	Plane TRI2 = P(tri2);
//	for (int i = 0; i < 3; i++) {
//		Pos3D cur = tri1[i], nxt = tri1[(i + 1) % 3];
//		Pos3D inx = intersection(TRI2, L(cur, nxt));
//		if (inx == MAXP) continue;
//		if (on_seg_weak(cur, nxt, inx) || cur == inx) {
//			if (inner_check(tri2, inx) == 1) IN = 1;
//			if (inner_check(tri2, inx) == -1) OUT = 1;
//		}
//	}
//	return IN && OUT;
//}


*/