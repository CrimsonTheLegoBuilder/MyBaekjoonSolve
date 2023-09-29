#include <array>     // array
#include <iostream>  // input, output
#include <cmath>     // atan2l, asin, sin, cos...
#include <vector>    // vector
#include <algorithm> // min, max, sort..
#include <complex>   // complex no.
#include <iomanip>   // setprecision
using namespace std;
using lint = long long;
using pi = array<lint, 2>;  // pair ll * 2
#define sz(a) ((int)(a).size())
#define all(a) (a).begin(), (a).end()

// Returns the convex hull of circles in O(n log n)
// Arcs are in the form (f, lo, hi) where (f.a, f.b) denotes the center, f.r denotes the radius, and [lo, hi] denotes the radian range.
// Line segments are implicitly expressed as two endpoint (i, hi) - (i + 1, lo).
// Code inspired by https://codeforces.com/gym/104334/submission/204263938
// Thanks koosaga
namespace CircleHull {
	using func_t = int;
	using real_t = long double;
	const func_t eps = 1e-9;     // tolerance
	const real_t pi = acos(-1);  // pi = 3.14159265...

	struct func {
		// a: x coord ; b: y coord ; r : r of circle
		// f(x) = a*sin(x) + b*cos(x) + r  : y coord after coord-trans
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
		if (l >= r)
			return vector<arc>();  // ??
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
		real_t x2 = pi - x1;
		x1 = norm(x1 - phi);
		x2 = norm(x2 - phi);
		if (x1 > x2)
			swap(x1, x2);

		vector<arc> vec;
		if (p.eval((x1 + x2) / 2) < q.eval((x1 + x2) / 2)) {  // 
			vec = { {real_t(0), x1, p}, {x1, x2, q}, {x2, pi * 2, p} };  // CW
		}
		else {
			vec = { {real_t(0), x1, q}, {x1, x2, p}, {x2, pi * 2, q} };  // CW
		}

		vector<arc> ret;
		for (auto [lo, hi, f] : vec) {
			lo = max(lo, l);
			hi = min(hi, r);
			if (lo >= hi)
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
			if (i + 1 == sz(ret) || ret[i + 1].f != ret[i].f) {
				r.push_back({ cur, ret[i].hi, ret[i].f });
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
	while (true) {
		int n;
		cin >> n;
		if (n == 0)
			break;
		vector<CircleHull::func> a(n);
		for (auto& x : a) {
			double xa, xb, xr;
			cin >> xa >> xb >> xr;
			x.a = (int)round(1000 * xa);
			x.b = (int)round(1000 * xb);
			x.r = (int)round(1000 * xr);
		}
		cout << setprecision(69) << CircleHull::getPerimeter(a) / 1e3 << "\n";
	}
}