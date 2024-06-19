#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
typedef std::vector<ld> vld;
const ld INF = 1e17;
const ld TOL = 1e-13;
const ld PI = acosl(-1);
const int LEN = 205;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(int x) { return (ll)x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}

enum Geo {
	TRI,
	CIR,
};
int N;
ld A[LEN][LEN];
struct Pos {
	ld x, y;
	inline Pos(ld x = 0, ld y = 0) : x(x), y(y) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& s) const { return { x * s, y * s }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	inline Pos operator - () const { return { -x, -y }; }
	inline Pos operator ~ () const { return { -y, x }; }
	inline ld Euc() const { return x * x + y * y; }
	inline ld mag() const { return sqrt(Euc()); }
	inline ld rad() const { return norm(atan2(y, x)); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	inline friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
};
struct Seg {
	Pos s, e;
	inline Seg(Pos S = Pos(), Pos E = Pos()) : s(S), e(E) {}
	inline ld green(const Pos& m, const ld& v) const { return m.y * v * (s.x - e.x); }
};
struct Triangle {
	Pos a, b, c;
	inline Triangle(Pos p = Pos(), Pos q = Pos(), Pos r = Pos()) {
		if ((q - p) / (r - q) < 0) std::swap(q, r);
		a = p; b = q; c = r;
	}
	inline int inner_check(const Pos& p, const Pos& v) const {
		ld f1 = (b - a) / (p - a);
		ld f2 = (c - b) / (p - b);
		ld f3 = (a - c) / (p - c);
		if (sign(f1) < 0 || sign(f2) < 0 || sign(f3) < 0) return 0;
		if (zero(f1)) return (b - a) / v > 0 ? 2 : 0;//on_seg && centripetal
		if (zero(f2)) return (c - b) / v > 0 ? 2 : 0;
		if (zero(f3)) return (a - c) / v > 0 ? 2 : 0;
		return 1;
	}
};
struct Circle {
	Pos c;
	int r;
	inline Circle(Pos C = Pos(), int R = 0) : c(C), r(R) {}
	inline bool operator == (const Circle& C) const { return c == C.c && r == C.r; }
	inline bool operator >= (const Pos& p) const { return sign(r * r - (c - p).Euc()) >= 0; }
	inline ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
	inline ld rad(const Pos& p) const { return (p - c).rad(); }
	inline ld green(const ld& lo, const ld& hi) const {
		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
		ld fan = area(lo, hi);
		Pos m = c + (s + e) * r * (ld).5;
		ld I = (cos(lo) - cos(hi)) * m.y * r;
		return fan + I - (s / e) * r * r * (ld).5;
	}
};
struct Shape {
	Geo type;
	Triangle T;
	Circle C;
	inline Shape(int t = 0, int a = 0, int b = 0, int c = 0, int d = 0, int e = 0, int f = 0) {
		type = (Geo)t;
		if (type == TRI) T = Triangle(Pos(a, b), Pos(c, d), Pos(e, f));
		else if (type == CIR) C = Circle(Pos(a, b), c);
	}
} C[LEN];
inline ld intersection(const Seg& s1, const Seg& s2) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
inline vld circle_line_intersections(const Pos& s, const Pos& e, const Circle& q, const bool& f = 0) {
	//https://math.stackexchange.com/questions/311921/get-location-of-vector-circle-intersection
	Pos vec = e - s;
	Pos OM = s - q.c;
	ld a = vec.Euc();
	ld b = vec * OM;
	ld c = OM.Euc() - q.r * q.r;
	ld J = b * b - a * c;
	if (J < -TOL) return {};
	ld det = sqrt(std::max((ld)0, J));
	ld lo = (-b - det) / a;
	ld hi = (-b + det) / a;
	vld ret;
	if (f) {
		if (0 < hi && hi < 1) ret.push_back(hi);
		if (zero(det)) return ret;
		if (0 < lo && lo < 1) ret.push_back(lo);
	}
	else {
		auto the = [&](ld rt) { return q.rad(s + (e - s) * rt); };
		if (-TOL < hi && hi < 1 + TOL) ret.push_back(the(hi));
		if (zero(det)) return ret;
		if (-TOL < lo && lo < 1 + TOL) ret.push_back(the(lo));
	}
	return ret;
}
inline vld intersection(const Circle& a, const Circle& b) {
	Pos ca = a.c, cb = b.c;
	Pos vec = cb - ca;
	ll ra = a.r, rb = b.r;
	ld distance = vec.mag();
	ld rd = vec.rad();
	if (vec.Euc() > sq(ra + rb) + TOL) return {};
	if (vec.Euc() < sq(ra - rb) - TOL) return {};
	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
	if (X < -1) X = -1;
	if (X > 1) X = 1;
	ld h = acos(X);
	vld ret = {};
	ret.push_back(norm(rd + h));
	if (zero(h)) return ret;
	ret.push_back(norm(rd - h));
	return ret;
}
inline void green_seg(const Seg& S, const int& I) {//refer to cki86201
	if (!sign(S.s.x - S.e.x)) return;
	vld VA = { 0, 1 }, inx;
	for (int j = 0; j < N; j++) {
		if (I == j) continue;
		if (C[j].type == TRI) {
			Pos* tri[] = { &C[j].T.a, &C[j].T.b, &C[j].T.c };
			for (int k = 0; k < 3; k++) {
				const Pos& q1 = *tri[k], & q2 = *tri[(k + 1) % 3];
				ld r = intersection(S, Seg(q1, q2));
				if (r > -.5) VA.push_back(r);
			}
		}
		if (C[j].type == CIR) {
			inx = circle_line_intersections(S.s, S.e, C[j].C, 1);
			for (const ld& r : inx) VA.push_back(r);
		}
	}
	std::sort(VA.begin(), VA.end());
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
			if (C[j].type == TRI) {
				int f = C[j].T.inner_check(m, pet);//centripetal
				if (f == 2 || C[j].T.inner_check(m, fug) == 2) col = std::max(col, j);
				if (f) prev = std::max(prev, j);
			}
			if (C[j].type == CIR) if (C[j].C >= m) prev = std::max(prev, j);
			if (prev >= j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == TRI) {
				if (C[j].T.inner_check(m, pet)) val = std::min(val, j);//centripetal
				if (C[j].T.inner_check(m, fug)) inval = std::min(inval, j);//centrifugal
			}
			if (C[j].type == CIR) {
				if (C[j].C >= m) {
					val = std::min(val, j);
					inval = std::min(inval, j);
				}
			}
			if (val < N && inval < N) break;
		}
		ld a = S.green(m, d);
		if (prev != -1 && prev > col) { for (int j = I; j < inval; j++) A[j][prev] -= a; }
		for (int j = I; j < val; j++) A[j][I] += a;
	}
	return;
}
inline void green_circle(const int& I) {//refer to cki86201
	vld VA = { 0, 2 * PI }, inx;
	const Circle& q = C[I].C;
	for (int j = 0; j < N; j++) {
		if (I == j || C[j].C == q) continue;
		if (C[j].type == CIR) {
			inx = intersection(q, C[j].C);
			for (const ld& r : inx) VA.push_back(r);
		}
		if (C[j].type == TRI) {
			Pos* tri[] = { &C[j].T.a, &C[j].T.b, &C[j].T.c };
			for (int k = 0; k < 3; k++) {
				const Pos& p1 = *tri[k], & p2 = *tri[(k + 1) % 3];
				inx = circle_line_intersections(p1, p2, q);
				for (const ld& r : inx) VA.push_back(r);
			}
		}
	}
	std::sort(VA.begin(), VA.end());
	int sz = VA.size();
	for (int i = 0; i < sz - 1; i++) {
		ld d = VA[i + 1] - VA[i];
		if (zero(d)) continue;
		ld t = (VA[i + 1] + VA[i]) * .5;
		Pos R = Pos(q.r * cos(t), q.r * sin(t));
		Pos pet = -R;
		Pos m = q.c + R;
		int prev = -1, val = N;
		for (int j = I - 1; j >= 0; j--) {
			if (C[j].type == TRI) { if (C[j].T.inner_check(m, pet)) prev = std::max(prev, j); }
			if (C[j].type == CIR) {
				if (C[j].C == q) continue;
				if (C[j].C >= m) prev = std::max(prev, j);
			}
			if (prev >= j) break;
		}
		for (int j = I + 1; j < N; j++) {
			if (C[j].type == TRI) { if (C[j].T.inner_check(m, pet)) val = std::min(val, j); }
			if (C[j].type == CIR) { if (C[j].C == q || C[j].C >= m) val = std::min(val, j); }
			if (val < N) break;
		}
		ld a = q.green(VA[i], VA[i + 1]);
		if (prev != -1) { for (int j = I; j < val; j++) A[j][prev] -= a; }
		for (int j = I; j < val; j++) A[j][I] += a;
	}
	return;
}
inline void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(18);
	std::cin >> N;
	int t = 0, a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	for (int i = 0; i < N; i++) {
		std::cin >> t;
		if (t == 1) std::cin >> a >> b >> c >> d >> e >> f;
		else if (t == 2) std::cin >> a >> b >> c;
		C[i] = Shape(t - 1, a, b, c, d, e, f);
	}
	memset(A, 0, sizeof A);
	for (int i = 0; i < N; i++) {
		if (C[i].type == TRI) {
			green_seg(Seg(C[i].T.a, C[i].T.b), i);
			green_seg(Seg(C[i].T.b, C[i].T.c), i);
			green_seg(Seg(C[i].T.c, C[i].T.a), i);
		}
		if (C[i].type == CIR) green_circle(i);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= i; j++) std::cout << A[i][j] << " ";
		std::cout << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj11392 paper refer to cki86201
