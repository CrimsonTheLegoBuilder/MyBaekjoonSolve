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
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-9;
const ld PI = acos(-1);
const int LEN = 1e3;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }

Vld ans;
int N, M;
struct Pos {
	ld x, y;
	int i, d;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) { i = 0, d = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	//bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	//bool operator < (const Pos& r) const { return x == r.x ? y == r.y ? d < r.d : y < r.y : x < r.x; }
	bool operator < (const Pos& r) const { return zero(x - r.x) ? zero(y - r.y) ? d < r.d : y < r.y : x < r.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(ld the) const { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << "(" << p.x << ", " << p.y << ")"; return os; }
} pos[LEN << 3]; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
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
	Line(Vec V = Vec(0, 0), Pos p = Pos(0, 0)) : s(V) { c = s.vy * p.x + s.vx * p.y; }
	Line(Pos ps = Pos(0, 0), Pos pe = Pos(0, 0)) {
		ld dy, dx;
		dy = pe.y - ps.y;
		dx = ps.x - pe.x;
		s = Vec(dy, dx);
		c = dy * ps.x + dx * ps.y;
	}
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
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
const Line Xaxis = { { 0, -1 }, 0 };
const Line Yaxis = { { 1, 0 }, 0 };
Line L(const Pos& s, const Pos& e) { return Line(s, e); }
Line rotate90(const Line& l, const Pos& p) { return Line(~l.s, p); }
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
ld area(const Polygon& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) ret += H[i] / H[(i + 1) % sz];
	return ret;
}
bool norm(Polygon& H) {
	ld a = area(H);
	if (zero(a)) return 1;
	if (a < 0) std::reverse(H.begin(), H.end());
	return 0;
}
Polygon rotate_and_norm(Polygon B, const int& j0, const Polygon& A, const int& i0, const ld& t, Pos& v) {
	int sz = B.size();
	for (int j = 0; j < sz; j++) B[j] = B[j].rot(t);
	v = A[i0] - B[j0];
	for (int j = 0; j < sz; j++) B[j] += v;
	return B;
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
} INVAL = { { 0, 0 }, -1 };
Circle enclose_circle(const Pos& u, const Pos& v) {
	Pos c = (u + v) * .5;
	return Circle(c, (c - u).mag());
}
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	Line l1 = rotate90(L(u, v), (u + v) * .5);
	Line l2 = rotate90(L(v, w), (v + w) * .5);
	if (zero(l1 / l2)) return { { 0, 0 }, -1 };
	Pos c = intersection(l1, l2);
	ld r = (c - u).mag();
	return Circle(c, r);
}
Circle minimum_enclose_circle(std::vector<Pos> P) {
	shuffle(P.begin(), P.end(), std::mt19937(0x14004));
	Circle mec = INVAL;
	int sz = P.size();
	for (int i = 0; i < sz; i++) {
		if (mec.r < -1 || mec < P[i]) {
			mec = Circle(P[i], 0);
			for (int j = 0; j <= i; j++) {
				if (mec < P[j]) {
					Circle ans = enclose_circle(P[i], P[j]);
					if (zero(mec.r)) { mec = ans; continue; }
					Circle l = INVAL, r = INVAL;
					for (int k = 0; k <= j; k++) {
						if (ans < P[k]) {
							int CCW = ccw(P[i], P[j], P[k]);
							Circle c = enclose_circle(P[i], P[j], P[k]);
							if (c.r < 0) continue;
							else if (CCW > 0 && (l.r < 0 || cross(P[i], P[j], c.c) > cross(P[i], P[j], l.c))) l = c;
							else if (CCW < 0 && (r.r < 0 || cross(P[i], P[j], c.c) < cross(P[i], P[j], r.c))) r = c;
						}
					}
					if (l.r < 0 && r.r < 0) mec = ans;
					else if (l.r < 0) mec = r;
					else if (r.r < 0) mec = l;
					else mec = l.r < r.r ? l : r;
				}
			}
		}
	}
	return mec;
}
ld minimum_enclose_circle(Polygon A, int i, Polygon B, int j, ld d) {
	Polygon V;
	Pos v = (A[(i + 1) % A.size()] - B[(j + 1) % B.size()]).unit() * d;
	//std::cout << v.mag() << "\n";
	for (Pos& p : B) p += v;
	for (const Pos& p : A) V.push_back(p);
	for (const Pos& p : B) V.push_back(p);
	return minimum_enclose_circle(V).r;
}
ld ternary_search(const Polygon& A, const int& i, const Polygon& B, const int& j) {
	int a = A.size(), b = B.size();
	const Pos& pa = A[(i + 1) % a], & pb = B[(j + 1) % b];
	ld s = 0, e = (pa - pb).mag(), m1, m2, r1 = 0, r2 = 0;
	//std::cout << e << "\n";
	int cnt = 70; while (cnt--) {
		m1 = (s + s + e) / 3;
		m2 = (s + e + e) / 3;
		//std::cout << r1 << " " << r2 << "\n";
		r1 = minimum_enclose_circle(A, i, B, j, m1);
		r2 = minimum_enclose_circle(A, i, B, j, m2);
		if (r1 > r2) s = m1;
		else e = m2;
	}
	return r2 * 2;
}
bool query() {
	std::cin >> N;
	if (!N) return 0;
	Polygon A(N);
	for (Pos& p : A) std::cin >> p;
	norm(A);
	std::cin >> M;
	Polygon B(M);
	for (Pos& p : B) std::cin >> p;
	norm(B);
	ld ret = INF;
	for (int i = 0; i < N; i++) {
		Pos& I0 = A[i], & I1 = A[(i + 1) % N];
		for (int j = 0; j < M; j++) {
			Pos& J0 = B[j], & J1 = B[(j + 1) % M];
			ld t = rad(J0 - J1, I1 - I0);
			Pos v;
			//std::cout << "FUCK::\n";
			Polygon B2 = rotate_and_norm(B, j, A, i, t, v);
			ret = std::min(ret, ternary_search(A, i, B2, j));
		}
	}
	//std::cout << ret << "\n";
	ans.push_back(ret);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (query());
	for (const ld& ret : ans) std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj20231

/*

40
904 -405
906 -400
395 -616
-16 -989
-10 -992
0 -992
27 -991
50 -990
103 -987
122 -985
130 -984
151 -981
163 -979
228 -966
253 -960
260 -958
312 -942
324 -938
369 -921
392 -912
408 -905
442 -888
467 -875
500 -857
524 -843
527 -841
570 -812
604 -786
618 -775
658 -742
687 -716
715 -688
731 -671
772 -623
797 -591
812 -570
843 -523
876 -463
878 -459
899 -416
40
-923 -266
-817 -489
-724 -604
-635 -711
-626 -718
-536 -780
-352 -889
-334 -897
-205 -934
102 -954
270 -920
357 -891
399 -874
494 -820
513 -809
629 -718
672 -680
756 -591
934 -213
944 -122
954 -4
955 30
939 178
918 261
894 345
865 405
776 565
709 644
478 832
382 877
305 890
202 900
-729 407
-755 393
-841 335
-932 234
-954 50
-956 18
-956 16
-946 -113
0

36
510 -524
616 -398
631 -375
675 -289
694 -250
726 -115
728 -73
732 15
732 95
704 213
690 261
653 334
592 434
497 533
452 547
289 518
66 478
-394 388
-638 339
-677 250
-713 163
-724 130
-735 48
-735 -41
-715 -164
-678 -266
-630 -386
-511 -533
-427 -599
-400 -619
-299 -672
-131 -723
-27 -738
110 -722
287 -678
426 -598
21
722 157
721 162
693 258
687 270
665 260
443 -142
381 -622
410 -618
442 -595
478 -567
485 -561
533 -513
599 -429
627 -392
663 -326
689 -271
699 -246
710 -214
722 -162
736 74
734 90
0

*/