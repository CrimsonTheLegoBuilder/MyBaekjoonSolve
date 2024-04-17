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
typedef double db;
typedef long double ld;
const db INF = 1e18;
const db TOL = 1e-7;
const db PI = acos(-1);
const int LEN = 1e5 + 5;
int N, M, T, Q;
bool zero(const db& x) { return std::abs(x) < TOL; }
int sign(const db& x) { return x < -TOL ? -1 : x > TOL; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pos {
	db x, y;
	Pos(db X = 0, db Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const db& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const db& scalar) const { return { x / scalar, y / scalar }; }
	db operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
    db operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
    db operator ^ (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator - () const { return Pos(-x, -y); }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { -x, -y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const db& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const db& scale) { x /= scale; y /= scale; return *this; }
	db xy() const { return x * y; }
	Pos rot(db the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	db Euc() const { return x * x + y * y; }
	db mag() const { return sqrt(Euc()); }
	Pos unit() const { return *this / mag(); }
	db rad() const { return atan2(y, x); }
	friend db rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
    bool close(const Pos& p) const { return zero((*this - p).Euc()); }
    inline bool close(const Pos& rhs,
        const db span = 1.,
        const db tol = 1e-20) const {
        return ((*this - rhs).Euc() / span) < tol;
    }
    friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
db cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
db cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	db ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool counterclockwise(const Pos& p, const Pos& q, const Pos& r) {
    return ccw(p, q, r) == 1;
}
bool clockwise(const Pos& p, const Pos& q, const Pos& r) {
    return ccw(p, q, r) == -1;
}
bool collinear(const Pos& p, const Pos& q, const Pos& r) {
    return !ccw(p, q, r);
}
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return !ccw(d1, d2, d3) && !ccw(d1, d2, d4);
}
db dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
db dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	db ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	db ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
db projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
db dist(const Pos& d1, const Pos& d2, const Pos& t) {
	return cross(d1, d2, t) / (d1 - d2).mag();
}
Pos intersection(const Pos& p1, const Pos& p2, const Pos& q1, const Pos& q2) {
	db a1 = cross(q1, q2, p1), a2 = -cross(q1, q2, p2);
	return (p1 * a2 + p2 * a1) / (a1 + a2);
}
db circumradius(const Pos& p1, const Pos& p2, const Pos& p3) {
    Pos d = p2 - p1;
    Pos e = p3 - p1;

    const db bl = d.Euc();
    const db cl = e.Euc();
    const db det = d / e;

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

    const db bl = d.Euc();
    const db cl = e.Euc();
    const db det = d / e;

    Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
        (d.x * cl - e.x * bl) * 0.5 / det);
    return p1 + radius;
}
struct Circle {
	Pos c;
	db r;
	Circle(Pos C = Pos(0, 0), db R = 0) : c(C), r(R) {}
	bool operator == (const Circle& C) const { return c == C.c && std::abs(r - C.r) < TOL; }
	bool operator != (const Circle& C) const { return !(*this == C); }
	bool operator < (const Circle& q) const {
		db dist = (c - q.c).mag();
		return r < q.r && dist + r < q.r + TOL;
	}
	bool operator > (const Pos& p) const { return r > (c - p).mag(); }
	bool operator >= (const Pos& p) const { return r + TOL > (c - p).mag(); }
	bool operator < (const Pos& p) const { return r < (c - p).mag(); }
	Circle operator + (const Circle& C) const { return { c + C.c, r + C.r }; }
	Circle operator - (const Circle& C) const { return { c - C.c, r - C.r }; }
	db H(const db& th) const { return sin(th) * c.x + cos(th) * c.y + r; }//coord trans | check right
	db A() const { return 1. * r * r * PI; }
	friend std::istream& operator >> (std::istream& is, Circle& c) { is >> c.c >> c.r; return is; }
	friend std::ostream& operator << (std::ostream& os, const Circle& c) { os << c.c << " " << c.r; return os; }
} INVAL = { { 0, 0 }, (std::numeric_limits<db>::max)() };
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

    const db bl = d.Euc();
    const db cl = e.Euc();
    const db det = d / e;

    Pos radius((e.y * bl - d.y * cl) * 0.5 / det,
        (d.x * cl - e.x * bl) * 0.5 / det);

    Pos c;
    db r;
    if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
        (det > 0.0 || det < 0.0))
        c = p1 + radius, r = radius.Euc();
    else c = Pos(INF, INF), r = std::numeric_limits<db>::max();

    return Circle(c, r);
}
using namespace std;

class Vec2 {
public:
    using T = double;
    T x;
    T y;

public:
    inline Vec2& operator=(const Vec2& rhs) { x = rhs.x; y = rhs.y; return *this; }

public:
    inline bool operator==(const Vec2& rhs) const { return ((x == rhs.x) && (y == rhs.y)); }
    inline bool operator!=(const Vec2& rhs) const { return !(operator==(rhs)); }
    inline Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    inline Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    inline Vec2 operator*(T rhs) const { return Vec2(T(x * rhs), T(y * rhs)); }
    inline Vec2 operator/(T rhs) const { return Vec2(T(x / rhs), T(y / rhs)); }
    inline Vec2 operator-() const { return Vec2(-x, -y); }
    inline Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    inline Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    inline Vec2& operator*= (T rhs) { x = T(x * rhs); y = T(y * rhs); return *this; }
    inline Vec2& operator/= (T rhs) { x = T(x / rhs); y = T(y / rhs); return *this; }

    inline double norm() const { return sqrt(squaredNorm()); }
    inline T squaredNorm() const { return ((x * x) + (y * y)); }
    inline T norm1() const { return abs(x) + abs(y); }
    inline void normalize()
    {
        double l = norm();
        if (l < numeric_limits<double>::epsilon()) {
            x = 0;
            y = 0;
        }
        else {
            x = (T)(x / l);
            y = (T)(y / l);
        }
    }

    inline Vec2 normalized() const
    {
        Vec2 n(*this);
        n.normalize();
        return n;
    }

    inline bool close(const Vec2& rhs,
        const double span = 1.0,
        const double tol = 1e-20) const {
        return ((*this - rhs).squaredNorm() / span) < tol;
    }

    inline T dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }
    inline T cross(const Vec2& rhs) const { return x * rhs.y - y * rhs.x; }
    inline double angle(const Vec2& rhs) const
    {
        double sqlen = squaredNorm();
        double sqlen2 = rhs.squaredNorm();

        if (sqlen == 0.0 || sqlen2 == 0.0) {
            return 0.0;
        }

        double val = dot(rhs) / sqrt(sqlen) / sqrt(sqlen2);
        val = std::max(-1.0, min(val, 1.0));
        return acos(val);
    }

public:
    static const Vec2& xAxis() { static Vec2 vec(1, 0); return vec; }
    static const Vec2& yAxis() { static Vec2 vec(0, 1); return vec; }
    static const Vec2& zero() { static Vec2 vec(0, 0); return vec; }
    friend ostream& operator<<(ostream& ostr, const Vec2& rhs) {
        ostr << rhs.x << " " << rhs.y;
        return ostr;
    }

    friend istream& operator>>(istream& istr, Vec2& rhs) {
        istr >> rhs.x >> rhs.y;
        return istr;
    }

    friend bool operator<(const Vec2& lhs, const Vec2& rhs) {
        return lhs.x != rhs.x ? lhs.x < rhs.x : lhs.y < rhs.y;
    }

public:
    Vec2() : x(T(0)), y(T(0)) {}

    Vec2(T x_, T y_) : x(x_), y(y_) {}

    Vec2(const Vec2& rhs) : x(rhs.x), y(rhs.y) {}

    ~Vec2() {}


};

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
static bool onSegment(const Vec2 p, const Vec2 q, const Vec2 r) {
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&  //
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// +1 --> Clockwise
// -1 --> Counterclockwise
inline int orientation(const Vec2& p, const Vec2& q, const Vec2& r) {
    using T = Vec2::T;
    T val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) {
        return 0;
    }
    return val > 0 ? 1 : -1;
}

inline bool clockwise(const Vec2& p, const Vec2& q, const Vec2& r) {
    return orientation(p, q, r) == 1;
}

inline bool counterclockwise(const Vec2& p, const Vec2& q, const Vec2& r) {
    return orientation(p, q, r) == -1;
}

inline bool colinear(const Vec2& p, const Vec2& q, const Vec2& r) {
    return orientation(p, q, r) == 0;
}

inline double circumradius(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
    Vec2 d = p2 - p1;
    Vec2 e = p3 - p1;

    const double bl = d.squaredNorm();
    const double cl = e.squaredNorm();
    const double det = d.cross(e);

    Vec2 radius((e.y * bl - d.y * cl) * 0.5 / det,
        (d.x * cl - e.x * bl) * 0.5 / det);

    if ((bl > 0.0 || bl < 0.0) && (cl > 0.0 || cl < 0.0) &&
        (det > 0.0 || det < 0.0))
        return radius.squaredNorm();
    return (std::numeric_limits<double>::max)();
}

inline Vec2 circumcenter(const Vec2& a, const Vec2& b, const Vec2& c) {
    const Vec2 d = b - a;
    const Vec2 e = c - a;

    const double bl = d.squaredNorm();
    const double cl = e.squaredNorm();
    const double det = d.cross(e);

    Vec2 radiusVec((e.y * bl - d.y * cl) * 0.5 / det, (d.x * cl - e.x * bl) * 0.5 / det);
    return a + radiusVec;
}

inline bool inCircle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p) {
    const Vec2 d = a - p;
    const Vec2 e = b - p;
    const Vec2 f = c - p;

    const double ap = d.squaredNorm();
    const double bp = e.squaredNorm();
    const double cp = f.squaredNorm();

    return d.cross(e * cp - f * bp) + ap * e.cross(f) < 0.0;
}

class BBox2 {
    constexpr static auto INF = std::numeric_limits<Vec2::T>::max();
private:
    Vec2 bottomLeft_;
    Vec2 topRight_;
    Vec2 center_;
    double span_;

public:
    BBox2(const vector<Vec2>& points) {
        topRight_ = Vec2(-INF, -INF);
        bottomLeft_ = Vec2(INF, INF);
        for (const auto& p : points) {
            bottomLeft_.x = min(bottomLeft_.x, p.x);
            bottomLeft_.y = min(bottomLeft_.y, p.y);
            topRight_.x = max(topRight_.x, p.x);
            topRight_.y = max(topRight_.y, p.y);
        }
        center_ = (bottomLeft_ + topRight_) / 2;
        span_ = (bottomLeft_ - topRight_).squaredNorm();
    }

    const Vec2& bottomLeft() const { return bottomLeft_; }
    const Vec2& topRight() const { return topRight_; }
    const Vec2& center() const { return center_; }
    const double& span() const { return span_; }
};

// Delaunator, mostly adopted from
// https://github.com/abellgithub/delaunator-cpp/blob/master/include/delaunator.cpp
class Delaunator {
public:
    constexpr static auto INF = std::numeric_limits<Vec2::T>::max();

    std::vector<Vec2> points_;
    std::vector<std::size_t> triangles_;
    std::vector<std::size_t> halfedges_;
    std::vector<std::size_t> hullPrev_;
    std::vector<std::size_t> hullNext_;
    std::vector<std::size_t> hullTri_;
    std::size_t hullStart_;

private:
    static constexpr std::size_t INVALID_INDEX = -1;

    std::vector<std::size_t> hullHash_;
    Vec2 center_;
    std::size_t hashSize_;

public:
    Delaunator(std::vector<Vec2> const& points) : points_(points) {
        std::size_t n = points.size();

        BBox2 bbox(points_);
        Vec2 center = bbox.center();

        std::size_t i0 = INVALID_INDEX;
        std::size_t i1 = INVALID_INDEX;
        std::size_t i2 = INVALID_INDEX;

        double min_dist = INF;
        for (size_t i = 0; i < points_.size(); ++i)
        {
            const Vec2& p = points_[i];
            const double d = (p - center).squaredNorm();
            if (d < min_dist) {
                i0 = i;
                min_dist = d;
            }
        }
        Vec2 p0 = points_[i0];

        min_dist = (std::numeric_limits<double>::max)();
        for (std::size_t i = 0; i < n; i++) {
            if (i == i0) continue;
            const double d = (p0 - points_[i]).squaredNorm();
            if (d < min_dist && d > 0.0) {
                i1 = i;
                min_dist = d;
            }
        }
        Vec2 p1 = points_[i1];

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
        Vec2 p2 = points_[i2];

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
            dists.push_back((p - center_).squaredNorm());

        // sort the points by distance from the seed triangle circumcenter
        std::vector<std::size_t> ids(n);
        std::iota(ids.begin(), ids.end(), 0);
        std::sort(ids.begin(), ids.end(),
            [&dists](std::size_t i, std::size_t j) { return dists[i] < dists[j]; });

        // initialize a hash table for storing edges of the advancing convex hull
        hashSize_ = static_cast<std::size_t>(std::ceil(std::sqrt(n)));
        hullHash_.resize(hashSize_, INVALID_INDEX);

        // initialize arrays for tracking the edges of the advancing convex hull
        hullPrev_.resize(n);
        hullNext_.resize(n);
        hullTri_.resize(n);

        hullStart_ = i0;

        size_t hull_size = 3;

        hullNext_[i0] = hullPrev_[i2] = i1;
        hullNext_[i1] = hullPrev_[i0] = i2;
        hullNext_[i2] = hullPrev_[i1] = i0;

        hullTri_[i0] = 0;
        hullTri_[i1] = 1;
        hullTri_[i2] = 2;

        hullHash_[hash_key(p0)] = i0;
        hullHash_[hash_key(p1)] = i1;
        hullHash_[hash_key(p2)] = i2;

        std::size_t max_triangles_ = n < 3 ? 1 : 2 * n - 5;
        triangles_.reserve(max_triangles_ * 3);
        halfedges_.reserve(max_triangles_ * 3);
        add_triangle(i0, i1, i2, INVALID_INDEX, INVALID_INDEX, INVALID_INDEX);
        Vec2 pPrev{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() };

        // Go through points based on distance from the center.
        for (std::size_t k = 0; k < n; k++) {
            const std::size_t i = ids[k];
            const Vec2 p = points_[i];

            // skip near-duplicate points
            if (k > 0 && p == pPrev)
                continue;
            pPrev = p;

            if (p == p0 || p == p1 || p == p2) {
                continue;
            }

            // find a visible edge on the convex hull using edge hash
            std::size_t start = 0;

            size_t key = hash_key(p);
            for (size_t j = 0; j < hashSize_; j++) {
                start = hullHash_[(key + j) % hashSize_];
                if (start != INVALID_INDEX && start != hullNext_[start])
                    break;
            }

            assert(hullPrev_[start] != start);
            assert(hullPrev_[start] != INVALID_INDEX);

            start = hullPrev_[start];
            size_t e = start;
            size_t q;

            // Advance until we find a place in the hull where our current point can be added.
            while (true) {
                q = hullNext_[e];
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
            std::size_t t = add_triangle(e, i, hullNext_[e], INVALID_INDEX, INVALID_INDEX, hullTri_[e]);

            hullTri_[i] = legalize(t + 2);  // Legalize the triangle we just added.
            hullTri_[e] = t;
            hull_size++;

            // walk forward through the hull, adding more triangles_ and flipping recursively
            std::size_t next = hullNext_[e];
            while (true) {
                q = hullNext_[next];
                if (!counterclockwise(p, points_[next], points_[q]))
                    break;
                t = add_triangle(next, i, q, hullTri_[i], INVALID_INDEX, hullTri_[next]);
                hullTri_[i] = legalize(t + 2);
                hullNext_[next] = next;  // mark as removed
                hull_size--;
                next = q;
            }

            // walk backward from the other side, adding more triangles_ and flipping
            if (e == start) {
                while (true) {
                    q = hullPrev_[e];
                    if (!counterclockwise(p, points_[q], points_[e]))
                        break;
                    t = add_triangle(q, i, e, INVALID_INDEX, hullTri_[e], hullTri_[q]);
                    legalize(t + 2);
                    hullTri_[q] = t;
                    hullNext_[e] = e;  // mark as removed
                    hull_size--;
                    e = q;
                }
            }

            // update the hull indices
            hullPrev_[i] = e;
            hullStart_ = e;
            hullPrev_[next] = i;
            hullNext_[e] = i;
            hullNext_[i] = next;

            hullHash_[hash_key(p)] = i;
            hullHash_[hash_key(points_[e])] = e;
        }
    }

private:
    std::size_t legalize(std::size_t a) {
        std::size_t i = 0;
        std::size_t ar = 0;
        std::vector<std::size_t> edgesStack;

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
                    a = edgesStack[i];
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

            const bool illegal = inCircle(points_[p0], points_[pr], points_[pl], points_[p1]);

            if (illegal) {
                triangles_[a] = p1;
                triangles_[b] = p0;

                auto hbl = halfedges_[bl];

                // Edge swapped on the other side of the hull (rare).
                // Fix the halfedge reference
                if (hbl == INVALID_INDEX) {
                    std::size_t e = hullStart_;
                    do {
                        if (hullTri_[e] == bl) {
                            hullTri_[e] = a;
                            break;
                        }
                        e = hullPrev_[e];
                    } while (e != hullStart_);
                }
                link(a, hbl);
                link(b, halfedges_[ar]);
                link(ar, bl);
                std::size_t br = b0 + (b + 1) % 3;

                if (i < edgesStack.size()) {
                    edgesStack[i] = br;
                }
                else {
                    edgesStack.push_back(br);
                }
                i++;

            }
            else {
                if (i > 0) {
                    i--;
                    a = edgesStack[i];
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
        size_t key = std::llround(std::floor(pseudo_angle(dx, dy) * static_cast<double>(hashSize_)));
        return key % hashSize_;
    };

    std::size_t hash_key(const Vec2& p) const {
        const Vec2 v = p - center_;
        size_t key = std::llround(std::floor(pseudo_angle(v.x, v.y) * static_cast<double>(hashSize_)));
        return key % hashSize_;
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

void solve() {
    int N;
    cin >> N;

    vector<Vec2> ts(N);
    for (int i = 0; i < N; ++i) {
        cin >> ts[i];
    }

    double ans = 0;
    Delaunator d(ts);
    for (int i = 0; i < d.triangles_.size(); i += 3) {
        const Vec2& p1 = d.points_[d.triangles_[i]];
        const Vec2& p2 = d.points_[d.triangles_[i + 1]];
        const Vec2& p3 = d.points_[d.triangles_[i + 2]];
        //cout << p1 << ", " << p2 << ", " << p3 << " -> " << circumradius(p1, p2, p3) << "\n";

        double r = circumradius(p1, p2, p3);
        ans = max(r, ans);
    }
    cout << sqrt(ans) << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL); cout.tie(NULL);

    int T = 1;
    while (T--) {
        solve();
    }

    return 0;
}//jinhwanlazy
