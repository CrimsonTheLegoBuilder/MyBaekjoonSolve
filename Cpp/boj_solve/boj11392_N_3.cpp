#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <deque>
typedef long long ll;
//typedef double ld;
typedef long double ld;
//typedef __float128 ld;
typedef std::vector<ld> vld;
const ld INF = 1e17;
const ld TOL = 1e-15;
const ld PI = acos(-1);
const int LEN = 205;
inline int sign(const ld& x) { return x <= -TOL ? -1 : x >= TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (sign(th) < 0) th += PI * 2;
	while (sign(th - PI * 2) >= 0) th -= PI * 2;
	return th;
}

#define triangle 0
#define circle 1
//#define DEBUG
//#define ASSERT

int N, M, T, Q;
bool V[LEN];
ld A[LEN][LEN];
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y){}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	inline Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	inline Pos rot(ld the) const { return Pos(x * cos(the) - y * sin(the), x * sin(the) + y * cos(the)); }
	inline ld Euc() const { return x * x + y * y; }
	//ld mag() const { return sqrt(Euc()); }
	ld mag() const { return hypotl(x, y); }
	Pos unit() const { return *this / mag(); }
	inline ld rad() const { return norm(atan2l(y, x)); }
	inline friend ld rad(const Pos& p1, const Pos& p2) { return norm(atan2l(p1 / p2, p1 * p2)); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return sign(ret);
}
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
inline void norm(const Pos& s, const Pos& e, Pos& p, Pos& q) { if (dot(s, e, p, q) < 0) std::swap(p, q); }
inline ld dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && sign(ret) >= 0;
}
inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && sign(ret) > 0;
}
inline Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
inline Polygon convex_cut(const std::vector<Pos>& ps, const Pos& b1, const Pos& b2) {
	std::vector<Pos> qs;
	int n = ps.size();
	for (int i = 0; i < n; i++) {
		Pos p1 = ps[i], p2 = ps[(i + 1) % n];
		int d1 = ccw(b1, b2, p1), d2 = ccw(b1, b2, p2);
		if (d1 >= 0) qs.push_back(p1);
		if (d1 * d2 < 0) qs.push_back(intersection(p1, p2, b1, b2));
	}
	return qs;
}
inline Polygon sutherland_hodgman(const std::vector<Pos>& C, const std::vector<Pos>& clip) {
	int sz = clip.size();
	std::vector<Pos> ret = C;
	for (int i = 0; i < sz; i++) {
		Pos b1 = clip[i], b2 = clip[(i + 1) % sz];
		ret = convex_cut(ret, b1, b2);
	}
	return ret;
}
struct Triangle;
struct Circle;
struct Circle {
	Pos c;
	int r;
	Circle(Pos C = Pos(0, 0), int R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	inline bool operator < (const Circle& q) const {
		ll rsub = sq(r - q.r);
		return r < q.r && rsub >(c - q.c).Euc() - TOL;
	}
	inline bool operator ^ (const Circle& q) const {
		ll rsum = sq(r + q.r);
		return rsum < (c - q.c).Euc() + TOL;
	}
	inline bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	inline bool operator >= (const Pos& p) const { return r * r + TOL > (c - p).Euc(); }
	inline bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	inline Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	inline Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	inline ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	inline ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
	inline bool operator < (const Triangle& t) const;
};
inline bool cmpr(const Circle& p, const Circle& q) { return p.r > q.r; }//sort descending order
typedef std::vector<Circle> Disks;
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc, 0 ~ 2pi
	int i;
	Arc(ld LO = 0, ld HI = 0, int I = -1) : lo(LO), hi(HI), i(I) {}
	bool operator < (const Arc& a) const { return zero(lo - a.lo) ? hi < a.hi : lo < a.lo; }
	inline ld area(const Circle& cen) const { return (hi - lo) * cen.r * cen.r; }
	inline ld green(const Circle& cen) const {
		Pos LO = -Pos(1, 0).rot(lo) * cen.r;
		Pos HI = Pos(1, 0).rot(hi) * cen.r;
		Pos vec = Pos(cen.c.x, cen.c.y);
		return (area(cen) + vec / (HI + LO)) * .5;
	}
	friend std::ostream& operator << (std::ostream& os, const Arc& l) { os << l.lo << " " << l.hi << " " << l.i; return os; }
};
struct Seg {
	Pos s, e;
	Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	inline ld green(const ld& v) const { return cross(O, s, e) * v * .5; }
};
inline ld intersection(const Seg& s1, const Seg& s2) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = cross(q1, q2, p1, p2);
	if (zero(det)) return -1;
	ld a1 = cross(q1, q2, p1, q1) / det;
	ld a2 = cross(p1, p2, q1, p1) / -det;
	if (sign(a1 - 0) >= 0 && sign(1 - a1) >= 0 && sign(a2 - 0) >= 0 && sign(1 - a2) >= 0) return a1;
	return -1;
}
inline void norm(const Seg& S, Pos& p, Pos& q) { norm(S.s, S.e, p, q); }
struct Triangle {
	Pos a, b, c;
	Triangle(Pos p = Pos(), Pos q = Pos(), Pos r = Pos()) {
		if (ccw(p, q, r) < 0) std::swap(q, r);
		a = p; b = q; c = r;
	}
	inline ld area() const { return cross(a, b, c); }
	inline int inner_check(const Pos& p, const Pos& v) const {
		ld f1 = cross(a, b, p);
		ld f2 = cross(b, c, p);
		ld f3 = cross(c, a, p);
		if (sign(f1) < 0 || sign(f2) < 0 || sign(f3) < 0) return 0;
		if (zero(f1)) return (b - a) / v > 0 ? 2 : 0;//on_seg && centripetal
		if (zero(f2)) return (c - b) / v > 0 ? 2 : 0;
		if (zero(f3)) return (a - c) / v > 0 ? 2 : 0;
		return 1;
	}
	inline bool inner_check(const Triangle& q) const {
		auto inner = [&](const Pos& p) -> bool {
			return sign(ccw(a, b, p)) >= 0
				&& sign(ccw(b, c, p)) >= 0
				&& sign(ccw(c, a, p)) >= 0;
			};
		return inner(q.a) && inner(q.b) && inner(q.c);
	}
	inline bool operator < (const Triangle& q) const {
		bool f1 = inner_check(q);
		bool f2 = !zero(area() - q.area());
		return f1 && f2;
	}
	inline bool operator == (const Triangle& q) const {
		bool f1 = inner_check(q);
		bool f2 = zero(area() - q.area());
		return f1 && f2;
	}
	inline bool operator < (const Circle& q) const { return q >= a && q >= b && q >= c; }
};
inline bool Circle::operator < (const Triangle& t) const {
	ld d1 = dist(t.a, t.b, c);
	ld d2 = dist(t.b, t.c, c);
	ld d3 = dist(t.c, t.a, c);
	return sign(d1 - r) >= 0 && sign(d2 - r) >= 0 && sign(d3 - r) >= 0;
}
struct Confetti {
	bool type;
	Triangle T;
	Circle C;
	int i;
	Confetti(int t = 0, int a = 0, int b = 0, int c = 0, int d = 0, int e = 0, int f = 0, int I = -1) : i(I) {
		type = t;
		if (type == triangle) T = Triangle(Pos(a, b), Pos(c, d), Pos(e, f));
		else if (type == circle) C = Circle(Pos(a, b), c);
	}
	inline bool operator < (const Confetti& cf) const {
		if (type == triangle && cf.type == triangle) return T < cf.T;
		if (type == circle && cf.type == circle) return C < cf.C;
		if (type == triangle && cf.type == circle) return T < cf.C;
		if (type == circle && cf.type == triangle) return C < cf.T;
	}
	inline bool operator == (const Confetti& cf) const {
		if (type != cf.type) return 0;
		if (type == triangle) return T == cf.T;
		else if (type == circle) return C == cf.C;
	}
} C[LEN];
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Pos& p, const ld& r) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - p;
	ld a = vec * vec;
	ld b = 2 * (vec * OM);
	ld c = OM * OM - r * r;
	ld J = b * b - 4 * a * c;
	//if (J < TOL) return {};
	if (sign(J) < 0) return {};
	ld lo = (-b - sqrt(J)) / (2 * a);
	ld hi = (-b + sqrt(J)) / (2 * a);
	if (sign(hi - 0) < 0 || sign(lo - 1) > 0) return {};
	return { { lo, hi } };//ratio, ratio
}
std::vector<Pos> circle_line_intersections(const Pos& s, const Pos& e, const Circle& c) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	return circle_line_intersections(s, e, c.c, c.r);
}
inline std::vector<Pos> intersection(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();

	if (sign(vec.Euc() - sq(ra + rb)) > 0) return {};
	if (sign(vec.Euc() - sq(ra - rb)) < 0) return {};

	//2nd hyprblc law of cos
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	//if (zero(h)) return { Pos(-1, norm(rd)) };
	return { Pos(norm(rd - h), norm(rd + h)) };
}
inline void green_seg(const Seg& S, const vld& VA, const int& I) {//refer to cki86201
	if (!ccw(O, S.s, S.e)) return;
	int sz = VA.size();
	Pos pet = ~(S.e - S.s);
	Pos fug = -pet;
	for (int i = 0; i < sz - 1; i++) {
		ld d = VA[i + 1] - VA[i];
		if (zero(d)) continue;
		ld ratio = (VA[i + 1] + VA[i]) * .5;
		Pos m = S.s + (S.e - S.s) * ratio;

		int prev = -1, col = -1, val = N, inval = N;
		for (int j = I - 1; j >= 0; j--) {
			if (C[j].type == triangle) {
				int f = C[j].T.inner_check(m, pet);//centripetal
				if (f == 2 || C[j].T.inner_check(m, fug) == 2) col = std::max(col, j);
				if (f) prev = std::max(prev, j);
			}
			if (C[j].type == circle) if (C[j].C >= m) prev = std::max(prev, j);
			if (prev >= j && col >= j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == triangle) {
				if (C[j].T.inner_check(m, pet)) val = std::min(val, j);//centripetal
				if (C[j].T.inner_check(m, fug)) inval = std::min(inval, j);//centrifugal
			}
			if (C[j].type == circle) {
				if (C[j].C >= m) {
					val = std::min(val, j);
					inval = std::min(inval, j);
				}
			}
			if (val < N && inval < N) break;
		}
		
		ld a = S.green(d);
		if (prev != -1 && prev > col) {
			for (int j = I; j < inval; j++) A[j][prev] -= a;
		}
		for (int j = I; j < val; j++) A[j][I] += a;
	}
	return;
}
inline void green_triangle(const int& i) {
	vld VA = { 0, 1 }, VB = { 0, 1 }, VC = { 0, 1 };
	for (int j = 0; j < N; j++) {
		if (i == j) continue;
		if (C[j].type == triangle) {
			//auto r = [&](const Pos& p1, const Pos& p2, const Pos& q) -> ld {
			//	return (q - p1).mag() / (p2 - p1).mag();
			//	};
			//Pos& a = C[i].T.a, b = C[i].T.b, c = C[i].T.c;
			//Polygon tri = { C[i].T.a, C[i].T.b, C[i].T.c };
			//Polygon clip = { C[j].T.a, C[j].T.b, C[j].T.c };
			//Polygon HPI = sutherland_hodgman(tri, clip);
			//for (const Pos& p : HPI) {
			//	if (on_seg_weak(a, b, p)) VA.push_back(r(a, b, p));
			//	if (on_seg_weak(b, c, p)) VB.push_back(r(b, c, p));
			//	if (on_seg_weak(c, a, p)) VC.push_back(r(c, a, p));
			//}
			Pos& a = C[i].T.a, b = C[i].T.b, c = C[i].T.c;
			Polygon clip = { C[j].T.a, C[j].T.b, C[j].T.c };
			for (int k = 0; k < 3; k++) {
				const Pos& q1 = clip[k], q2 = clip[(k + 1) % 3];
				ld r1 = intersection(Seg(a, b), Seg(q1, q2));
				if (sign(r1) >= 0) VA.push_back(r1);
				ld r2 = intersection(Seg(b, c), Seg(q1, q2));
				if (sign(r2) >= 0) VB.push_back(r2);
				ld r3 = intersection(Seg(c, a), Seg(q1, q2));
				if (sign(r3) >= 0) VC.push_back(r3);
			}
		}//T && T
		if (C[j].type == circle) {
			Circle& d = C[j].C;
			Polygon inx;
			Pos* tri[] = { &C[i].T.a, &C[i].T.b, &C[i].T.c };
			vld* VT[] = { &VA, &VB, &VC };
			for (int k = 0; k < 3; k++) {
				Pos& a = *tri[k];
				Pos& b = *tri[(k + 1) % 3];
				inx = circle_line_intersections(a, b, d);
				if (inx.size()) {
					Pos ratio = inx[0];
					ld s = inx[0].x;
					ld e = inx[0].y;
					if (sign(s - 0) >= 0) VT[k]->push_back(s);
					if (sign(1 - e) >= 0) VT[k]->push_back(e);
				}
			}
		}//T && C
	}
	std::sort(VA.begin(), VA.end());
	std::sort(VB.begin(), VB.end());
	std::sort(VC.begin(), VC.end());
	green_seg(Seg(C[i].T.a, C[i].T.b), VA, i);
	green_seg(Seg(C[i].T.b, C[i].T.c), VB, i);
	green_seg(Seg(C[i].T.c, C[i].T.a), VC, i);
	return;
}
inline void green_circle(const int& I) {//refer to cki86201
	vld VA = { 0, 2 * PI };
	const Circle& q = C[I].C;
	for (int j = 0; j < N; j++) {
		if (I == j || C[j] == C[I]) continue;
		if (C[j].type == circle) {
			Pos vec = q.c - C[j].C.c;
			int ra = q.r, rb = C[j].C.r;
			if (sign(vec.Euc() - sq(ra + rb)) > 0) continue;
			if (sign(vec.Euc() - sq(ra - rb)) < 0) continue;
			//if (vec.Euc() > sq(ra + rb)) continue;
			//if (vec.Euc() < sq(ra - rb)) continue;

			auto inx = intersection(q, C[j].C);
			if (!inx.size()) continue;
			ld lo = inx[0].x;
			ld hi = inx[0].y;
			if (sign(lo) >= 0) VA.push_back(lo);
			if (sign(hi) >= 0) VA.push_back(hi);
		}//C && C
		if (C[j].type == triangle) {
			Pos& a = C[j].T.a, b = C[j].T.b, c = C[j].T.c;
			Polygon inx, tri = { a, b, c };
			for (int k = 0; k < 3; k++) {
				inx = circle_line_intersections(tri[k], tri[(k + 1) % 3], q);
				if (inx.size()) {
					Pos ratio = inx[0];
					if (sign(ratio.x - 0) >= 0) {
						Pos s = tri[k] + (tri[(k + 1) % 3] - tri[k]) * ratio.x;
						VA.push_back((s - q.c).rad());
					}
					if (sign(ratio.y - 1) >= 0) {
						Pos e = tri[k] + (tri[(k + 1) % 3] - tri[k]) * ratio.y;
						VA.push_back((e - q.c).rad());
					}
				}
			}
		}//C && T
	}
	std::sort(VA.begin(), VA.end());
	int sz = VA.size();
	for (int i = 0; i < sz - 1; i++) {
		ld d = VA[i + 1] - VA[i];
		if (zero(d)) continue;
		ld t = (VA[i + 1] + VA[i]) * .5;
		Pos R = Pos(q.r, 0).rot(t);
		Pos pet = -R;
		Pos m = q.c + R;

		int prev = -1, val = N;
		for (int j = I - 1; j >= 0; j--) {
			if (C[j].type == triangle) if (C[j].T.inner_check(m, pet)) prev = std::max(prev, j);
			if (C[j].type == circle) {
				if (C[j].C == q) continue;
				if (C[j].C >= m) prev = std::max(prev, j);
			}
			if (prev >= j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == triangle) if (C[j].T.inner_check(m, pet)) val = std::min(val, j);
			if (C[j].type == circle) if (C[j].C == q || C[j].C >= m) val = std::min(val, j);
			if (val < N) break;
		}

		ld a = Arc(VA[i], VA[i + 1]).green(q);
		if (prev != -1) {
			for (int j = I; j < val; j++) A[j][prev] -= a;
		}
		for (int j = I; j < val; j++) A[j][I] += a;
	}
	return;
}
inline void green() {
	memset(A, 0, sizeof A);
	for (int i = 0; i < N; i++) {
		if (C[i].type == triangle) green_triangle(i);
		if (C[i].type == circle) green_circle(i);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= i; j++) std::cout << (long double)A[i][j] << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
	return;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	int ret = 0;
	std::cin >> N;
	int t = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> t;
		if (t == 1) std::cin >> a >> b >> c >> d >> e >> f;
		else if (t == 2) std::cin >> a >> b >> c;
		C[i] = Confetti(t - 1, a, b, c, d, e, f, i);
	}
	green();
	return;
}
int main() { solve(); return 0; }//boj11392 Confetti refer to cki86201

/*
4
1 0 0 2 0 0 2
1 1 1 -1 1 1 -1
1 0 0 2 0 0 2
1 1 1 -1 1 1 -1

4
1 0 0 2 0 0 2
1 2 2 2 0 0 2
1 0 0 2 0 0 2
1 2 2 2 0 0 2

4
2 0 0 1
1 0 0 2 0 0 2
2 0 0 1
1 0 0 2 0 0 2
*/
