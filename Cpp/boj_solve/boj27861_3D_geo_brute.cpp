#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld INF = 1e17;
const ld TOL = 1e-7;
int N, M, T, Q, tmp[2]{ 0 };
bool V[6];

struct Seq {
	ll x, y;
	Seq(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Seq& s) const { return x == s.x && y == s.y; }
	bool operator < (const Seq& s) const { return x == s.x ? y < s.y : x < s.x; }
	void print() const { std::cout << x << " " << y << "\n"; return; }
}; std::vector<Seq> seq;
bool zero(const ld& x) { return std::abs(x) < TOL; }
struct Pos3D {// 3-dimensional point [x, y, z]
	ld x, y, z;
	Pos3D(ld X = 0, ld Y = 0, ld Z = 0): x(X), y(Y), z(Z) {}
	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
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
	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
	Pos3D& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
	ld mag() const { return sqrtl(x * x + y * y + z * z); }
} candi[6], MAXP{ INF, INF, INF };
std::vector<Pos3D> tri1, tri2;
struct Line3D {// 3-dimensional line [Vec, P0]
	Pos3D dir, p0;
	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)): dir(DIR), p0(P0) {}
};
struct Plane {// 3-dimensional plane [Norm, P0]
	Pos3D norm, p0;
	Plane(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)): norm(NORM), p0(P0) {}
};
Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
	ld ret = dot(d1, d3, d2);
	return zero(cross(d1, d2, d3).mag()) && ret > 0;
}
Line3D L(const Pos3D& p1, const Pos3D& p2) { return { p2 - p1, p1 }; }
Plane P(const Pos3D& p1, const Pos3D& p2, const Pos3D& p3) {
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Plane(norm, p1);
}
Plane P(std::vector<Pos3D>& tri) {
	Pos3D p1 = tri[0], p2 = tri[1], p3 = tri[2];
	Pos3D norm = (p2 - p1) / (p3 - p2);
	return Plane(norm, p1);
}
Pos3D intersection(const Plane& S, const Line3D& l) {
	ld det = S.norm * l.dir;
	if (zero(det)) return { INF, INF, INF };
	ld t = (S.norm * S.p0 - S.norm * l.p0) / det;
	return l.p0 + (l.dir * t);
}
int inner_check(std::vector<Pos3D>& H, const Pos3D& p) {
	int sz = H.size();
	if (sz <= 1) return -1;
	if (sz == 2) {
		if (on_seg_strong(H[0], H[1], p)) return 0;
		else return -1;
	}
	Pos3D torque0 = cross(H[0], H[1], p);
	for (int i = 1; i < sz; i++) {
 		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
		Pos3D torqueI = cross(cur, nxt, p);
		if (zero(torqueI.mag())) {
			if (on_seg_strong(cur, nxt, p)) return 0;
			else return -1;
		}
		if (torque0 * torqueI < 0) return -1;
	}
	return 1;
}
bool brute(const Seq& s) {
	bool IN = 0, OUT = 0;
	Plane TRI2 = P(tri2);
	for (int i = 0; i < 3; i++) {
		Pos3D cur = tri1[i], nxt = tri1[(i + 1) % 3];
		Pos3D inx = intersection(TRI2, L(cur, nxt));
		if (inx == MAXP) continue;
		if (on_seg_weak(cur, nxt, inx) || cur == inx) {
			if (inner_check(tri2, inx) == 1) IN = 1;
			if (inner_check(tri2, inx) == -1) OUT = 1;
		}
	}
	if (IN == 1 && OUT == 1) seq.push_back({ s });
	return IN && OUT;
}
void dfs(int depth = 0, int idx = 1) {
	if (depth == 2) {
		int n = 0;
		tri2.clear();
		for (int j = 1; j < 6; j++) {
			if (V[j]) tmp[n++] = j;
			if (!V[j]) tri2.push_back(candi[j]);
		}
		brute(Seq(tmp[0] + 1, tmp[1] + 1));
		return;
	}
	for (int i = idx; i < 6; i++) {
		if (!V[i]) {
			tri1.push_back(candi[i]);
			V[i] = 1;
			dfs(depth + 1, i + 1);
			tri1.pop_back();
			V[i] = 0;
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	for (int i = 0; i < 6; i++) std::cin >> candi[i].x >> candi[i].y >> candi[i].z;
	tri1.push_back(candi[0]); V[0] = 1;
	dfs();
	std::sort(seq.begin(), seq.end());
	int sz = seq.size();
	std::cout << sz << "\n";
	for (int i = 0; i < sz; i++) seq[i].print();
	return;
}
int main() { solve(); return 0; }//boj27861 Linked Triangles


//#include <iostream>
//#include <algorithm>
//#include <vector>
//#include <cmath>
//#include <cstring>
//typedef long long ll;
////typedef long double ld;
//typedef double ld;
//const ld INF = 1e17;
//const ld TOL = 1e-7;
//int N, M, T, Q, tmp[2]{ 0 };
//bool V[6];
//
//struct Seq {
//	ll x, y;
//	Seq(ll X = 0, ll Y = 0) : x(X), y(Y) {}
//	bool operator == (const Seq& s) const { return x == s.x && y == s.y; }
//	bool operator < (const Seq& s) const { return x == s.x ? y < s.y : x < s.x; }
//	void print() const { std::cout << x << " " << y << "\n"; return; }
//}; std::vector<Seq> seq;
//bool zero(const ld& x) { return std::abs(x) < TOL; }
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
//	ld mag() const { return hypot(x, y); }
//	void print() const { std::cout << x << " " << y << "\n"; return; }
//}; std::vector<Pos> ans;
//struct Pos3D {
//	ld x, y, z;
//	Pos3D(ld X = 0, ld Y = 0, ld Z = 0): x(X), y(Y), z(Z) {}
//	bool operator == (const Pos3D& p) const { return zero(x - p.x) && zero(y - p.y) && zero(z - p.z); }
//	ld operator * (const Pos3D& p) const { return x * p.x + y * p.y + z * p.z; }
//	Pos3D operator / (const Pos3D& p) const {
//		Pos3D ret;
//		ret.x = y * p.z - z * p.y;
//		ret.y = z * p.x - x * p.z;
//		ret.z = x * p.y - y * p.x;
//		return ret;
//	}
//	Pos3D operator + (const Pos3D& p) const { return { x + p.x, y + p.y, z + p.z }; }
//	Pos3D operator - (const Pos3D& p) const { return { x - p.x, y - p.y, z - p.z }; }
//	Pos3D operator * (const ld& scalar) const { return { x * scalar, y * scalar, z * scalar }; }
//	Pos3D& operator += (const Pos3D& p) { x + p.x; y + p.y; z + p.z; return *this; }
//	Pos3D& operator *= (const ld& scalar) { x * scalar; y * scalar; z * scalar; return *this; }
//	ld mag() const { return sqrtl(x * x + y * y + z * z); }
//} candi[6], MAXP{ INF, INF, INF };
//std::vector<Pos3D> tri1, tri2;
//struct Line3D {
//	Pos3D dir, p0;
//	Line3D(Pos3D DIR = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)): dir(DIR), p0(P0) {}
//};
//struct Plane {
//	Pos3D norm, p0;
//	Plane(Pos3D NORM = Pos3D(0, 0, 0), Pos3D P0 = Pos3D(0, 0, 0)): norm(NORM), p0(P0) {}
//};
//Pos3D cross(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) / (d3 - d2); }
//ld dot(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) { return (d2 - d1) * (d3 - d2); }
//bool on_seg_strong(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
//	ld ret = dot(d1, d3, d2);
//	return zero(cross(d1, d2, d3).mag()) && (ret > 0 || zero(ret));
//}
//bool on_seg_weak(const Pos3D& d1, const Pos3D& d2, const Pos3D& d3) {
//	ld ret = dot(d1, d3, d2);
//	return zero(cross(d1, d2, d3).mag()) && ret > 0;
//}
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
// 		Pos3D cur = H[i], nxt = H[(i + 1) % sz];
//		Pos3D torqueI = cross(cur, nxt, p);
//		if (zero(torqueI.mag())) {
//			if (on_seg_strong(cur, nxt, p)) return 0;
//			else return -1;
//		}
//		if (torque0 * torqueI < 0) return -1;
//	}
//	return 1;
//}
//bool brute(const Seq& s) {
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
//void brute() {
//	for (int i = 1; i < 5; i++) {
//		tri1.push_back(candi[i]);
//		for (int j = i + 1; j < 6; j++) {
//			tri1.push_back(candi[j]);
//			for (int k = 1; k < 6; k++)
//				if (k != i && k != j)
//					tri2.push_back(candi[k]);
//			if (brute(Seq(0, 0))) seq.push_back(Seq(i + 1, j + 1));
//			tri1.pop_back();
//			tri2.clear();
//		}
//		tri1.pop_back();
//	}
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	for (int i = 0; i < 6; i++) std::cin >> candi[i].x >> candi[i].y >> candi[i].z;
//	tri1.push_back(candi[0]); V[0] = 1;
//	brute();
//	//std::sort(seq.begin(), seq.end());
//	int sz = seq.size();
//	std::cout << sz << "\n";
//	for (int i = 0; i < sz; i++) seq[i].print();
//	return;
//}
//int main() { solve(); return 0; }//boj27861 Linked Triangles