#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
#include <numeric>
typedef long long ll;
//typedef long double ld;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e18;
const ld TOL = 1e-5;
const ld PI = acos(-1);
const int LEN = 3005;
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline bool eq(const ld& x, const ld& y) { return zero(x - y); }
inline ld sq(const ld& x) { return x * x; }
inline ll sq(const ll& x) { return x * x; }
inline ld norm(ld th) { while (th < 0) th += 2 * PI; while (sign(th - 2 * PI) >= 0) th -= 2 * PI; return th; }
inline ld fit(const ld& x, const ld& lo = 0, const ld& hi = 1) { return std::min(hi, std::max(lo, x)); }
inline ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
inline ll gcd(ll x, ll y, ll z) {
	x = std::abs(x); y = std::abs(y); z = std::abs(z);
	ll w = gcd(x, y);
	return gcd(w, z);
}

/*

tested in range -1e5 < x, y < 1e5;
Delaunator - https://github.com/abellgithub/delaunator-cpp/blob/master/include/delaunator.cpp
modify : jinhwanlazy
I'm : stupid

*/

int N;
ll X_, Y_, Z_;
ld R_, D;
bool LF;//line flag
struct Pii {
	ll x, y; int i;
	Pii(ll x_ = 0, ll y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const ll& n) const { return { x * n, y * n }; }
	Pii operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pii& p) const { return { x * p.y - y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const ll& n) { x *= n; y *= n; return *this; }
	Pii& operator /= (const ll& n) { x /= n; y /= n; return *this; }
	Pii operator - () const { return { -x, -y }; }
	Pii operator ~ () const { return { -y, x }; }
	Pii operator ! () const { return { y, x }; }
	ll xy() const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
typedef std::vector<Pii> Vpii;
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return sign(cross(d1, d2, d3, d4)); }
std::vector<Pii> graham_scan(std::vector<Pii>& C) {
	std::vector<Pii> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pii& p, const Pii& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
struct Pos {
	ld x, y; int i;
	Pos(ld x_ = 0, ld y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	ld operator ^ (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator - () const { return Pos(-x, -y); }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
	ld xy() const { return x * y; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	bool close(const Pos& rhs,
		const ld span = 1.,
		const ld tol = 1e-20) const {
		return ((*this - rhs).Euc() / span) < tol;
	}
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Pos conv(const Pii& p) { return Pos(p.x, p.y, p.i); }
Pii conv(const Pos& p) { return Pii(round(p.x), round(p.y)); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool counterclockwise(const Pos& p, const Pos& q, const Pos& r) { return ccw(p, q, r) == 1; }
bool clockwise(const Pos& p, const Pos& q, const Pos& r) { return ccw(p, q, r) == -1; }
bool collinear(const Pos& p, const Pos& q, const Pos& r) { return !ccw(p, q, r); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) { return cross(d1, d2, t) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld circumradius(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
		(det > 0.0 || det < 0.0))
		return radius.Euc();
	return (std::numeric_limits<double>::max)();
}
Pos circumcenter(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	return p1 + radius;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, (std::numeric_limits<ld>::max)() };
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	if (!ccw(u, v, w)) return INVAL;
	Pos m1 = (u + v) * .5, v1 = ~(v - u);
	Pos m2 = (u + w) * .5, v2 = ~(w - u);
	Pos c = intersection(m1, m1 + v1, m2, m2 + v2);
	return Circle(c, (u - c).mag());
}
Circle circumcircle(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	Pos c;
	ld r;
	if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
		(det > 0.0 || det < 0.0))
		c = p1 + radius, r = radius.Euc();
	else c = Pos(INF, INF), r = std::numeric_limits<ld>::max();
	return Circle(c, r);
}
bool in_circle(const Pos& a, const Pos& b, const Pos& c, const Pos& p) {
	const Pos d = a - p;
	const Pos e = b - p;
	const Pos f = c - p;

	const double ap = d.Euc();
	const double bp = e.Euc();
	const double cp = f.Euc();

	return d / (e * cp - f * bp) + ap * (e / f) < 0.0;
}
class BBox2 {//jinhwanlazy
	constexpr static auto INF = std::numeric_limits<double>::max();
private:
	Pos bottom_left_;
	Pos top_right_;
	Pos center_;
	double span_;

public:
	BBox2(const std::vector<Pos>& points) {
		top_right_ = Pos(-INF, -INF);
		bottom_left_ = Pos(INF, INF);
		for (const Pos& p : points) {
			bottom_left_.x = std::min(bottom_left_.x, p.x);
			bottom_left_.y = std::min(bottom_left_.y, p.y);
			top_right_.x = std::max(top_right_.x, p.x);
			top_right_.y = std::max(top_right_.y, p.y);
		}
		center_ = (bottom_left_ + top_right_) / 2;
		span_ = (bottom_left_ - top_right_).Euc();
	}

	const Pos& bottomLeft() const { return bottom_left_; }
	const Pos& topRight() const { return top_right_; }
	const Pos& center() const { return center_; }
	const double& span() const { return span_; }
};
class Delaunator {
public:
	constexpr static auto INF = std::numeric_limits<double>::max();

	std::vector<Pos> points_;
	std::vector<std::size_t> triangles_;
	std::vector<std::size_t> halfedges_;
	std::vector<std::size_t> hull_prev_;
	std::vector<std::size_t> hull_next_;
	std::vector<std::size_t> hull_tri_;
	std::size_t hull_start_;

private:
	static constexpr std::size_t INVALID_INDEX = -1;

	std::vector<std::size_t> hull_hash_;
	Pos center_;
	std::size_t hash_size_;

public:
	Delaunator(std::vector<Pos> const& points) : points_(points) {
		std::size_t n = points.size();

		BBox2 bbox(points_);
		Pos center = bbox.center();

		std::size_t i0 = INVALID_INDEX;
		std::size_t i1 = INVALID_INDEX;
		std::size_t i2 = INVALID_INDEX;

		double min_dist = INF;
		for (size_t i = 0; i < points_.size(); ++i)
		{
			const Pos& p = points_[i];
			const double d = (p - center).Euc();
			if (d < min_dist) {
				i0 = i;
				min_dist = d;
			}
		}
		Pos p0 = points_[i0];

		min_dist = (std::numeric_limits<double>::max)();
		for (std::size_t i = 0; i < n; i++) {
			if (i == i0) continue;
			const double d = (p0 - points_[i]).Euc();
			if (d < min_dist && d > 0.0) {
				i1 = i;
				min_dist = d;
			}
		}
		Pos p1 = points_[i1];

		double min_radius = INF;
		for (std::size_t i = 0; i < n; i++) {
			if (i == i0 || i == i1)
				continue;
			const double r = circumradius(p0, p1, points_[i]);
			if (r < min_radius) {
				i2 = i;
				min_radius = r;
			}
		}
		Pos p2 = points_[i2];

		if (!(min_radius < INF)) {
			throw std::runtime_error("not triangulation");
		}

		if (counterclockwise(p0, p1, p2)) {
			std::swap(i1, i2);
			std::swap(p1, p2);
		}

		center_ = circumcenter(p0, p1, p2);

		std::vector<double> dists;
		dists.reserve(points_.size());
		for (const auto& p : points_)
			dists.push_back((p - center_).Euc());

		// sort the points by distance from the seed triangle circumcenter
		std::vector<std::size_t> ids(n);
		std::iota(ids.begin(), ids.end(), 0);
		std::sort(ids.begin(), ids.end(),
			[&dists](std::size_t i, std::size_t j) { return dists[i] < dists[j]; });

		// initialize a hash table for storing edges of the advancing convex hull
		hash_size_ = static_cast<std::size_t>(std::ceil(std::sqrt(n)));
		hull_hash_.resize(hash_size_, INVALID_INDEX);

		// initialize arrays for tracking the edges of the advancing convex hull
		hull_prev_.resize(n);
		hull_next_.resize(n);
		hull_tri_.resize(n);

		hull_start_ = i0;

		size_t hull_size = 3;

		hull_next_[i0] = hull_prev_[i2] = i1;
		hull_next_[i1] = hull_prev_[i0] = i2;
		hull_next_[i2] = hull_prev_[i1] = i0;

		hull_tri_[i0] = 0;
		hull_tri_[i1] = 1;
		hull_tri_[i2] = 2;

		hull_hash_[hash_key(p0)] = i0;
		hull_hash_[hash_key(p1)] = i1;
		hull_hash_[hash_key(p2)] = i2;

		std::size_t max_triangles_ = n < 3 ? 1 : 2 * n - 5;
		triangles_.reserve(max_triangles_ * 3);
		halfedges_.reserve(max_triangles_ * 3);
		add_triangle(i0, i1, i2, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX);
		Pos p_prev{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() };

		// Go through points based on distance from the center.
		for (std::size_t k = 0; k < n; k++) {
			const std::size_t i = ids[k];
			const Pos p = points_[i];

			// skip near-duplicate points
			if (k > 0 && p == p_prev)
				continue;
			p_prev = p;

			if (p == p0 || p == p1 || p == p2) {
				continue;
			}

			// find a visible edge on the convex hull using edge hash
			std::size_t start = 0;

			size_t key = hash_key(p);
			for (size_t j = 0; j < hash_size_; j++) {
				start = hull_hash_[(key + j) % hash_size_];
				if (start != INVALID_INDEX && start != hull_next_[start])
					break;
			}

			assert(hull_prev_[start] != start);
			assert(hull_prev_[start] != INVALID_INDEX);

			start = hull_prev_[start];
			size_t e = start;
			size_t q;

			// Advance until we find a place in the hull where our current point can be added.
			while (true) {
				q = hull_next_[e];
				if (p.close(points_[e], bbox.span()) || p.close(points_[q], bbox.span())) {
					e = INVALID_INDEX;
					break;
				}
				if (counterclockwise(p, points_[e], points_[q]))
					break;
				e = q;
				if (e == start) {
					e = INVALID_INDEX;
					break;
				}
			}

			if (e == INVALID_INDEX)  // likely a near-duplicate point; skip it
				continue;

			// add the first triangle from the point
			std::size_t t = add_triangle(e, i, hull_next_[e], INVALID_INDEX, INVALID_INDEX, hull_tri_[e]);

			hull_tri_[i] = legalize(t + 2);  // Legalize the triangle we just added.
			hull_tri_[e] = t;
			hull_size++;

			// walk forward through the hull, adding more triangles_ and flipping recursively
			std::size_t next = hull_next_[e];
			while (true) {
				q = hull_next_[next];
				if (!counterclockwise(p, points_[next], points_[q]))
					break;
				t = add_triangle(next, i, q, hull_tri_[i], INVALID_INDEX, hull_tri_[next]);
				hull_tri_[i] = legalize(t + 2);
				hull_next_[next] = next;  // mark as removed
				hull_size--;
				next = q;
			}

			// walk backward from the other side, adding more triangles_ and flipping
			if (e == start) {
				while (true) {
					q = hull_prev_[e];
					if (!counterclockwise(p, points_[q], points_[e]))
						break;
					t = add_triangle(q, i, e, INVALID_INDEX, hull_tri_[e], hull_tri_[q]);
					legalize(t + 2);
					hull_tri_[q] = t;
					hull_next_[e] = e;  // mark as removed
					hull_size--;
					e = q;
				}
			}

			// update the hull indices
			hull_prev_[i] = e;
			hull_start_ = e;
			hull_prev_[next] = i;
			hull_next_[e] = i;
			hull_next_[i] = next;

			hull_hash_[hash_key(p)] = i;
			hull_hash_[hash_key(points_[e])] = e;
		}
	}

private:
	std::size_t legalize(std::size_t a) {
		std::size_t i = 0;
		std::size_t ar = 0;
		std::vector<std::size_t> edges_stack;

		// recursion eliminated with a fixed-size stack
		while (true) {
			const size_t b = halfedges_[a];

			/* if the pair of triangles_ doesn't satisfy the Delaunay condition
			 * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
			 * then do the same check/flip recursively for the new pair of triangles_
			 *
			 *           pl                    pl
			 *          /||\                  /  \
			 *       al/ || \bl            al/    \a
			 *        /  ||  \              /      \
			 *       /  a||b  \    flip    /___ar___\
			 *     p0\   ||   /p1   =>   p0\---bl---/p1
			 *        \  ||  /              \      /
			 *       ar\ || /br             b\    /br
			 *          \||/                  \  /
			 *           pr                    pr
			 */
			const size_t a0 = 3 * (a / 3);
			ar = a0 + (a + 2) % 3;

			if (b == INVALID_INDEX) {
				if (i > 0) {
					i--;
					a = edges_stack[i];
					continue;
				}
				else {
					// i = INVALID_INDEX;
					break;
				}
			}

			const size_t b0 = 3 * (b / 3);
			const size_t al = a0 + (a + 1) % 3;
			const size_t bl = b0 + (b + 2) % 3;

			const std::size_t p0 = triangles_[ar];
			const std::size_t pr = triangles_[a];
			const std::size_t pl = triangles_[al];
			const std::size_t p1 = triangles_[bl];

			const bool illegal = in_circle(points_[p0], points_[pr], points_[pl], points_[p1]);

			if (illegal) {
				triangles_[a] = p1;
				triangles_[b] = p0;

				auto hbl = halfedges_[bl];

				// Edge swapped on the other side of the hull (rare).
				// Fix the halfedge reference
				if (hbl == INVALID_INDEX) {
					std::size_t e = hull_start_;
					do {
						if (hull_tri_[e] == bl) {
							hull_tri_[e] = a;
							break;
						}
						e = hull_prev_[e];
					} while (e != hull_start_);
				}
				link(a, hbl);
				link(b, halfedges_[ar]);
				link(ar, bl);
				std::size_t br = b0 + (b + 1) % 3;

				if (i < edges_stack.size()) {
					edges_stack[i] = br;
				}
				else {
					edges_stack.push_back(br);
				}
				i++;

			}
			else {
				if (i > 0) {
					i--;
					a = edges_stack[i];
					continue;
				}
				else {
					break;
				}
			}
		}
		return ar;
	};

	// monotonically increases with real angle, but doesn't need expensive trigonometry
	static inline double pseudo_angle(const double dx, const double dy) {
		const double p = dx / (std::abs(dx) + std::abs(dy));
		return (dy > 0.0 ? 3.0 - p : 1.0 + p) / 4.0;  // [0..1)
	}

	std::size_t hash_key(double x, double y) const {
		const double dx = x - center_.x;
		const double dy = y - center_.y;
		size_t key = std::llround(std::floor(pseudo_angle(dx, dy) * static_cast<double>(hash_size_)));
		return key % hash_size_;
	};

	std::size_t hash_key(const Pos& p) const {
		const Pos v = p - center_;
		size_t key = std::llround(std::floor(pseudo_angle(v.x, v.y) * static_cast<double>(hash_size_)));
		return key % hash_size_;
	};

	std::size_t add_triangle(std::size_t i0,
		std::size_t i1,
		std::size_t i2,
		std::size_t a,
		std::size_t b,
		std::size_t c) {
		std::size_t t = triangles_.size();
		triangles_.push_back(i0);
		triangles_.push_back(i1);
		triangles_.push_back(i2);
		link(t, a);
		link(t + 1, b);
		link(t + 2, c);
		return t;
	}

	void link(std::size_t a, std::size_t b) {
		std::size_t s = halfedges_.size();
		if (a == s) {
			halfedges_.push_back(b);
		}
		else if (a < s) {
			halfedges_[a] = b;
		}
		else {
			throw std::runtime_error("Cannot link edge");
		}
		if (b != INVALID_INDEX) {
			std::size_t s2 = halfedges_.size();
			if (b == s2) {
				halfedges_.push_back(a);
			}
			else if (b < s2) {
				halfedges_[b] = a;
			}
			else {
				throw std::runtime_error("Cannot link edge");
			}
		}
	}
};
struct Seg {
	Pos s, e, dir;
	Seg(Pos s_ = Pos(), Pos e_ = Pos()) : s(s_), e(e_) { dir = e - s; }
	//bool operator < (const Seg& l) const { return s == l.s ? e < l.e : s < l.s; }
	bool inner(const Pos& p) const { return sign(dir / (p - s)) > 0; }
	friend bool parallel(const Seg& l0, const Seg& l1) { return zero(l0.dir / l1.dir); }
	friend bool same_dir(const Seg& l0, const Seg& l1) { return parallel(l0, l1) && l0.dir * l1.dir > 0; }
	friend Pos intersection_(const Seg& s1, const Seg& s2) {
		const Pos& p1 = s1.s, & p2 = s1.e;
		const Pos& q1 = s2.s, & q2 = s2.e;
		ld a1 = cross(q1, q2, p1);
		ld a2 = -cross(q1, q2, p2);
		return (p1 * a2 + p2 * a1) / (a1 + a2);
	}
	bool operator < (const Seg& l) const {
		if (same_dir(*this, l)) return l.inner(s);
		bool f0 = O < dir;
		bool f1 = O < l.dir;
		if (f0 != f1) return f1;
		return sign(dir / l.dir) > 0;
	}
	//bool operator == (const Seg& l) const { return s == l.s && e == l.e; }
	Pos p(const ld& rt = .5) const { return s + (e - s) * rt; }
	ld green(const ld& lo = 0, const ld& hi = 1) const {
		ld d = hi - lo;
		ld ratio = (lo + hi) * .5;
		Pos m = p(ratio);
		return m.y * d * (s.x - e.x);
	}
};
typedef std::vector<Seg> Segs;
ld dot(const Seg& p, const Seg& q) { return dot(p.s, p.e, q.s, q.e); }
ld intersection(const Seg& s1, const Seg& s2, const bool& f = 0) {
	const Pos& p1 = s1.s, p2 = s1.e, q1 = s2.s, q2 = s2.e;
	ld det = (q2 - q1) / (p2 - p1);
	if (zero(det)) return -1;
	ld a1 = ((q2 - q1) / (q1 - p1)) / det;
	ld a2 = ((p2 - p1) / (p1 - q1)) / -det;
	if (f == 1) return fit(a1, 0, 1);
	if (0 < a1 && a1 < 1 && -TOL < a2 && a2 < 1 + TOL) return a1;
	return -1;
}
Segs half_plane_intersection(Segs& HP, const bool& srt = 1) {
	auto check = [&](Seg& u, Seg& v, Seg& w) -> bool {
		return w.inner(intersection_(u, v));
		};
	if (srt) std::sort(HP.begin(), HP.end());
	std::deque<Seg> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Seg> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(dq[i]);
	return HPI;
}
Segs half_plane_intersection(const Segs& P, const Segs& Q) {
	Segs HP(P.size() + Q.size());
	std::merge(P.begin(), P.end(), Q.begin(), Q.end(), HP.begin());
	return half_plane_intersection(HP, 0);
}
Segs cell(std::vector<Pos>& C, const int& idx, const int f = 1) {
	int sz = C.size();
	ld mx = 1e9;
	Segs HP = {
		Seg(Pos(-mx, -mx), Pos(mx, -mx)),
		Seg(Pos(mx, -mx), Pos(mx, mx)),
		Seg(Pos(mx, mx), Pos(-mx, mx)),
		Seg(Pos(-mx, mx), Pos(-mx, -mx))
	};
	for (int i = 0; i < sz; i++) {
		if (i == idx) continue;
		Pos v = ~(C[i] - C[idx]);
		v *= f;
		Pos m = (C[i] + C[idx]) * .5;
		HP.push_back(Seg(m, m + v));
	}
	return half_plane_intersection(HP);
}
void update_XYZ(const Seg& u, const Seg& v) {
	Pii p1 = conv(u.s * 2);
	Pii p2 = conv(u.e * 2);
	Pii q1 = conv(v.s * 2);
	Pii q2 = conv(v.e * 2);
	ll a1 = cross(q1, q2, p1);
	ll a2 = -cross(q1, q2, p2);
	ll det = (a1 + a2);
	Pii inx = p1 * a2 + p2 * a1;
	X_ = inx.x;
	Y_ = inx.y;
	Z_ = det * 2;
	return;
}
Vint DT[LEN], NG;
Segs VDP[LEN];
Segs VDN[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Vpii Cii(N);
	for (int i = 0; i < N; i++) { std::cin >> Cii[i], Cii[i].i = i; }
	if (N == 1) { std::cout << "0\nL 1 0 " << Cii[0].x << "\n"; return; }
	Vpii P_ = Cii;
	D = 1e27;
	LF = 1;//line flag
	Vpii H = graham_scan(P_);
	if (H.size() == 2) {
		Pii s = H[0], e = H[1];
		Pii vec = e - s;
		X_ = -vec.y;
		Y_ = vec.x;
		Pos v = ~conv(vec);
		Pos m = (conv(s) + conv(e)) * .5;
		Z_ = round(v * m);
		std::cout << "0\nL " << X_ << " " << Y_ << " " << Z_ << "\n";
		return;
	}
	int sz = H.size();
	auto jaw_check = [&](const int& i, const int& j) -> bool {
		return ccw(H[i], H[(i + 1) % sz], H[j], H[(j + 1) % sz]) >= 0;
		};
	for (int i = 0, j = 1; i < sz; i++) {
		while (jaw_check(i, j)) j = (j + 1) % sz;
		ld d = cross(H[i], H[(i + 1) % sz], H[j]) / (H[i] - H[(i + 1) % sz]).mag();
		d *= .5;
		if (D > d) {
			D = d;
			Pii s = H[i], e = H[(i + 1) % sz];
			Pii vec = e - s;
			X_ = -vec.y;
			Y_ = vec.x;
			Pos v = ~conv(vec);
			Pos m = (conv(s) + conv(H[j])) * .5;
			Z_ = round(v * m);
		}
	}
	Polygon C;
	for (Pii& p : Cii) C.push_back(conv(p));
	Delaunator DTR(C);
	for (int i = 0; i < DTR.triangles_.size(); i += 3) {
		const int& a = DTR.points_[DTR.triangles_[i]].i;
		const int& b = DTR.points_[DTR.triangles_[i + 1]].i;
		const int& c = DTR.points_[DTR.triangles_[i + 2]].i;
		DT[a].push_back(b); DT[a].push_back(c);
		DT[b].push_back(a); DT[b].push_back(c);
		DT[c].push_back(a); DT[c].push_back(b);
	}
	for (int c = 0; c < N; c++) {
		Vint& I = DT[c];
		std::sort(I.begin(), I.end());
		I.erase(unique(I.begin(), I.end()), I.end());
		Pii sd = Cii[c];//seed
		ld mx = 1e9;
		Segs HP = {
			Seg(Pos(-mx, -mx), Pos(mx, -mx)),
			Seg(Pos(mx, -mx), Pos(mx, mx)),
			Seg(Pos(mx, mx), Pos(-mx, mx)),
			Seg(Pos(-mx, mx), Pos(-mx, -mx))
		};
		for (const int& i : I) {
			Pii p = Cii[i];
			Pii vec = p - sd;
			ll g = gcd(std::abs(vec.x), std::abs(vec.y));
			vec /= g;
			Pos v = ~conv(vec);
			Pos s = conv(sd), e = conv(p);
			Pos m = (s + e) * .5;
			HP.push_back(Seg(m, m + v));
		}
		Segs hpi = half_plane_intersection(HP);
		if (hpi.size() < 3) continue;
		VDP[c] = hpi;
	}
	for (const Pii& q : H) {
		int c = q.i;
		Segs hpi = cell(C, q.i, -1);
		if (hpi.size() < 3) continue;
		VDN[c] = hpi;
		NG.push_back(q.i);
	}
	for (int pv = 0; pv < N; pv++) {
		if (!VDP[pv].size()) continue;
		Pii p_ = Cii[pv];
		Pos sdp = C[pv];//seed
		for (const int& ng : NG) {
			//if (!VDN[ng].size()) continue;
			Segs inx = half_plane_intersection(VDP[pv], VDN[ng]);
			int sz = inx.size();
			if (sz < 3) continue;
			Pos sdn = C[ng];//seed
			for (int i = 0; i < sz; i++) {
				int j = (i + 1) % sz;
				const Seg& l0 = inx[i];
				const Seg& l1 = inx[j];
				//if (!ccw(l0.s, l0.e, l1.s, l1.e)) continue;
				Pos ix = intersection_(l0, l1);
				ld dn = (sdn - ix).mag();
				ld dp = (sdp - ix).mag();
				ld d = dn - dp;
				d *= .5;
				if (D > d) {
					D = d;
					R_ = (dn + dp) * .5;
					update_XYZ(l0, l1);
					LF = 0;
				}
			}
		}
	}
	if (zero(D)) D = +0;
	if (zero(R_)) R_ = +0;
	ll g = gcd(X_, Y_, Z_);
	X_ /= g; Y_ /= g; Z_ /= g;
	if (LF) std::cout << D << "\nL " << X_ << " " << Y_ << " " << Z_ << "\n";
	else std::cout << D << "\nC " << X_ << " " << Y_ << " " << Z_ << " " << R_ << "\n";
	return;
}
int main() { solve(); return 0; }//boj33390 Keychain refer to Crysfly from QOJ

/*

더 큰 수를 정의하지 않기 위해 일부러 직선 형태로만 반평면 교집합을 관리하는 꼼수를 배웠음.
정수형 답을 반환하기 위해 모든 바이섹터가 .5 단위로 표현될 수 있음을 응용.
양의 반평면 교집합은 seed와 다른 점들을 같은 거리에 둘 수 있는 원의 중심이 있을 수 있는 구역
음의 반평면 교집합은 seed와 다른 점들을 원의 서로 다른 면에 두는 원의 중심이 있을 수 있는 구역
잔머리 보소

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <array>
#include <tuple>
#include <complex>
#include <numeric>
typedef long long ll;
typedef long double ld;
//typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
typedef std::vector<bool> Vbool;
const ld INF = 1e18;
const ld TOL = 1e-5;
const ld PI = acos(-1);
const int LEN = 3005;
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }
inline bool zero(const ld& x) { return !sign(x); }
inline ll sq(ll x) { return (ll)x * x; }
inline ld sq(ld x) { return x * x; }
inline ld norm(ld th) {
	while (th < 0) th += 2 * PI;
	while (sign(th - 2 * PI) >= 0) th -= 2 * PI;
	return th;
}
inline ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
inline ll gcd(ll x, ll y, ll z) {
	x = std::abs(x); y = std::abs(y); z = std::abs(z);
	ll w = gcd(x, y);
	return gcd(w, z);
}

int N;
ll X_, Y_, Z_;
ld R_, D;
bool LF;//line flag
struct Pii {
	ll x, y; int i;
	Pii(ll x_ = 0, ll y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
	Pii operator * (const ll& n) const { return { x * n, y * n }; }
	Pii operator / (const ll& n) const { return { x / n, y / n }; }
	ll operator * (const Pii& p) const { return { x * p.x + y * p.y }; }
	ll operator / (const Pii& p) const { return { x * p.y - y * p.x }; }
	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
	Pii& operator *= (const ll& n) { x *= n; y *= n; return *this; }
	Pii& operator /= (const ll& n) { x /= n; y /= n; return *this; }
	Pii operator - () const { return { -x, -y }; }
	Pii operator ~ () const { return { -y, x }; }
	Pii operator ! () const { return { y, x }; }
	ll xy() const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	ll Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (int)1e9, (int)1e9 };
typedef std::vector<Pii> Vpii;
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return sign(cross(d1, d2, d3, d4)); }
std::vector<Pii> graham_scan(std::vector<Pii>& C) {
	std::vector<Pii> H;
	if (C.size() < 3) {
		std::sort(C.begin(), C.end());
		return C;
	}
	std::swap(C[0], *min_element(C.begin(), C.end()));
	std::sort(C.begin() + 1, C.end(), [&](const Pii& p, const Pii& q) -> bool {
		int ret = ccw(C[0], p, q);
		if (!ret) return (C[0] - p).Euc() < (C[0] - q).Euc();
		return ret > 0;
		}
	);
	C.erase(unique(C.begin(), C.end()), C.end());
	int sz = C.size();
	for (int i = 0; i < sz; i++) {
		while (H.size() >= 2 && ccw(H[H.size() - 2], H.back(), C[i]) <= 0)
			H.pop_back();
		H.push_back(C[i]);
	}
	return H;
}
struct Pos {
	ld x, y, i;
	Pos(ld x_ = 0, ld y_ = 0, int i_ = 0) : x(x_), y(y_), i(i_) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	ld operator ^ (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator - () const { return Pos(-x, -y); }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& n) { x *= n; y *= n; return *this; }
	Pos& operator /= (const ld& n) { x /= n; y /= n; return *this; }
	ld xy() const { return x * y; }
	Pos rot(const ld& t) { return { x * cos(t) - y * sin(t), x * sin(t) + y * cos(t) }; }
	ld Euc() const { return x * x + y * y; }
	ld mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : sign(a / b) > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	bool close(const Pos& rhs,
		const ld span = 1.,
		const ld tol = 1e-20) const {
		return ((*this - rhs).Euc() / span) < tol;
	}
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
Pos conv(const Pii& p) { return Pos(p.x, p.y, p.i); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { return sign(cross(d1, d2, d3)); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return sign(cross(d1, d2, d3, d4)); }
bool counterclockwise(const Pos& p, const Pos& q, const Pos& r) { return ccw(p, q, r) == 1; }
bool clockwise(const Pos& p, const Pos& q, const Pos& r) { return ccw(p, q, r) == -1; }
bool collinear(const Pos& p, const Pos& q, const Pos& r) { return !ccw(p, q, r); }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && sign(dot(d1, d3, d2)) > 0; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld dist(const Pos& d1, const Pos& d2, const Pos& t) { return cross(d1, d2, t) / (d1 - d2).mag(); }
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) { ld a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2); return (p1 * a2 + p2 * a1) / (a1 + a2); }
ld circumradius(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
		(det > 0.0 || det < 0.0))
		return radius.Euc();
	return (std::numeric_limits<double>::max)();
}
Pos circumcenter(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	return p1 + radius;
}
struct Circle {
	Pos c;
	ld r;
	Circle(Pos C = Pos(0, 0), ld R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		ld dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	ld H(const ld& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	ld A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, (std::numeric_limits<ld>::max)() };
Circle enclose_circle(const Pos& u, const Pos& v, const Pos& w) {
	if (!ccw(u, v, w)) return INVAL;
	Pos m1 = (u + v) * .5, v1 = ~(v - u);
	Pos m2 = (u + w) * .5, v2 = ~(w - u);
	Pos c = intersection(m1, m1 + v1, m2, m2 + v2);
	return Circle(c, (u - c).mag());
}
Circle circumcircle(const Pos& p1, const Pos& p2, const Pos& p3) {
	Pos d = p2 - p1;
	Pos e = p3 - p1;
	const ld bl = d.Euc();
	const ld cl = e.Euc();
	const ld det = d / e;
	Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
		(d.x * cl - e.x * bl) * 0.5 / det);
	Pos c;
	ld r;
	if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
		(det > 0.0 || det < 0.0))
		c = p1 + radius, r = radius.Euc();
	else c = Pos(INF, INF), r = std::numeric_limits<ld>::max();
	return Circle(c, r);
}
bool in_circle(const Pos& a, const Pos& b, const Pos& c, const Pos& p) {
	const Pos d = a - p;
	const Pos e = b - p;
	const Pos f = c - p;

	const double ap = d.Euc();
	const double bp = e.Euc();
	const double cp = f.Euc();

	return d / (e * cp - f * bp) + ap * (e / f) < 0.0;
}
class BBox2 {//jinhwanlazy
	constexpr static auto INF = std::numeric_limits<double>::max();
private:
	Pos bottom_left_;
	Pos top_right_;
	Pos center_;
	double span_;

public:
	BBox2(const std::vector<Pos>& points) {
		top_right_ = Pos(-INF, -INF);
		bottom_left_ = Pos(INF, INF);
		for (const Pos& p : points) {
			bottom_left_.x = std::min(bottom_left_.x, p.x);
			bottom_left_.y = std::min(bottom_left_.y, p.y);
			top_right_.x = std::max(top_right_.x, p.x);
			top_right_.y = std::max(top_right_.y, p.y);
		}
		center_ = (bottom_left_ + top_right_) / 2;
		span_ = (bottom_left_ - top_right_).Euc();
	}

	const Pos& bottomLeft() const { return bottom_left_; }
	const Pos& topRight() const { return top_right_; }
	const Pos& center() const { return center_; }
	const double& span() const { return span_; }
};
class Delaunator {
public:
	constexpr static auto INF = std::numeric_limits<double>::max();

	std::vector<Pos> points_;
	std::vector<std::size_t> triangles_;
	std::vector<std::size_t> halfedges_;
	std::vector<std::size_t> hull_prev_;
	std::vector<std::size_t> hull_next_;
	std::vector<std::size_t> hull_tri_;
	std::size_t hull_start_;

private:
	static constexpr std::size_t INVALID_INDEX = -1;

	std::vector<std::size_t> hull_hash_;
	Pos center_;
	std::size_t hash_size_;

public:
	Delaunator(std::vector<Pos> const& points) : points_(points) {
		std::size_t n = points.size();

		BBox2 bbox(points_);
		Pos center = bbox.center();

		std::size_t i0 = INVALID_INDEX;
		std::size_t i1 = INVALID_INDEX;
		std::size_t i2 = INVALID_INDEX;

		double min_dist = INF;
		for (size_t i = 0; i < points_.size(); ++i)
		{
			const Pos& p = points_[i];
			const double d = (p - center).Euc();
			if (d < min_dist) {
				i0 = i;
				min_dist = d;
			}
		}
		Pos p0 = points_[i0];

		min_dist = (std::numeric_limits<double>::max)();
		for (std::size_t i = 0; i < n; i++) {
			if (i == i0) continue;
			const double d = (p0 - points_[i]).Euc();
			if (d < min_dist && d > 0.0) {
				i1 = i;
				min_dist = d;
			}
		}
		Pos p1 = points_[i1];

		double min_radius = INF;
		for (std::size_t i = 0; i < n; i++) {
			if (i == i0 || i == i1)
				continue;
			const double r = circumradius(p0, p1, points_[i]);
			if (r < min_radius) {
				i2 = i;
				min_radius = r;
			}
		}
		Pos p2 = points_[i2];

		if (!(min_radius < INF)) {
			throw std::runtime_error("not triangulation");
		}

		if (counterclockwise(p0, p1, p2)) {
			std::swap(i1, i2);
			std::swap(p1, p2);
		}

		center_ = circumcenter(p0, p1, p2);

		std::vector<double> dists;
		dists.reserve(points_.size());
		for (const auto& p : points_)
			dists.push_back((p - center_).Euc());

		// sort the points by distance from the seed triangle circumcenter
		std::vector<std::size_t> ids(n);
		std::iota(ids.begin(), ids.end(), 0);
		std::sort(ids.begin(), ids.end(),
			[&dists](std::size_t i, std::size_t j) { return dists[i] < dists[j]; });

		// initialize a hash table for storing edges of the advancing convex hull
		hash_size_ = static_cast<std::size_t>(std::ceil(std::sqrt(n)));
		hull_hash_.resize(hash_size_, INVALID_INDEX);

		// initialize arrays for tracking the edges of the advancing convex hull
		hull_prev_.resize(n);
		hull_next_.resize(n);
		hull_tri_.resize(n);

		hull_start_ = i0;

		size_t hull_size = 3;

		hull_next_[i0] = hull_prev_[i2] = i1;
		hull_next_[i1] = hull_prev_[i0] = i2;
		hull_next_[i2] = hull_prev_[i1] = i0;

		hull_tri_[i0] = 0;
		hull_tri_[i1] = 1;
		hull_tri_[i2] = 2;

		hull_hash_[hash_key(p0)] = i0;
		hull_hash_[hash_key(p1)] = i1;
		hull_hash_[hash_key(p2)] = i2;

		std::size_t max_triangles_ = n < 3 ? 1 : 2 * n - 5;
		triangles_.reserve(max_triangles_ * 3);
		halfedges_.reserve(max_triangles_ * 3);
		add_triangle(i0, i1, i2, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX);
		Pos p_prev{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() };

		// Go through points based on distance from the center.
		for (std::size_t k = 0; k < n; k++) {
			const std::size_t i = ids[k];
			const Pos p = points_[i];

			// skip near-duplicate points
			if (k > 0 && p == p_prev)
				continue;
			p_prev = p;

			if (p == p0 || p == p1 || p == p2) {
				continue;
			}

			// find a visible edge on the convex hull using edge hash
			std::size_t start = 0;

			size_t key = hash_key(p);
			for (size_t j = 0; j < hash_size_; j++) {
				start = hull_hash_[(key + j) % hash_size_];
				if (start != INVALID_INDEX && start != hull_next_[start])
					break;
			}

			assert(hull_prev_[start] != start);
			assert(hull_prev_[start] != INVALID_INDEX);

			start = hull_prev_[start];
			size_t e = start;
			size_t q;

			// Advance until we find a place in the hull where our current point can be added.
			while (true) {
				q = hull_next_[e];
				if (p.close(points_[e], bbox.span()) || p.close(points_[q], bbox.span())) {
					e = INVALID_INDEX;
					break;
				}
				if (counterclockwise(p, points_[e], points_[q]))
					break;
				e = q;
				if (e == start) {
					e = INVALID_INDEX;
					break;
				}
			}

			if (e == INVALID_INDEX)  // likely a near-duplicate point; skip it
				continue;

			// add the first triangle from the point
			std::size_t t = add_triangle(e, i, hull_next_[e], INVALID_INDEX, INVALID_INDEX, hull_tri_[e]);

			hull_tri_[i] = legalize(t + 2);  // Legalize the triangle we just added.
			hull_tri_[e] = t;
			hull_size++;

			// walk forward through the hull, adding more triangles_ and flipping recursively
			std::size_t next = hull_next_[e];
			while (true) {
				q = hull_next_[next];
				if (!counterclockwise(p, points_[next], points_[q]))
					break;
				t = add_triangle(next, i, q, hull_tri_[i], INVALID_INDEX, hull_tri_[next]);
				hull_tri_[i] = legalize(t + 2);
				hull_next_[next] = next;  // mark as removed
				hull_size--;
				next = q;
			}

			// walk backward from the other side, adding more triangles_ and flipping
			if (e == start) {
				while (true) {
					q = hull_prev_[e];
					if (!counterclockwise(p, points_[q], points_[e]))
						break;
					t = add_triangle(q, i, e, INVALID_INDEX, hull_tri_[e], hull_tri_[q]);
					legalize(t + 2);
					hull_tri_[q] = t;
					hull_next_[e] = e;  // mark as removed
					hull_size--;
					e = q;
				}
			}

			// update the hull indices
			hull_prev_[i] = e;
			hull_start_ = e;
			hull_prev_[next] = i;
			hull_next_[e] = i;
			hull_next_[i] = next;

			hull_hash_[hash_key(p)] = i;
			hull_hash_[hash_key(points_[e])] = e;
		}
	}

private:
	std::size_t legalize(std::size_t a) {
		std::size_t i = 0;
		std::size_t ar = 0;
		std::vector<std::size_t> edges_stack;

		// recursion eliminated with a fixed-size stack
		while (true) {
			const size_t b = halfedges_[a];

			/* if the pair of triangles_ doesn't satisfy the Delaunay condition
			 * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
			 * then do the same check/flip recursively for the new pair of triangles_
			 *
			 *           pl                    pl
			 *          /||\                  /  \
			 *       al/ || \bl            al/    \a
			 *        /  ||  \              /      \
			 *       /  a||b  \    flip    /___ar___\
			 *     p0\   ||   /p1   =>   p0\---bl---/p1
			 *        \  ||  /              \      /
			 *       ar\ || /br             b\    /br
			 *          \||/                  \  /
			 *           pr                    pr
			 */
			const size_t a0 = 3 * (a / 3);
			ar = a0 + (a + 2) % 3;

			if (b == INVALID_INDEX) {
				if (i > 0) {
					i--;
					a = edges_stack[i];
					continue;
				}
				else {
					// i = INVALID_INDEX;
					break;
				}
			}

			const size_t b0 = 3 * (b / 3);
			const size_t al = a0 + (a + 1) % 3;
			const size_t bl = b0 + (b + 2) % 3;

			const std::size_t p0 = triangles_[ar];
			const std::size_t pr = triangles_[a];
			const std::size_t pl = triangles_[al];
			const std::size_t p1 = triangles_[bl];

			const bool illegal = in_circle(points_[p0], points_[pr], points_[pl], points_[p1]);

			if (illegal) {
				triangles_[a] = p1;
				triangles_[b] = p0;

				auto hbl = halfedges_[bl];

				// Edge swapped on the other side of the hull (rare).
				// Fix the halfedge reference
				if (hbl == INVALID_INDEX) {
					std::size_t e = hull_start_;
					do {
						if (hull_tri_[e] == bl) {
							hull_tri_[e] = a;
							break;
						}
						e = hull_prev_[e];
					} while (e != hull_start_);
				}
				link(a, hbl);
				link(b, halfedges_[ar]);
				link(ar, bl);
				std::size_t br = b0 + (b + 1) % 3;

				if (i < edges_stack.size()) {
					edges_stack[i] = br;
				}
				else {
					edges_stack.push_back(br);
				}
				i++;

			}
			else {
				if (i > 0) {
					i--;
					a = edges_stack[i];
					continue;
				}
				else {
					break;
				}
			}
		}
		return ar;
	};

	// monotonically increases with real angle, but doesn't need expensive trigonometry
	static inline double pseudo_angle(const double dx, const double dy) {
		const double p = dx / (std::abs(dx) + std::abs(dy));
		return (dy > 0.0 ? 3.0 - p : 1.0 + p) / 4.0;  // [0..1)
	}

	std::size_t hash_key(double x, double y) const {
		const double dx = x - center_.x;
		const double dy = y - center_.y;
		size_t key = std::llround(std::floor(pseudo_angle(dx, dy) * static_cast<double>(hash_size_)));
		return key % hash_size_;
	};

	std::size_t hash_key(const Pos& p) const {
		const Pos v = p - center_;
		size_t key = std::llround(std::floor(pseudo_angle(v.x, v.y) * static_cast<double>(hash_size_)));
		return key % hash_size_;
	};

	std::size_t add_triangle(std::size_t i0,
		std::size_t i1,
		std::size_t i2,
		std::size_t a,
		std::size_t b,
		std::size_t c) {
		std::size_t t = triangles_.size();
		triangles_.push_back(i0);
		triangles_.push_back(i1);
		triangles_.push_back(i2);
		link(t, a);
		link(t + 1, b);
		link(t + 2, c);
		return t;
	}

	void link(std::size_t a, std::size_t b) {
		std::size_t s = halfedges_.size();
		if (a == s) {
			halfedges_.push_back(b);
		}
		else if (a < s) {
			halfedges_[a] = b;
		}
		else {
			throw std::runtime_error("Cannot link edge");
		}
		if (b != INVALID_INDEX) {
			std::size_t s2 = halfedges_.size();
			if (b == s2) {
				halfedges_.push_back(a);
			}
			else if (b < s2) {
				halfedges_[b] = a;
			}
			else {
				throw std::runtime_error("Cannot link edge");
			}
		}
	}
};
struct Linear {//ps[0] -> ps[1] ::
	Pos ps[2];
	Pos dir_;
	const Pos& operator [] (const int& i) const { return ps[i]; }
	const Pos& dir() const { return dir_; }
	Linear(Pos a = Pos(0, 0), Pos b = Pos(0, 0)) {
		ps[0] = a;
		ps[1] = b;
		//dir_ = (ps[1] - ps[0]).unit();
		dir_ = (ps[1] - ps[0]);
	}
	bool include(const Pos& p) const { return sign(dir_ / (p - ps[0])) > 0; }
	friend bool parallel(const Linear& l0, const Linear& l1) { return zero(l0.dir() / l1.dir()); }
	friend bool same_dir(const Linear& l0, const Linear& l1) { return parallel(l0, l1) && l0.dir() * l1.dir() > 0; }
	bool operator < (const Linear& l0) const {
		if (same_dir(*this, l0)) return l0.include(ps[0]);
		else return cmpq(this->dir(), l0.dir());
	}
};
typedef std::vector<Linear> VHP;
Pos intersection(const Linear& l1, const Linear& l2) { return intersection(l1[0], l1[1], l2[0], l2[1]); }
void init(std::vector<Linear>& HP, const ld& mx = 5e9) {
	HP.push_back(Linear(Pos(-mx, -mx), Pos(mx, -mx)));
	HP.push_back(Linear(Pos(mx, -mx), Pos(mx, mx)));
	HP.push_back(Linear(Pos(mx, mx), Pos(-mx, mx)));
	HP.push_back(Linear(Pos(-mx, mx), Pos(-mx, -mx)));
	return;
}
//std::vector<Pos> half_plane_intersection(std::vector<Linear>& HP) {
std::vector<Linear> half_plane_intersection(std::vector<Linear>& HP, const bool& srt = 1) {
	auto check = [&](Linear& u, Linear& v, Linear& w) -> bool {
		return w.include(intersection(u, v));
		};
	if (srt) std::sort(HP.begin(), HP.end());
	std::deque<Linear> dq;
	int sz = HP.size();
	for (int i = 0; i < sz; ++i) {
		if (i && same_dir(HP[i], HP[(i - 1) % sz])) continue;
		while (dq.size() > 1 && !check(dq[dq.size() - 2], dq[dq.size() - 1], HP[i])) dq.pop_back();
		while (dq.size() > 1 && !check(dq[1], dq[0], HP[i])) dq.pop_front();
		dq.push_back(HP[i]);
	}
	while (dq.size() > 2 && !check(dq[dq.size() - 2], dq[dq.size() - 1], dq[0])) dq.pop_back();
	while (dq.size() > 2 && !check(dq[1], dq[0], dq[dq.size() - 1])) dq.pop_front();
	sz = dq.size();
	if (sz < 3) return {};
	std::vector<Linear> HPI;
	for (int i = 0; i < sz; ++i) HPI.push_back(dq[i]);
	return HPI;
	//std::vector<Pos> HPI;
	//for (int i = 0; i < sz; ++i) HPI.push_back(intersection(dq[i], dq[(i + 1) % sz]));
	//return HPI;
}
std::vector<Linear> half_plane_intersection(const VHP& P, const VHP& Q) {
	//std::vector<Linear> HP;
	//int sz;
	//sz = P.size();
	//for (int i = 0; i < sz; i++) HP.push_back(P[i]);
	//sz = Q.size();
	//for (int i = 0; i < sz; i++) HP.push_back(Q[i]);
	//return half_plane_intersection(HP);
	std::vector<Linear> HP(P.size() + Q.size());
	std::merge(P.begin(), P.end(), Q.begin(), Q.end(), HP.begin());
	return half_plane_intersection(HP, 0);
}
std::vector<Linear> get_cell(std::vector<Pos>& C, const int& idx, const int f = 1) {
	int sz = C.size();
	std::vector<Linear> HP; init(HP);
	for (int i = 0; i < sz; i++) {
		if (i == idx) continue;
		Pos v = ~(C[i] - C[idx]);
		v *= f;
		Pos m = (C[i] + C[idx]) * .5;
		HP.push_back(Linear(m, m + v));
	}
	return half_plane_intersection(HP);
}
Pii conv(const Pos& p) {
	ll x = round(p.x);
	ll y = round(p.y);
	return Pii(x, y);
}
//Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
//	ld a1 = cross(q1, q2, p1);
//	ld a2 = -cross(q1, q2, p2);
//	return (p1 * a2 + p2 * a1) / (a1 + a2);
//}
void update_XYZ(const Linear& u, const Linear& v) {
	Pii p1 = conv(u[0] * 2);
	Pii p2 = conv(u[1] * 2);
	Pii q1 = conv(v[0] * 2);
	Pii q2 = conv(v[1] * 2);
	ll a1 = cross(q1, q2, p1);
	ll a2 = -cross(q1, q2, p2);
	ll det = (a1 + a2);
	Pii inx = p1 * a2 + p2 * a1;
	X_ = inx.x;
	Y_ = inx.y;
	Z_ = det * 2;
	return;
}
Vint DT[LEN], NG;
std::vector<Linear> VDP[LEN];
std::vector<Linear> VDN[LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	std::cin >> N;
	Vpii Cii(N);
	for (int i = 0; i < N; i++) { std::cin >> Cii[i], Cii[i].i = i; }
	if (N == 1) { std::cout << "0\nL 1 0 " << Cii[0].x << "\n"; return; }
	if (N == 2) {
		Pii s = Cii[0], e = Cii[1];
		Pii vec = e - s;
		X_ = -vec.y;
		Y_ = vec.x;
		Pos v = ~conv(vec);
		Pos m = (conv(s) + conv(e)) * .5;
		Z_ = round(v * m);
		std::cout << "0\nL " << X_ << " " << Y_ << " " << Z_ << "\n";
		return;
	}
	Vpii P_ = Cii;
	D = 1e27;
	LF = 1;//line flag
	//line
	Vpii H = graham_scan(P_);
	if (H.size() == 2) {
		Pii s = H[0], e = H[1];
		Pii vec = e - s;
		X_ = -vec.y;
		Y_ = vec.x;
		Pos v = ~conv(vec);
		Pos m = (conv(s) + conv(e)) * .5;
		Z_ = round(v * m);
		std::cout << "0\nL " << X_ << " " << Y_ << " " << Z_ << "\n";
		return;
	}
	int sz = H.size();
	auto jaw_check = [&](const int& i, const int& j) -> bool {
		return ccw(H[i], H[(i + 1) % sz], H[j], H[(j + 1) % sz]) >= 0;
		};
	for (int i = 0, j = 1; i < sz; i++) {
		while (jaw_check(i, j)) j = (j + 1) % sz;
		ld d = cross(H[i], H[(i + 1) % sz], H[j]) / (H[i] - H[(i + 1) % sz]).mag();
		d *= .5;
		if (D > d) {
			D = d;
			Pii s = H[i], e = H[(i + 1) % sz];
			Pii vec = e - s;
			X_ = -vec.y;
			Y_ = vec.x;
			Pos v = ~conv(vec);
			Pos m = (conv(s) + conv(H[j])) * .5;
			Z_ = round(v * m);
		}
	}
	//circle
	//std::cout << "circle::\n";
	Polygon C;
	for (Pii& p : Cii) C.push_back(conv(p));
	Delaunator DTR(C);
	for (int i = 0; i < DTR.triangles_.size(); i += 3) {
		const int& a = DTR.points_[DTR.triangles_[i]].i;
		const int& b = DTR.points_[DTR.triangles_[i + 1]].i;
		const int& c = DTR.points_[DTR.triangles_[i + 2]].i;
		DT[a].push_back(b); DT[a].push_back(c);
		DT[b].push_back(a); DT[b].push_back(c);
		DT[c].push_back(a); DT[c].push_back(b);
	}
	for (int c = 0; c < N; c++) {
		Vint& I = DT[c];
		std::sort(I.begin(), I.end());
		I.erase(unique(I.begin(), I.end()), I.end());
		Pii sd = Cii[c];//seed
		std::vector<Linear> HP;
		init(HP, 5e9);
		for (const int& i : I) {
			Pii p = Cii[i];
			Pii vec = p - sd;
			ll g = gcd(std::abs(vec.x), std::abs(vec.y));
			vec /= g;
			Pos v = ~conv(vec);
			Pos s = conv(sd), e = conv(p);
			//Pos v = ~(e - s);
			Pos m = (s + e) * .5;
			HP.push_back(Linear(m, m + v));
		}
		VHP hpi = half_plane_intersection(HP);
		//std::cout << "hpi_p.sz:: " << hpi.size() << "\n";
		if (hpi.size() < 3) continue;
		VDP[c] = hpi;
	}
	for (const Pii& q : H) {
		int c = q.i;
		VHP hpi = get_cell(C, q.i, -1);
		//std::cout << "hpi_n.sz:: " << hpi.size() << "\n";
		if (hpi.size() < 3) continue;
		VDN[c] = hpi;
		NG.push_back(q.i);
	}
	for (int pv = 0; pv < N; pv++) {
		if (!VDP[pv].size()) continue;
		Pii p_ = Cii[pv];
		Pos sdp = C[pv];//seed
		for (const int& ng : NG) {
			if (!VDN[ng].size()) continue;
			VHP inx = half_plane_intersection(VDP[pv], VDN[ng]);
			int sz = inx.size();
			if (sz < 3) continue;
			Pos sdn = C[ng];//seed
			for (int i = 0; i < sz; i++) {
				int j = (i + 1) % sz;
				const Linear& l0 = inx[i];
				const Linear& l1 = inx[j];
				//std::cout << "l[0]:: " << l0[0] << " ";
				//std::cout << "l[1]:: " << l0[1] << "\n";
				//std::cout << "l[0]:: " << l1[0] << " ";
				//std::cout << "l[1]:: " << l1[1] << "\n";
				if (!ccw(l0[0], l0[1], l1[0], l1[1])) continue;
				Pos ix = intersection(l0, l1);
				ld dn = (sdn - ix).mag();
				ld dp = (sdp - ix).mag();
				ld d = dn - dp;
				d *= .5;
				if (D > d) {
					D = d;
					R_ = (dn + dp) * .5;
					update_XYZ(l0, l1);
					LF = 0;
				}
			}
		}
	}
	//result
	if (zero(D)) D = +0;
	if (zero(R_)) R_ = +0;
	ll g = gcd(X_, Y_, Z_);
	X_ /= g; Y_ /= g; Z_ /= g;
	if (LF) std::cout << D << "\nL " << X_ << " " << Y_ << " " << Z_ << "\n";
	else std::cout << D << "\nC " << X_ << " " << Y_ << " " << Z_ << " " << R_ << "\n";
	return;
}
int main() { solve(); return 0; }//boj33390 Keychain refer to Crysfly from QOJ

*/