#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <complex>
using namespace std;
typedef long long ll;
typedef double ld;
//typedef double ld;
const ld TOL = 1e-9;   // tolerance
const ld PI = acos(-1);// pi = 3.14159265...
// Returns the convex hull of circles in O(N log N)
// Arcs are in the form (lo, hi, c) where (c.x, c.y) denotes the center, c.r denotes the radius, and [lo, hi] denotes the radian range.
// Code inspired by https://codeforces.com/gym/104334/submission/204263938
// refer to koosaga'

int N;
bool zero(const ld& x) { return std::abs(x) < TOL; }
//struct Pos {
//	ld x, y;
//	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
//	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
//	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
//	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
//	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//	Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
//	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
//	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
//	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
//	Pos operator ~ () const { return { -y, x }; }
//	ld operator ! () const { return x * y; }
//	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
//	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
//	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
//	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
//	ld Euc() const { return x * x + y * y; }
//	ld mag() const { return hypot(x, y); }
//	Pos unit() const { return *this / mag(); }
//	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
//}; const Pos O = { 0, 0 };
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
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
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }// coord trans | check right
	ld A() const { return r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c.x >> c.c.y >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c.x << " " << c.c.y << " " << c.r; return os; }
};
struct Arc {
	ld lo, hi;// [lo, hi] - radian range of arc
	Circle c; // c.r - radius of arc
};
ld norm(ld th) {// angle normalization
	while (th < 0) th += 2 * PI;
	while (th >= 2 * PI) th -= 2 * PI;
	return th;
}
std::vector<Arc> merge(const Circle& p, const Circle& q, const ld& cur, const ld& nxt) {// merge 2 disks
	if (cur >= nxt) return std::vector<Arc>();// ignore when next disk is inside the hull

	Circle delta = p - q;// difference between p and q : delta.r > 0 when p.r > q.r
	//if (std::abs(delta.c.x) < TOL && std::abs(delta.c.y) < TOL) {
	if (delta.c == O) {
		if (delta.r >= 0) return { { cur, nxt, p } };// same center :: p.r > q.r  
		else return { { cur, nxt, q } };             // same center :: p.r < q.r
	}

	ld t = -ld(delta.r) / delta.c.mag();// angle between tangent and line segment between centers && size comparison
	if (t <= -1) return { { cur, nxt, p } };      // p > q
	if (t >= 1) return { { cur, nxt, q } };       // p < q

	ld phi = atan2l(delta.c.y, delta.c.x);// angle between x-line and line segment between centers
	ld x1 = asin(std::abs(t)) + (t < 0 ? PI : 0);// normalizated angle between tangent and line segment between centers
	ld x2 = PI - x1;
	x1 = norm(x1 - phi);// absolute angle of tangent 1
	x2 = norm(x2 - phi);// absolute angle of tangent 2
	if (x1 > x2) std::swap(x1, x2);

	std::vector<Arc> h2d;
	if (p.H((x1 + x2) / 2) < q.H((x1 + x2) / 2)) // p is to the right of q
		h2d = { {ld(0), x1, p}, {x1, x2, q}, {x2, 2 * PI, p} };// when the HullDisks starts at p
	else // p is to the left of q
		h2d = { {ld(0), x1, q}, {x1, x2, p}, {x2, 2 * PI, q} };// when the HullDisks starts at q

	std::vector<Arc> hull;
	for (auto [lo, hi, c] : h2d) {// ignore all arc when it out of angle cur - nxt
		lo = std::max(lo, cur);
		hi = std::min(hi, nxt);
		if (lo >= hi) continue;
		hull.push_back({ lo, hi, c });
	}
	return hull;
}
std::vector<Arc> merge(const std::vector<Arc>& H1, const std::vector<Arc>& H2) {// merge hull H1, H2
	int i = 0, j = 0;
	ld cur = 0;// merge continues until "cur" completes one revolution
	std::vector<Arc> tmp;
	while (i < H1.size() && j < H2.size()) {
		if (H1[i].hi < H2[j].hi) {// tangent of H1[i] is superior to H2[j]'s
			std::vector<Arc> h2d = merge(H1[i].c, H2[j].c, cur, H1[i].hi);
			cur = H1[i++].hi;// hand over the initiative to next disk
			for (const Arc& x : h2d) tmp.push_back(x);
		}
		else {
			std::vector<Arc> h2d = merge(H1[i].c, H2[j].c, cur, H2[j].hi);
			cur = H2[j++].hi;
			for (const Arc& x : h2d) tmp.push_back(x);
		}
	}

	std::vector<Arc> hull;
	cur = 0;
	for (int i = 0; i < tmp.size(); i++) {
		if (i + 1 == tmp.size() || tmp[i + 1].c != tmp[i].c) {
			hull.push_back({ cur, tmp[i].hi, tmp[i].c });// complete one revolution
			cur = tmp[i].hi;
		}
	}
	return hull;
}
std::vector<Arc> solve(int l, int r, std::vector<Circle>& C) {// divide and conquer
	if (l == r) return { {0, 2 * PI, C[l]} };
	int m = l + r >> 1;
	std::vector<Arc> HL = solve(l, m, C);
	std::vector<Arc> HR = solve(m + 1, r, C);
	return merge(HL, HR);
}
std::vector<Arc> HullDisks(std::vector<Circle>& C) {
	for (auto& c : C) std::swap(c.c.x, c.c.y);// swap x - y coord for convenience of angle calculation
	std::vector<Arc> hull = solve(0, C.size() - 1, C);
	for (auto& A : hull) std::swap(A.c.c.x, A.c.c.y);// set back
	return hull;
}
ld get_round(const std::vector<Arc>& H) {// hull = [[lo, hi, c], [lo, hi, c], [lo, hi, c]...]
	ld R = 0;
	for (int i = 0; i < H.size(); i++) {
		Arc p = H[i], q = H[(i + 1) % H.size()];
		Circle d = p.c - q.c;
		R += p.c.r * (p.hi - p.lo);// calculate arc / r * theta
		R += sqrt(d.c.Euc() - d.r * d.r);// calculate bridge / sqrt(a^2 - c^2) = b
	}
	return R;
}
ld get_area(std::vector<Arc>& H) {
	using point = complex<ld>;
	std::vector<point> v;
	ld A = 0;
	for (auto& [lo, hi, c] : H) {
		A += 1.0 * c.r * c.r * (hi - lo);
		A += c.r * (c.c.x * (sin(hi) - sin(lo)) - c.c.y * (cos(hi) - cos(lo)));
		point x{ 1.0 * c.c.x, 1.0 * c.c.y };
		point r{ 1.0 * c.r, 0.0 };
		v.push_back(x + r * exp(point(0, lo)));
		v.push_back(x + r * exp(point(0, hi)));
	}
	for (int i = 0; i < v.size(); i += 2) {
		A += (conj(v[(i + v.size() - 1) % v.size()]) * v[i]).imag();
	}
	return A * .5;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	ld R;//, A;
	std::vector<Circle> C(N);
	for (int i = 0; i < N; i++) std::cin >> C[i];
	std::vector<Arc> Hull = HullDisks(C);
	R = get_round(Hull);
	//A = get_area(Hull);
	std::cout << R << "\n";
	//std::cout << A << "\n";
	return;
}
int main() { solve(); return 0; }//boj27957