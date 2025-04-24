#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const ld INF = 1e17;
const ld TOL = 1e-20;
const ld PI = acosl(-1);
const int LEN = 10005;
inline int sign(const ld& x) { return x <= -TOL ? -1 : x >= TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ll sq(const ll& x) { return x * x; }
inline ld sq(const ld& x) { return x * x; }
inline ld fit(const ld& x, const ld& lo, const ld& hi) { return std::min(hi, std::max(lo, x)); }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }

#define STRONG 0
#define WEAK 1
#define LO x
#define HI y

int N, M;
Vld ANS;
struct Pos {
	ld x, y;
	int i;
	Pos(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) { i = 0; }
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return *this < p || *this == p; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ld xy() const { return x * y; }
	Pos rot(const ld& the) const { return { x * cosl(the) - y * sinl(the), x * sinl(the) + y * cosl(the) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrtl(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2l(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = Pos(0, 0);
typedef std::vector<Pos> Polygon;
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
bool inner_check(const Polygon& H, const Pos& p) {
	int sz = H.size();
	for (int i = 0; i < sz; i++) if (ccw(H[i], H[(i + 1) % sz], p) < 0) return 0;
	return 1;
}
struct Seg {
	Pos s, e;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) {}
	Pos p(const ld& rt) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
ld intersection(const Seg& s1, const Seg& s2, const bool& f = STRONG) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == WEAK) return a1;
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Pos get_vec(const Polygon& P, const Polygon& V) {
	Pos ab = Pos(0, 0);//a * sin(t) + b * cos(t) + c = 0
	int sz = V.size();
	for (int i = 0; i < sz; i++) {
		if (i < sz - 1 && V[i].HI > V[i + 1].LO) {
			Pos v = P[V[i + 1].i] - P[V[i].i];
			ab += v;
		}
	}
	return ab;
}
bool query() {
	ld min = INF, max = -1;
	ld mint = 0, maxt = 0;
	std::cin >> N;
	if (!N) return 0;
	Polygon P(N); for (Pos& p : P) std::cin >> p, p.y *= -1;
	Vld T = { 0, PI };
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			Pos v = (P[j] - P[i]);
			ld l = v.mag();
			//v = v.unit();
			if (v < O) v *= -1;
			T.push_back(norm(v.rad()));
			ld t = asinl(2 / l);
			Pos v1 = v.rot(t);
			if (v1 < O) v1 *= -1;
			T.push_back(norm(v1.rad()));
			Pos v2 = v.rot(-t);
			if (v2 < O) v2 *= -1;
			T.push_back(norm(v2.rad()));
		}
	}
	std::sort(T.begin(), T.end());
	T.erase(unique(T.begin(), T.end()), T.end());
	int sz = T.size();
	for (int i = 0; i < sz - 1; i++) {
		ld s = T[i], e = T[i + 1];
		ld m = (s + e) * .5;
		Pos v = Pos(1, 0).rot(m);
		Pos h = ~v;
		Seg I = Seg(O, h);
		Polygon V;
		ld x;
		for (int k = 0; k < N; k++) {
			Pos p = P[k];
			Pos q = p + v;
			Seg K = Seg(p, q);
			x = intersection(I, K, WEAK);
			Pos r = Pos(x - 1, x + 1);
			r.i = k;
			V.push_back(r);
		}
		std::sort(V.begin(), V.end());
		Pos ab = get_vec(P, V);
		T.push_back(norm(ab.rad()));
		T.push_back(norm((~ab).rad()));
	}
	std::sort(T.begin(), T.end());
	T.erase(unique(T.begin(), T.end()), T.end());
	for (const ld& t : T) {
		Pos v = Pos(1, 0).rot(t);
		Pos h = ~v;
		Seg I = Seg(O, h);
		Polygon V;
		ld x;
		for (const Pos& p : P) {
			Pos q = p + v;
			Seg K = Seg(p, q);
			x = intersection(I, K, WEAK);
			Pos r = Pos(x - 1, x + 1);
			V.push_back(r);
		}
		std::sort(V.begin(), V.end());
		V.erase(unique(V.begin(), V.end()), V.end());
		x = 0;
		int sz = V.size();
		for (int i = 0; i < sz; i++) {
			if (i < sz - 1 && V[i].HI > V[i + 1].LO) x += (V[i + 1].LO - V[i].LO);
			else x += V[i].HI - V[i].LO;
		}
		if (min > x) { min = x; mint = std::min(norm(v.rad()), norm(v.rad() + PI)); }
		if (max < x) { max = x; maxt = std::min(norm(v.rad()), norm(v.rad() + PI)); }
	}
	std::cout << mint << "\n" << maxt << "\n";
	//ANS.push_back(mint);
	//ANS.push_back(maxt);
	return 1;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(11);
	while (query());
	//for (const ld& ans : ANS) std::cout << ans << "\n";
	//for (const ld& ans : ANS) {
	//	ld x;
	//	std::cin >> x;
	//	std::cout << ans << "\n";
	//	ld err = (ans - x) / ans;
	//	if (zero(x)) std::cout << "answer is zero and my result is " << ans << "\n";
	//	else std::cout << err << "\n";
	//	if (std::abs(err) > 1e-10) std::cout << "FUCK::\n";
	//	else std::cout << "GOOD\n";
	//}
	return;
}
int main() { solve(); return 0; }//boj4985 Secrets in Shadows



//struct Circle {
//	Pos c;
//	ll r;
//	Circle(Pos c_ = Pos(), ll r_ = 0) : c(c_), r(r_) {}
//	bool operator > (const Pos& p) const { return sign(r - (c - p).mag()) > 0; }
//	Pos p(const ld& t) const { return c + Pos(r, 0).rot(t); }
//	ld rad(const Pos& p) const { return (p - c).rad(); }
//	ld area(const ld& lo, const ld& hi) const { return (hi - lo) * r * r * .5; }
//	ld green(const ld& lo, const ld& hi) const {
//		Pos s = Pos(cos(lo), sin(lo)), e = Pos(cos(hi), sin(hi));
//		ld fan = area(lo, hi);
//		Pos m = c + (s + e) * r * (ld).5;
//		ld tz = (cos(lo) - cos(hi)) * m.y * r;
//		return fan + tz - (s / e) * r * r * (ld).5;
//	}
//	friend std::istream& operator >> (std::istream& is, Circle& p) { is >> p.c.x >> p.c.y >> p.r; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Circle& p) { os << p.c.x << " " << p.c.y << " " << p.r; return os; }
//} C[3];
//Vld intersections(const Circle& a, const Circle& b) {
//	Pos ca = a.c, cb = b.c;
//	Pos vec = cb - ca;
//	ld ra = a.r, rb = b.r;
//	ld distance = vec.mag();
//	ld rd = vec.rad();
//	if (vec.Euc() > sq(ra + rb) + TOL) return {};
//	if (vec.Euc() < sq(ra - rb) - TOL) return {};
//	ld X = (ra * ra - rb * rb + vec.Euc()) / (2 * distance * ra);
//	if (X < -1) X = -1;
//	if (X > 1) X = 1;
//	ld h = acos(X);
//	Vld ret = {};
//	ret.push_back(norm(rd - h));
//	if (zero(h)) return ret;
//	ret.push_back(norm(rd + h));
//	return ret;
//}