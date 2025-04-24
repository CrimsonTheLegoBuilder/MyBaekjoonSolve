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

/*

#include <array>     // array
#include <iostream>  // input, output
#include <cmath>     // atan2l, asin, sin, cos...
#include <vector>    // vector
#include <algorithm> // min, max, sort..
#include <complex>   // complex no.
#include <iomanip>   // setprecision
//#include <bits/stdc++.h>
using namespace std;
using lint = long long;
//using pi = array<lint, 2>;  // pair ll * 2
#define sz(a) ((int)(a).size())
#define all(a) (a).begin(), (a).end()

// Returns the convex hull of circles in O(n log n)
// Arcs are in the form (f, lo, hi) where (f.a, f.b) denotes the center, f.r denotes the radius, and [lo, hi] denotes the radian range.
// Line segments are implicitly expressed as two endpoint (i, hi) - (i + 1, lo).
// Code inspired by https://codeforces.com/gym/104334/submission/204263938
// Thanks koosaga
// this code is awesome...
namespace CircleHull {
	using func_t = long long;
	using real_t = long double;
	const real_t eps = 1e-9;     // tolerance
	const real_t pi = acos(-1);  // pi = 3.14159265...

	struct func {
		// a: y coord ; b: x coord ; r : r of circle
		// f(x) = a*sin(x) + b*cos(x) + r
		func_t a, b, r;
		constexpr func operator+(func f) const { return { a + f.a, b + f.b, r + f.r }; }
		constexpr func operator-(func f) const { return { a - f.a, b - f.b, r - f.r }; }
		bool operator!=(func f) const { return abs(a - f.a) >= eps || abs(b - f.b) >= eps || abs(r - f.r) >= eps; }
		real_t eval(real_t x) { return sin(x) * a + cos(x) * b + r; }
	};

	struct arc {
		real_t lo, hi;  // start, end angle of edge (or bridge)
		func f;
	};

	real_t norm(real_t x) {  // angle normalization
		while (x < 0)
			x += pi * 2;
		while (x >= pi * 2)
			x -= pi * 2;
		return x;
	};

	vector<arc> get_max(func p, func q, real_t l, real_t r) {
		//std::cout << p.r << " " << "\n";
		if (l >= r)  // judge by slope
			return vector<arc>();
		func delta = p - q;
		if (abs(delta.a) < eps && abs(delta.b) < eps) {  //  if 2 circle's center is same
			if (delta.r >= 0)
				return { {l, r, p} };
			else
				return { {l, r, q} };
		}
		real_t t = -real_t(delta.r) / hypot(delta.a, delta.b);  // The angle between the two centers and their radii diff'
		if (t >= 1) return { {l, r, q} };   // if q.r > p.r:
		if (t <= -1) return { {l, r, p} };  // if p.r < q.r:

		real_t phi = atan2l(delta.b, delta.a);  // absolute angle between 2 centers
		real_t x1 = asin(abs(t)) + (t < 0 ? pi : 0);  // relative angle of tangent
		//real_t x12 = asin(abs(t)) * 180.0 / pi;
		//std::cout << phi << " " << "\n";
		//std::cout << x1 << " " << x12 << "\n";
		real_t x2 = pi - x1;
		//std::cout << x2 << " " << "\n";
		x1 = norm(x1 - phi);
		x2 = norm(x2 - phi);
		//std::cout << x1 << " " << x2 << "\n";
		if (x1 > x2)
			swap(x1, x2);

		vector<arc> vec;
		if (p.eval((x1 + x2) / 2) < q.eval((x1 + x2) / 2)) {  //
			vec = { {real_t(0), x1, p}, {x1, x2, q}, {x2, pi * 2, p} };  // CW?
		}
		else {
			//std::cout << p.eval((x1 + x2) / 2) << " " << q.eval((x1 + x2) / 2) << "\n";
			vec = { {real_t(0), x1, q}, {x1, x2, p}, {x2, pi * 2, q} };  // CW?
		}
		//std::cout << x1 << " " << x2 << "\n";

		vector<arc> ret;
		for (auto [lo, hi, f] : vec) {
			lo = max(lo, l);
			hi = min(hi, r);
			if (lo >= hi)  // if CW : continue (break)
				continue;
			ret.push_back({ lo, hi, f });
		}
		return ret;
	}

	vector<arc> merge(vector<arc>& v1, vector<arc>& v2) {
		int i = 0, j = 0;
		real_t cur = 0;
		vector<arc> ret;
		while (i < sz(v1) && j < sz(v2)) {
			if (v1[i].hi < v2[j].hi) {
				auto tmp = get_max(v1[i].f, v2[j].f, cur, v1[i].hi);
				cur = v1[i++].hi;
				for (auto x : tmp)
					ret.push_back(x);
			}
			else {
				auto tmp = get_max(v1[i].f, v2[j].f, cur, v2[j].hi);
				cur = v2[j++].hi;
				for (auto x : tmp)
					ret.push_back(x);
			}
		}
		vector<arc> r;
		cur = 0;
		for (int i = 0; i < sz(ret); i++) {
			if (i + 1 == sz(ret) || ret[i + 1].f != ret[i].f) {  //angle update
				r.push_back({ cur, ret[i].hi, ret[i].f });       //anlges are continue
				cur = ret[i].hi;
			}
		}
		return r;
	}

	vector<arc> solve(int l, int r, vector<func>& f) {
		if (l == r)
			return { {0, 2 * pi, f[l]} };
		int m = (l + r) / 2;
		auto vl = solve(l, m, f);
		auto vr = solve(m + 1, r, f);
		return merge(vl, vr);
	}

	vector<arc> solve(vector<func> f) {
		for (auto& u : f)
			swap(u.a, u.b);
		auto g = solve(0, sz(f) - 1, f);
		for (auto& u : g) {
			swap(u.f.a, u.f.b);
		}
		return g;
	}
	real_t getArea(vector<func> f) {
		auto hull = solve(f);
		using point = complex<real_t>;
		vector<point> v;
		real_t ans = 0;
		for (auto& [lo, hi, f] : hull) {
			ans += 1.0 * f.r * f.r * (hi - lo);
			ans += f.r * (f.a * (sin(hi) - sin(lo)) - f.b * (cos(hi) - cos(lo)));
			point x{ 1.0 * f.a, 1.0 * f.b };
			point r{ 1.0 * f.r, 0.0 };
			v.push_back(x + r * exp(point(0, lo)));
			v.push_back(x + r * exp(point(0, hi)));
		}
		for (int i = 0; i < sz(v); i += 2) {
			ans += (conj(v[(i + sz(v) - 1) % sz(v)]) * v[i]).imag();
		}
		return ans / 2;
	}
	real_t getPerimeter(vector<func> f) {
		auto hull = solve(f);
		using point = complex<real_t>;
		vector<point> v;
		real_t ans = 0;
		for (auto& [lo, hi, f] : hull) {
			ans += f.r * (hi - lo);
			point x{ 1.0 * f.a, 1.0 * f.b };
			point r{ 1.0 * f.r, 0.0 };
			v.push_back(x + r * exp(point(0, lo)));
			v.push_back(x + r * exp(point(0, hi)));
		}
		for (int i = 0; i < sz(v); i += 2) {
			ans += abs(v[(i + sz(v) - 1) % sz(v)] - v[i]);
		}
		return ans;
	}

} // namespace CircleHull

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int n;
	cin >> n;
	vector<CircleHull::func> a(n);
	for (auto& x : a) {
		lint xa, xb, xr;
		cin >> xa >> xb >> xr;
		x.a = xa;
		x.b = xb;
		x.r = xr;
	}
	cout << setprecision(69) << CircleHull::getPerimeter(a) << "\n";
	return 0;
}

*/


/*

#include <cmath>
#include <vector>
#include <iomanip>
#include <complex>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;   // tolerance
const ld PI = acos(-1);// pi = 3.14159265...
// Returns the convex hull of circles in O(N log N)
// Arcs are in the form (lo, hi, c) where (c.x, c.y) denotes the center, c.r denotes the radius, and [lo, hi] denotes the radian range.
// Code inspired by https://codeforces.com/gym/104334/submission/204263938
// refer to koosaga'

struct circle {
	ll x, y, r;// [x, y] - center, r - radius
	constexpr circle operator+(const circle& c) const { return { x + c.x, y + c.y, r + c.r }; }
	constexpr circle operator-(const circle& c) const { return { x - c.x, y - c.y, r - c.r }; }
	bool operator!=(circle c) const { return std::abs(x - c.x) >= TOL || std::abs(y - c.y) >= TOL || std::abs(r - c.r) >= TOL; }
	ld H(const ld& th) const { return sin(th) * x + cos(th) * y + r; }// coord trans | check right
};
struct arc {
	ld lo, hi;// [lo, hi] - radian range of arc
	circle c; // c.r - radius of arc
};


ld norm(ld& th) {// angle normalization
	while (th < 0) th += 2 * PI;
	while (th >= 2 * PI) th -= 2 * PI;
	return th;
}
std::vector<arc> merge(circle p, circle q, ld cur, ld nxt) {// merge 2 disks
	if (cur >= nxt) return std::vector<arc>();// ignore when next disk is inside the hull

	circle delta = p - q;// difference between p and q : delta.r > 0 when p.r > q.r
	if (std::abs(delta.x) < TOL && std::abs(delta.y) < TOL) {
		if (delta.r >= 0) return { { cur, nxt, p } };// same center :: p.r > q.r
		else return { { cur, nxt, q } };             // same center :: p.r < q.r
	}

	ld t = -ld(delta.r) / hypot(delta.x, delta.y);// angle between tangent and line segment between centers && size comparison
	if (t <= -1) return { { cur, nxt, p } };      // p > q
	if (t >= 1) return { { cur, nxt, q } };       // p < q

	ld phi = atan2l(delta.y, delta.x);// angle between x-line and line segment between centers
	ld x1 = asin(std::abs(t)) + (t < 0 ? PI : 0);// normalizated angle between tangent and line segment between centers
	ld x2 = PI - x1;
	x1 = norm(x1 - phi);// absolute angle of tangent 1
	x2 = norm(x2 - phi);// absolute angle of tangent 2
	if (x1 > x2) std::swap(x1, x2);

	std::vector<arc> h2d;
	if (p.H((x1 + x2) / 2) < q.H((x1 + x2) / 2)) // p is to the right of q
		h2d = { {ld(0), x1, p}, {x1, x2, q}, {x2, 2 * PI, p} };// when the HullDisks starts at p
	else // p is to the left of q
		h2d = { {ld(0), x1, q}, {x1, x2, p}, {x2, 2 * PI, q} };// when the HullDisks starts at q

	std::vector<arc> hull;
	for (auto [lo, hi, c] : h2d) {// ignore all arc when it out of angle cur - nxt
		lo = std::max(lo, cur);
		hi = std::min(hi, nxt);
		if (lo >= hi) continue;
		hull.push_back({ lo, hi, c });
	}
	return hull;
}
std::vector<arc> merge(std::vector<arc>& H1, std::vector<arc>& H2) {// merge hull H1, H2
	int i = 0, j = 0;
	ld cur = 0;// merge continues until "cur" completes one revolution
	std::vector<arc> tmp;
	while (i < H1.size() && j < H2.size()) {
		if (H1[i].hi < H2[j].hi) {// tangent of H1[i] is superior to H2[j]'s
			std::vector<arc> h2d = merge(H1[i].c, H2[j].c, cur, H1[i].hi);
			cur = H1[i++].hi;// hand over the initiative to next disk
			for (const arc& x : h2d) tmp.push_back(x);
		}
		else {
			std::vector<arc> h2d = merge(H1[i].c, H2[j].c, cur, H2[j].hi);
			cur = H2[j++].hi;
			for (const arc& x : h2d) tmp.push_back(x);
		}
	}

	std::vector<arc> hull;
	cur = 0;
	for (int i = 0; i < tmp.size(); i++) {
		if (i + 1 == tmp.size() || tmp[i + 1].c != tmp[i].c) {
			hull.push_back({ cur, tmp[i].hi, tmp[i].c });// complete one revolution
			cur = tmp[i].hi;
		}
	}
	return hull;
}
std::vector<arc> solve(int l, int r, std::vector<circle>& C) {// divide and conquer
	if (l == r) return { {0, 2 * PI, C[l]} };
	int m = l + r >> 1;
	std::vector<arc> HL = solve(l, m, C);
	std::vector<arc> HR = solve(m + 1, r, C);
	return merge(HL, HR);
}
std::vector<arc> HullDisks(std::vector<circle>& C) {
	for (auto& c : C) std::swap(c.x, c.y);// swap x - y coord for convenience of angle calculation
	std::vector<arc> hull = solve(0, C.size() - 1, C);
	for (auto& A : hull) std::swap(A.c.x, A.c.y);// set back
	return hull;
}
ld getRound(std::vector<arc>& H) {// hull = [[lo, hi, c], [lo, hi, c], [lo, hi, c]...]
	ld  R = 0;
	for (int i = 0; i < H.size(); i++) {
		arc  p = H[i], q = H[(i + 1) % H.size()];
		circle d = p.c - q.c;
		R += p.c.r * (p.hi - p.lo);// calculate arc / r * theta
		R += sqrt((d.x * d.x + d.y * d.y) - d.r * d.r);// calculate bridge / sqrt(a^2 - c^2) = b
	}
	return R;
}
ld getArea(std::vector<arc>& H) {
	using point = complex<ld>;
	std::vector<point> v;
	ld A = 0;
	for (auto& [lo, hi, c] : H) {
		A += 1.0 * c.r * c.r * (hi - lo);
		A += c.r * (c.x * (sin(hi) - sin(lo)) - c.y * (cos(hi) - cos(lo)));
		point x{ 1.0 * c.x, 1.0 * c.y };
		point r{ 1.0 * c.r, 0.0 };
		v.push_back(x + r * exp(point(0, lo)));
		v.push_back(x + r * exp(point(0, hi)));
	}
	for (int i = 0; i < v.size(); i += 2) {
		A += (conj(v[(i + v.size() - 1) % v.size()]) * v[i]).imag();
	}
	return A * .5;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	int n;
	std::cin >> n;
	ll x, y, r;
	ld R;//, A;
	std::vector<circle> Circles(n);
	for (int i = 0; i < n; i++) {
		std::cin >> x >> y >> r;
		Circles[i].x = x;
		Circles[i].y = y;
		Circles[i].r = r;
	}
	std::vector<arc> Hull = HullDisks(Circles);
	R = getRound(Hull);
	//A = getArea(Hull);
	std::cout << R << "\n";
	//std::cout << A << "\n";

	return 0;
}

*/


/*

#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <bits/stdc++.h>

#define MAX 50005
#define INF (1e9)
#define DEBUG 0

#define all(v) v.begin(), v.end()
#define compress(v) sort(all(v)), v.erase(unique(all(v)), v.end())

using namespace std;
typedef long long ll;
typedef double ld;
typedef pair<ll, ll> pll;
typedef pair<ld, ld> pdd;
typedef complex<long double> cpx;

inline int readChar();
template<class T = int> inline T readInt();
template<class T> inline void writeInt(T x, char end = 0);
inline void writeChar(int x);
inline void writeWord(const char* s);
static const int buf_size = 1 << 18;
inline int getChar() {
#ifndef LOCAL
	static char buf[buf_size];
	static int len = 0, pos = 0;
	if (pos == len) pos = 0, len = fread(buf, 1, buf_size, stdin);
	if (pos == len) return -1;
	return buf[pos++];
#endif
}
inline int readChar() {
#ifndef LOCAL
	int c = getChar();
	while (c <= 32) c = getChar();
	return c;
#else
	char c; cin >> c; return c;
#endif
}
template <class T>
inline T readInt() {
#ifndef LOCAL
	int s = 1, c = readChar();
	T x = 0;
	if (c == '-') s = -1, c = getChar();
	while ('0' <= c && c <= '9') x = x * 10 + c - '0', c = getChar();
	return s == 1 ? x : -x;
#else
	T x; cin >> x; return x;
#endif
}
static int write_pos = 0;
static char write_buf[buf_size];
inline void writeChar(int x) {
	if (write_pos == buf_size) fwrite(write_buf, 1, buf_size, stdout), write_pos = 0;
	write_buf[write_pos++] = x;
}
template <class T>
inline void writeInt(T x, char end) {
	if (x < 0) writeChar('-'), x = -x;
	char s[24]; int n = 0;
	while (x || !n) s[n++] = '0' + x % 10, x /= 10;
	while (n--) writeChar(s[n]);
	if (end) writeChar(end);
}
inline void writeWord(const char* s) {
	while (*s) writeChar(*s++);
}
struct Flusher {
	~Flusher() { if (write_pos) fwrite(write_buf, 1, write_pos, stdout), write_pos = 0; }
}flusher;

const ld PI = acos(-1);
struct Circle {
	ld x, y, r, i;
	bool operator < (const Circle& c) const {
		if (x - r != c.x - c.r) return x - r < c.x - c.r;
		if (x != c.x) return x < c.x;
		if (y != c.y) return y < c.y;
		return r > c.r;
	}
	bool operator == (const Circle& c) const {
		return x == c.x && y == c.y && r == c.r;
	}
};

vector<Circle> circle;
vector<ll> cvx[MAX << 2];

struct Line {
	ld x, y, theta;
};
vector<Line> th[MAX << 2];
ld Norm(ld theta) {
	if (abs(theta) <= 1e-10) return theta;
	if (theta < 0) theta += 2 * PI;
	if (theta > 2 * PI) theta -= 2 * PI;
	return theta;
}

inline ld alpha(ld v1, Line v2) {
	if (v2.theta >= INF) return v2.theta;
	ld ret = Norm(v1 - v2.theta);
	return ret;
}

inline ld dist(pll p1, pll p2) {
	return (ld)(p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

Line tangent(ld theta, Circle c) {
	ld t = Norm(theta + PI / 2);
	Line ret = { (ld)c.x + c.r * cos(t), (ld)c.y + c.r * sin(t), theta };
	return ret;
}

Line tangent(Circle c1, Circle c2) {
	if (c1 == c2) return { c1.x - c1.r, c1.y, INF };
	ld d = (c2.x - c1.x) * (c2.x - c1.x) + (c2.y - c1.y) * (c2.y - c1.y);

	if (d - (c1.r - c2.r) * (c1.r - c2.r) < 1e-10) return { c1.x - c1.r, c1.y, INF + 1 };

	ld d2 = sqrt(d - (c1.r - c2.r) * (c1.r - c2.r));
	ld theta = atan2(c2.y - c1.y, c2.x - c1.x);

	if (c1.r <= c2.r) theta += atan2(abs(c1.r - c2.r), d2);
	else theta -= atan2(abs(c1.r - c2.r), d2);
	return tangent(Norm(theta), c1);
}

inline ld ccw(pdd a, pdd b) {
	return a.first * b.second - a.second * b.first;
}

bool isLeft(Line l1, Line l2) {
	return ccw({ cos(l1.theta), sin(l1.theta) }, { l2.x - l1.x, l2.y - l1.y }) <= 0;
}

void add(ll n, ll x) {
	if (cvx[n].empty() || cvx[n].back() != x) cvx[n].push_back(x);
}

void Advance(ld& l, ll& p, ll& q, ll le, ll ri, ll n) {
	vector<ll>& left = cvx[le];
	vector<ll>& right = cvx[ri];
	ld a1 = alpha(l, tangent(circle[left[p]], circle[right[q]]));
	ld a2 = alpha(l, th[le][p]);
	ld a3 = alpha(l, th[ri][q]);
	ld a4 = alpha(l, tangent(circle[right[q]], circle[left[p]]));

#if DEBUG
	cout << "NEW : " << ' ' << left[p] << ' ' << right[q] << endl;
	double t1 = tangent(circle[left[p]], circle[right[q]]).theta;
	double t2 = left.size() == 1 ? 1e16 : tangent(circle[left[p]], circle[left[(p + 1) % left.size()]]).theta;
	double t3 = right.size() == 1 ? 1e16 : tangent(circle[right[q]], circle[right[(q + 1) % right.size()]]).theta;
	double t4 = tangent(circle[right[q]], circle[left[p]]).theta;
	cout << tan(t1) << ' ' << tan(t2) << ' ' << tan(t3) << ' ' << tan(t4) << endl;
	cout << t1 * 180 / PI << ' ' << t2 * 180 / PI << ' ' << t3 * 180 / PI << ' ' << t4 * 180 / PI << endl;
	cout << a1 * 180 / PI << ' ' << a2 * 180 / PI << ' ' << a3 * 180 / PI << ' ' << a4 * 180 / PI << endl;
#endif

	if (a1 < a2 && a1 < a3) { add(n, right[q]); }
	if (a4 < a2 && a4 < a3) { add(n, left[p]); }

	if (a2 < a3) {
		l = th[le][p].theta;
		p = (p + 1) % left.size();
	}
	else {
		l = th[ri][q].theta;
		q = (q + 1) % right.size();
	}
}

void Merge(ll s, ll e, ll n) {
	vector<ll>& ret = cvx[n];
	if (s == e) { ret.push_back(s); th[n].push_back({ circle[s].x - circle[s].r, circle[s].y, INF }); return; }

	ll m = s + e >> 1; Merge(s, m, n << 1); Merge(m + 1, e, n << 1 | 1);
#if DEBUG
	cout << endl << s << ' ' << e << endl;
#endif
	ll p = 0, q = 0;
	ld theta = 1.5707963265;
	for (ll i = 0; i < 2 * (e - s + 1) - 1; ++i) {
		if (theta >= INF) break;
		theta = Norm(theta);
		Line lp = tangent(theta, circle[cvx[n << 1][p]]);
		Line lq = tangent(theta, circle[cvx[n << 1 | 1][q]]);

#if DEBUG
		cout << theta / PI * 180 << ' ' << tan(theta) << endl;
		cout << circle[left[p]].x << ' ' << circle[left[p]].y << ' ' << circle[left[p]].r << ' ' << lp.x << ' ' << lp.y << ' ' << lp.theta / PI * 180 << endl;
		cout << circle[right[q]].x << ' ' << circle[right[q]].y << ' ' << circle[right[q]].r << ' ' << lq.x << ' ' << lq.y << ' ' << lq.theta / PI * 180 << endl;
#endif

		//        if(dist({circle[left[p]].x, circle[left[p]].y}, {circle[right[q]].x, circle[right[q]].y}) <= circle[right[q]].r - circle[left[p]].r) {
		//            add(ret, right[q]);
		//            p = (p + 1) % left.size();
		//            continue;
		//        }
		//
		//        if(dist({circle[left[p]].x, circle[left[p]].y}, {circle[right[q]].x, circle[right[q]].y}) <= circle[left[p]].r - circle[right[q]].r) {
		//            add(ret, left[p]);
		//            q = (q + 1) % right.size();
		//            continue;
		//        }

		if (isLeft(lp, lq)) {
			add(n, cvx[n << 1][p]);
			if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
			Advance(theta, p, q, n << 1, n << 1 | 1, n);
		}
		else {
			add(n, cvx[n << 1 | 1][q]);
			if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
			Advance(theta, q, p, n << 1 | 1, n << 1, n);
		}
#if DEBUG
		cout << "CONVEX HULL : ";
		for (auto v : ret) cout << v << ' ';
		cout << endl;
#endif

		if (ret.size() >= 3 && ret[0] == ret[ret.size() - 2] && ret[1] == ret.back()) { ret.pop_back(); break; }
	}
	if (ret.size() >= 2 && ret[0] == ret.back()) ret.pop_back();
	th[n].resize(ret.size());
	for (ll i = 0; i < (ll)ret.size(); ++i) th[n][i] = tangent(circle[ret[i]], circle[ret[(i + 1) % ret.size()]]);

#if DEBUG
	cout << "CONVEX HULL : ";
	for (auto v : ret) cout << v << ' ';
	cout << endl;
#endif // DEBUG

	return;
}

ll N, T;
int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	N = readInt();
	circle.resize(N);
	for (ll i = 0; i < N; ++i) {
		ll x, y, z;
		x = readInt(); y = readInt(); z = readInt();
		circle[i].x = x; circle[i].y = y; circle[i].r = z;
		circle[i].i = i;
	}
	//for(ll i = 0; i < N; ++i) circle[i].r += 10;
	//compress(circle);

	Merge(0, circle.size() - 1, 1);
	vector<ll>& ans = cvx[1];

#if 0
	for (ll v : ans) cout << "Circles" << circle[v].x << ' ' << circle[v].y << ' ' << circle[v].r << '\n';
#endif // DEBUG

#if 0
	for (ll v : ans) cout << circle[v].i << ' ';
	cout << endl;
#endif // 1

	cout.precision(10); cout << fixed;
	if (ans.size() == 1) {
		cout << 2 * PI * circle[ans[0]].r;
		return 0;
	}

	ld sum = 0;
	for (ll i = 0; i < (ll)ans.size(); ++i) {
		ll r = ans[(i + 1) % ans.size()];
		ld theta = Norm(th[1][(i + ans.size() - 1) % ans.size()].theta - th[1][i].theta);
		sum += circle[ans[i]].r * theta;

		ld d = dist({ circle[ans[i]].x, circle[ans[i]].y }, { circle[r].x, circle[r].y });
		sum += sqrt(d - (circle[ans[i]].r - circle[r].r) * (circle[ans[i]].r - circle[r].r));
	}
	cout << sum;
	return 0;
}

*/