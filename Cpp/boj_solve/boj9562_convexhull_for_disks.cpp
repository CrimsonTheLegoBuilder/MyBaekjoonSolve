#include <cmath>
#include <vector>
#include <iomanip>
#include <complex>
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;     // tolerance
const ld PI = acos(-1);  // pi = 3.14159265...
// Returns the convex hull of circles in O(N log N)
// Arcs are in the form (lo, hi, c) where (c.x, c.y) denotes the center, c.r denotes the radius, and [lo, hi] denotes the radian range.
// Code inspired by https://codeforces.com/gym/104334/submission/204263938


struct circle {
	ll x, y, r;  // [x, y] - center, r - radius
	constexpr circle operator+(circle c) const { return { x + c.x, y + c.y, r + c.r }; }
	constexpr circle operator-(circle c) const { return { x - c.x, y - c.y, r - c.r }; }
	bool operator!=(circle c) const { return std::abs(x - c.x) >= TOL || std::abs(y - c.y) >= TOL || std::abs(r - c.r) >= TOL; }
	ld H(ld th) { return sin(th) * x + cos(th) * y + r; }  // coord - transformation
};
struct arc {
	ld lo, hi;  // [lo, hi] - radian range of arc
	circle c;   // c.r - radius of arc
};


ld norm(ld th) {  // angle normalization
	while (th < 0) th += 2 * PI;
	while (th >= 2 * PI) th -= 2 * PI;
	return th;
}
std::vector<arc> merge(circle p, circle q, ld cur, ld nxt) {  // merge 2 disks
	if (cur >= nxt) return std::vector<arc>();  // ignore when next disk is inside the hull

	circle delta = p - q;  // difference between p and q : delta.r > 0 when p.r > q.r
	if (std::abs(delta.x) < TOL && std::abs(delta.y) < TOL) {
		if (delta.r >= 0) return { { cur, nxt, p } };  // same center :: p.r > q.r  
		else return { { cur, nxt, q } };               // same center :: p.r < q.r
	}

	ld t = -ld(delta.r) / hypot(delta.x, delta.y);  // angle between tangent and line segment between centers
	if (t <= -1) return { { cur, nxt, p } };        // p.r > q.r
	if (t >= 1) return { { cur, nxt, q } };         // p.r < q.r

	ld phi = atan2l(delta.y, delta.x);  // angle between x-line and line segment between centers
	ld x1 = asin(std::abs(t)) + (t < 0 ? PI : 0);  // normalizated angle between tangent and line segment between centers
	ld x2 = PI - x1;
	x1 = norm(x1 - phi);  // absolute angle of tangent 1
	x2 = norm(x2 - phi);  // absolute angle of tangent 2
	if (x1 > x2) std::swap(x1, x2);

	std::vector<arc> h2d;
	if (p.H((x1 + x2) / 2) < q.H((x1 + x2) / 2)) {  // p is to the right of q
		h2d = { {ld(0), x1, p}, {x1, x2, q}, {x2, 2 * PI, p} };  // when the HullDisks starts at p
	}
	else {  // p is to the left of q
		h2d = { {ld(0), x1, q}, {x1, x2, p}, {x2, 2 * PI, q} };  // when the HullDisks starts at q
	}

	std::vector<arc> hull;
	for (auto [lo, hi, c] : h2d) {  // ignore all arc when it out of angle cur - nxt
		lo = std::max(lo, cur);
		hi = std::min(hi, nxt);
		if (lo >= hi) continue;
		hull.push_back({ lo, hi, c });
	}
	return hull;
}
std::vector<arc> merge(std::vector<arc>& H1, std::vector<arc>& H2) {  // merge hull H1, H2
	int i = 0, j = 0;
	ld cur = 0;  // merge continues until "cur" completes one revolution
	std::vector<arc> tmp;
	while (i < H1.size() && j < H2.size()) {
		if (H1[i].hi < H2[j].hi) {  // tangent of H1[i] is superior to H2[j]'s
			std::vector<arc> h2d = merge(H1[i].c, H2[j].c, cur, H1[i].hi);
			cur = H1[i++].hi;  // hand over the initiative to next disk
			for (auto x : h2d) tmp.push_back(x);
		}
		else {
			std::vector<arc> h2d = merge(H1[i].c, H2[j].c, cur, H2[j].hi);
			cur = H2[j++].hi;
			for (auto x : h2d) tmp.push_back(x);
		}
	}

	std::vector<arc> hull;
	cur = 0;
	for (int i = 0; i < tmp.size(); i++) {
		if (i + 1 == tmp.size() || tmp[i + 1].c != tmp[i].c) {
			hull.push_back({ cur, tmp[i].hi, tmp[i].c });  // complete one revolution
			cur = tmp[i].hi;
		}
	}
	return hull;
}
std::vector<arc> solve(int l, int r, std::vector<circle>& C) {  // divide and conquer
	if (l == r) return { {0, 2 * PI, C[l]} };
	int m = l + r >> 1;
	std::vector<arc> HL = solve(l, m, C);
	std::vector<arc> HR = solve(m + 1, r, C);
	return merge(HL, HR);
}
std::vector<arc> HullDisks(std::vector<circle>& C) {
	for (auto& c : C) std::swap(c.x, c.y);  // swap x - y coord for convenience of angle calculation
	std::vector<arc> hull = solve(0, C.size() - 1, C);
	for (auto& A : hull) std::swap(A.c.x, A.c.y);  // set back
	return hull;
}
ld getPerimeter(std::vector<arc>& H) {  // hull = [[lo, hi, c], [lo, hi, c], [lo, hi, c]...]
	ld  R = 0;
	for (int i = 0; i < H.size(); i++) {
		arc  p = H[i], q = H[(i + 1) % H.size()];
		circle d = p.c - q.c;
		R += p.c.r * (p.hi - p.lo);  // calculate arc / r * theta
		R += sqrt((d.x * d.x + d.y * d.y) - d.r * d.r);  // calculate bridge / sqrt(a^2 - c^2) = b
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
	return A / 2;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(5);
	int T; std::cin >> T;
	while (T--) {
		int n; std::cin >> n;
		ll x, y, r;
		ld R;// , A;
		std::vector<circle> Circles(n);
		for (int i = 0; i < n; i++) {
			std::cin >> x >> y >> r;
			Circles[i].x = x;
			Circles[i].y = y;
			Circles[i].r = r;
		}
		std::vector<arc> Hull = HullDisks(Circles);
		R = getPerimeter(Hull);
		//A = getArea(Hull);
		std::cout << R << "\n";
		//std::cout << std::setprecision(50) << A << "\n";
	}
	return 0;
}