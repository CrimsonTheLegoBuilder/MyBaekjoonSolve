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
ll V[LEN];
bool zero(const db& x) { return std::abs(x) < TOL; }
int sign(const db& x) { return x < -TOL ? -1 : x > TOL; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

/*

tested in range -1e5 < x, y < 1e5;
Delaunator - https://github.com/abellgithub/delaunator-cpp/blob/master/include/delaunator.cpp
modify : jinhwanlazy
I'm : stupid

*/

struct Pii {
    int x, y, i;
    Pii(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
    bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
    bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
    bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
    bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
    Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
    Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
    Pii operator * (const int& n) const { return { x * n, y * n }; }
    Pii operator / (const int& n) const { return { x / n, y / n }; }
    ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
    ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
    Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
    Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
    Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
    Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
    Pii operator ~ () const { return { -y, x }; }
    Pii operator ! () const { return { -x, -y }; }
    ll xy() const { return (ll)x * y; }
    ll Euc() const { return (ll)x * x + (ll)y * y; }
    int Man() const { return std::abs(x) + std::abs(y); }
    ld mag() const { return hypot(x, y); }
    friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
    friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (int)INF, (int)INF };
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
    ll ret = cross(d1, d2, d3);
    return !ret ? 0 : ret > 0 ? 1 : -1;
}
struct Pos {
    db x, y, i;
    Pos(db X = 0, db Y = 0, int I = 0) : x(X), y(Y), i(I) {}
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
        Pos pPrev{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN() };

        // Go through points based on distance from the center.
        for (std::size_t k = 0; k < n; k++) {
            const std::size_t i = ids[k];
            const Pos p = points_[i];

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
void query() {
    std::cin >> N;
    std::vector<Pii> tmp(N);
    std::vector<Pos> C(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> tmp[i];
        tmp[i].i = i;
        C[i] = Pos(tmp[i].x, tmp[i].y, i);
        V[i] = (ll)INF;
    }
    if (N == 2) {
        ll D = (tmp[0] - tmp[1]).Euc();
        std::cout << D << "\n" << D << "\n";
        return;
    }
    Delaunator d(C);
    for (int i = 0; i < d.triangles_.size(); i += 3) {
        const Pii& a = tmp[d.points_[d.triangles_[i]].i];
        const Pii& b = tmp[d.points_[d.triangles_[i + 1]].i];
        const Pii& c = tmp[d.points_[d.triangles_[i + 2]].i];
        V[a.i] = std::min({ V[a.i], (a - b).Euc(), (a - c).Euc() });
        V[b.i] = std::min({ V[b.i], (b - a).Euc(), (b - c).Euc() });
        V[c.i] = std::min({ V[c.i], (c - a).Euc(), (c - b).Euc() });
    }
    for (int i = 0; i < N; i++) std::cout << V[i] << "\n";
    return;
}
void solve() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cout.tie(0);
    std::cout << std::fixed;
    std::cout.precision(10);
    std::cin >> T;
    while (T--) query();
    return;
}
int main() { solve(); return 0; }//In case of failure


/*

#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
typedef long long ll;
typedef double ld;
const int LEN = 1e5 + 5;
const int INF = 1e9;
const ll LINF = 1e18;
int N, T;

/*

2-Dimensional coord' sort tree
https://github.com/koosaga/olympiad/blob/master/ICPC/CERC/cerc2008_j.cpp

Because of the memory limit exceed, studied the segtree referring to Koosaga's code

*/
ll sqr(const int& a) { return (ll)a * a; }
struct Pos {
    int x, y, i;
    Pos(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
    bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
    bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
    bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
    bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
    Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
    Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
    Pos operator * (const int& n) const { return { x * n, y * n }; }
    Pos operator / (const int& n) const { return { x / n, y / n }; }
    ll operator * (const Pos& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
    ll operator / (const Pos& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
    Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
    Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
    Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
    Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
    Pos operator ~ () const { return { -y, x }; }
    ll operator ! () const { return (ll)x * y; }
    ll Euc() const { return (ll)x * x + (ll)y * y; }
    ll Man() const { return std::abs(x) + std::abs(y); }
    ld mag() const { return hypot(x, y); }
    friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
    friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pos[LEN];
const Pos O = Pos(0, 0);
std::vector<Pos> C, H;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
    ll ret = cross(d1, d2, d3);
    return !ret ? 0 : ret > 0 ? 1 : -1;
}
struct Node {
    Pos p;//mid point
    bool spl;//dx < dy ?
    int sx, ex, sy, ey;
    Node(Pos P = Pos(0, 0), bool SPL = 0, int SX = 0, int EX = 0, int SY = 0, int EY = 0) :
        p(P), spl(SPL), sx(SX), ex(EX), sy(SY), ey(EY) {
    }
} tree[LEN << 2];//segment tree
bool V[LEN << 2];//visited
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
void init(int s = 0, int e = N - 1, int n = 1) {//divide & conquer
    int MAXX = -INF, MAXY = -INF, MINX = INF, MINY = INF;
    int m = s + e >> 1;
    for (int i = s; i <= e; i++) {
        MAXX = std::max(MAXX, pos[i].x);
        MAXY = std::max(MAXY, pos[i].y);
        MINX = std::min(MINX, pos[i].x);
        MINY = std::min(MINY, pos[i].y);
    }
    tree[n].spl = (MAXX - MINX) < (MAXY - MINY);//slope cmp :: dx < dy
    if (tree[n].spl) std::sort(pos + s, pos + e + 1, cmpy);//if dy is dominant, sort by y
    else std::sort(pos + s, pos + e + 1, cmpx);//if dx is dominant, sort by x
    V[n] = 1;
    tree[n] = Node(pos[m], tree[n].spl, MINX, MAXX, MINY, MAXY);
    if (s <= m - 1) init(s, m - 1, n << 1);
    if (m + 1 <= e) init(m + 1, e, n << 1 | 1);
    return;
}
ll search(const Pos& t, ll X = LINF, int n = 1) {//divide & conquer | refer to koosaga
    ll S = LINF, A = LINF;
    if (t != tree[n].p) S = std::min(X, (t - tree[n].p).Euc());
    if (tree[n].spl) {//if dy is dominant
        if (!cmpy(tree[n].p, t)) {//p.y >= t.y
            if (V[n << 1]) A = search(t, S, n << 1);//L search
            S = std::min(S, A);
            if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sy - t.y) < S) A = search(t, S, n << 1 | 1);
            //if R < MIN, execute R search
        }
        else {//p.y < t.y
            if (V[n << 1 | 1]) A = search(t, S, n << 1 | 1);//R search
            S = std::min(S, A);
            if (V[n << 1] && sqr(tree[n << 1].ey - t.y) < S) A = search(t, S, n << 1);
            //if L < MIN, execute L search
        }
    }
    else {//if dx is dominant
        if (!cmpx(tree[n].p, t)) {//p.x >= t.x
            if (V[n << 1]) A = search(t, S, n << 1);
            S = std::min(S, A);
            if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sx - t.x) < S) A = search(t, S, n << 1 | 1);
        }
        else {//p.x < t.x
            if (V[n << 1 | 1]) A = search(t, S, n << 1 | 1);
            S = std::min(S, A);
            if (V[n << 1] && sqr(tree[n << 1].ex - t.x) < S) A = search(t, S, n << 1);
        }
    }
    return std::min(S, A);
}
void solve() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cout.tie(0);
    std::cin >> T;
    while (T--) {
        std::cin >> N;
        memset(V, 0, sizeof V);
        for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].i = i;
        init();
        std::sort(pos, pos + N, cmpi);
        for (int i = 0; i < N; i++) std::cout << search(pos[i]) << "\n";
    }
    return;
}
int main() { solve(); return 0; }//boj7890 CERC 2008 J In case of failure


////fucking memory limit exceed
//
//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <cstring>
//#include <cassert>
//#include <cmath>
//#include <vector>
//#include <queue>
//#include <deque>
//#include <tuple>
//typedef long long ll;
//typedef double ld;
//typedef long double lld;
//const ld INF = 1e18;
//const ld TOL = 1e-7;
//const ld PI = acos(-1);
//const int LEN = 1e5;
//int N, M, T, Q;
//ll V[LEN];
//bool zero(const ld& x) { return std::abs(x) < TOL; }
//int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
//int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
//ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
//
////O(N log N) Delaunay_triangulation && Voronoi_diagram solver
////https://cp-algorithms.com/geometry/delaunay.html
//struct Pii {
//	int x, y, i;
//	Pii(int X = 0, int Y = 0, int I = 0) : x(X), y(Y), i(I) {}
//	bool operator == (const Pii& p) const { return x == p.x && y == p.y; }
//	bool operator != (const Pii& p) const { return x != p.x || y != p.y; }
//	bool operator < (const Pii& p) const { return x == p.x ? y < p.y : x < p.x; }
//	bool operator <= (const Pii& p) const { return x == p.x ? y <= p.y : x <= p.x; }
//	Pii operator + (const Pii& p) const { return { x + p.x, y + p.y }; }
//	Pii operator - (const Pii& p) const { return { x - p.x, y - p.y }; }
//	Pii operator * (const int& n) const { return { x * n, y * n }; }
//	Pii operator / (const int& n) const { return { x / n, y / n }; }
//	ll operator * (const Pii& p) const { return { (ll)x * p.x + (ll)y * p.y }; }
//	ll operator / (const Pii& p) const { return { (ll)x * p.y - (ll)y * p.x }; }
//	Pii& operator += (const Pii& p) { x += p.x; y += p.y; return *this; }
//	Pii& operator -= (const Pii& p) { x -= p.x; y -= p.y; return *this; }
//	Pii& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
//	Pii& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
//	Pii operator ~ () const { return { -y, x }; }
//	ll operator ! () const { return (ll)x * y; }
//	ll Euc() const { return (ll)x * x + (ll)y * y; }
//	int Man() const { return std::abs(x) + std::abs(y); }
//	ld mag() const { return hypot(x, y); }
//	friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
//	friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
//};
//const Pii Oii = { 0, 0 };
//const Pii INF_PT = { (int)INF, (int)INF };
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
//ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
//ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
//int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
//	ll ret = cross(d1, d2, d3);
//	return !ret ? 0 : ret > 0 ? 1 : -1;
//}
//struct QuadEdge {
//	Pii origin;
//	QuadEdge* rot = nullptr;
//	QuadEdge* onext = nullptr;
//	bool used = 0;
//	QuadEdge* rev() const { return rot->rot; }//reverse
//	QuadEdge* lnext() const { return rot->rev()->onext->rot; }
//	QuadEdge* oprev() const { return rot->onext->rot; }
//	Pii dest() const { return rev()->origin; }//destination
//};
//QuadEdge* make_edge(Pii from, Pii to) {
//	QuadEdge* e1 = new QuadEdge;
//	QuadEdge* e2 = new QuadEdge;
//	QuadEdge* e3 = new QuadEdge;
//	QuadEdge* e4 = new QuadEdge;
//	e1->origin = from;
//	e2->origin = to;
//	e3->origin = e4->origin = INF_PT;
//	e1->rot = e3;
//	e2->rot = e4;
//	e3->rot = e2;
//	e4->rot = e1;
//	e1->onext = e1;
//	e2->onext = e2;
//	e3->onext = e4;
//	e4->onext = e3;
//	return e1;
//}
//void splice(QuadEdge* a, QuadEdge* b) {
//	std::swap(a->onext->rot->onext, b->onext->rot->onext);
//	std::swap(a->onext, b->onext);
//}
//void delete_edge(QuadEdge* e) {
//	splice(e, e->oprev());
//	splice(e->rev(), e->rev()->oprev());
//	delete e->rev()->rot;
//	delete e->rev();
//	delete e->rot;
//	delete e;
//}
//QuadEdge* connect(QuadEdge* a, QuadEdge* b) {
//	QuadEdge* e = make_edge(a->dest(), b->origin);
//	splice(e, a->lnext());
//	splice(e->rev(), b);
//	return e;
//}
//bool left_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) > 0; }
//bool right_of(Pii p, QuadEdge* e) { return cross(p, e->origin, e->dest()) < 0; }
//template <class T> T det3(T a1, T a2, T a3, T b1, T b2, T b3, T c1, T c2, T c3) {
//	return a1 * (b2 * c3 - c2 * b3)
//		- a2 * (b1 * c3 - c1 * b3)
//		+ a3 * (b1 * c2 - c1 * b2);
//}
//bool in_circle(Pii a, Pii b, Pii c, Pii d) {
//	lld det = -det3<lld>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//	det -= det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//	det += det3<lld>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//	if (fabs(det) > 1e18) return det > 0;//overflow prevention (refer to koosaga)
//	else {
//		ll det = -det3<ll>(b.x, b.y, b.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), c.x, c.y, c.Euc(), d.x, d.y, d.Euc());
//		det -= det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), d.x, d.y, d.Euc());
//		det += det3<ll>(a.x, a.y, a.Euc(), b.x, b.y, b.Euc(), c.x, c.y, c.Euc());
//		return det > 0;
//	}
//}
//std::pair<QuadEdge*, QuadEdge*> build_tr(int l, int r, std::vector<Pii>& C) {
//	if (r - l + 1 == 2) {
//		QuadEdge* res = make_edge(C[l], C[r]);
//		return std::make_pair(res, res->rev());
//	}
//	if (r - l + 1 == 3) {
//		QuadEdge* a = make_edge(C[l], C[l + 1]), * b = make_edge(C[l + 1], C[r]);
//		splice(a->rev(), b);
//		int sign = ccw(C[l], C[l + 1], C[r]);
//		if (!sign) return std::make_pair(a, b->rev());
//		QuadEdge* c = connect(b, a);
//		if (sign == 1) return std::make_pair(a, b->rev());
//		else return std::make_pair(c->rev(), c);
//	}
//	int m = l + r >> 1;
//	QuadEdge* ldo, * ldi, * rdo, * rdi;
//	std::tie(ldo, ldi) = build_tr(l, m, C);
//	std::tie(rdi, rdo) = build_tr(m + 1, r, C);
//	while (1) {
//		if (left_of(rdi->origin, ldi)) {
//			ldi = ldi->lnext();
//			continue;
//		}
//		if (right_of(ldi->origin, rdi)) {
//			rdi = rdi->rev()->onext;//rnext
//			continue;
//		}
//		break;
//	}
//	QuadEdge* basel = connect(rdi->rev(), ldi);
//	auto valid = [&basel](QuadEdge* e) { return right_of(e->dest(), basel); };
//	if (ldi->origin == ldo->origin) ldo = basel->rev();
//	if (rdi->origin == rdo->origin) rdo = basel;
//	while (1) {
//		QuadEdge* lcand = basel->rev()->onext;
//		if (valid(lcand)) {
//			while (in_circle(basel->dest(), basel->origin, lcand->dest(), lcand->onext->dest())) {
//				QuadEdge* t = lcand->onext;
//				delete_edge(lcand);
//				lcand = t;
//			}
//		}
//		QuadEdge* rcand = basel->oprev();
//		if (valid(rcand)) {
//			while (in_circle(basel->dest(), basel->origin, rcand->dest(), rcand->oprev()->dest())) {
//				QuadEdge* t = rcand->oprev();
//				delete_edge(rcand);
//				rcand = t;
//			}
//		}
//		if (!valid(lcand) && !valid(rcand)) break;
//		if (!valid(lcand) || (valid(rcand) && in_circle(lcand->dest(), lcand->origin, rcand->origin, rcand->dest())))
//			basel = connect(rcand, basel->rev());
//		else
//			basel = connect(basel->rev(), lcand->rev());
//	}
//	return std::make_pair(ldo, rdo);
//}
////std::vector<std::tuple<Pii, Pii, Pii>> Delaunay_triangulation(std::vector<Pii> C) {
//void Delaunay_triangulation(std::vector<Pii>& C) {
//	std::sort(C.begin(), C.end());
//	int sz = C.size();
//	auto ret = build_tr(0, sz - 1, C);
//	QuadEdge* e = ret.first;
//	std::vector<QuadEdge*> edges = { e };
//	while (cross(e->onext->dest(), e->dest(), e->origin) < 0) e = e->onext;
//	auto add = [&C, &e, &edges]() {
//		QuadEdge* cur = e;
//		do {
//			cur->used = 1;
//			C.push_back(cur->origin);
//			edges.push_back(cur->rev());
//			cur = cur->lnext();
//		} while (cur != e);
//		};
//	add();
//	C.clear();
//	int kek = 0;
//	while (kek < edges.size()) if (!(e = edges[kek++])->used) add();
//	sz = C.size();
//	//std::vector<std::tuple<Pii, Pii, Pii>> triangles;
//	//for (int i = 0; i < sz; i += 3) triangles.push_back(std::make_tuple(C[i], C[i + 1], C[i + 2]));
//	for (int i = 0; i < sz; i += 3) {
//		Pii a = C[i], b = C[i + 1], c = C[i + 2];
//		V[a.i] = std::min({ V[a.i], (a - b).Euc(), (a - b).Euc() });
//		V[b.i] = std::min({ V[b.i], (b - a).Euc(), (b - c).Euc() });
//		V[c.i] = std::min({ V[c.i], (c - a).Euc(), (c - b).Euc() });
//	}
//	//return triangles;
//	return;
//}
//std::vector<Pii> C;
////std::vector<int> seed[LEN];
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> T;
//	while (T--) {
//		std::cin >> N;
//		C.resize(N);
//		for (int i = 0; i < N; i++) std::cin >> C[i], C[i].i = i, V[i] = INF;
//		//assert(N);
//		if (N == 1) {}
//		else if (N == 2) { ll D = (C[0] - C[1]).Euc(); std::cout << D << "\n" << D << "\n"; }
//		else { Delaunay_triangulation(C); for (int i = 0; i < N; i++) std::cout << V[i] << "\n"; }
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj7890 CERC 2008 J In case of failure



//Pos Q[LEN];
//ll MIN = LINF;
//void solve(const Pos& t, int n = 1) {//divide & conquer | refer to koosaga
//	if (t != tree[n].p) MIN = std::min(MIN, (t - tree[n].p).Euc());
//	if (tree[n].spl) {//if dy is dominant
//		if (!cmpy(tree[n].p, t)) {//p.y >= t.y
//			if (V[n << 1]) solve(t, n << 1);//L search
//			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sy - t.y) < MIN)
//				solve(t, n << 1 | 1);//if L < MIN, execute R search
//		}
//		else {//p.y < t.y
//			if (V[n << 1 | 1]) solve(t, n << 1 | 1);
//			if (V[n << 1] && sqr(tree[n << 1].ey - t.y) < MIN) solve(t, n << 1);
//		}
//	}
//	else {//if dx is dominant
//		if (!cmpx(tree[n].p, t)) {//p.x >= t.x
//			if (V[n << 1]) solve(t, n << 1);
//			if (V[n << 1 | 1] && sqr(tree[n << 1 | 1].sx - t.x) < MIN) solve(t, n << 1 | 1);
//		}
//		else {//p.y < t.y
//			if (V[n << 1 | 1]) solve(t, n << 1 | 1);
//			if (V[n << 1] && sqr(tree[n << 1].ex - t.x) < MIN) solve(t, n << 1);
//		}
//	}
//	return;
//}
        //for (int i = 0; i < N; i++) {
        //	MIN = LINF;
        //	//solve(pos[i]);
        //	solve(Q[i]);
        //	std::cout << MIN << "\n";
        //}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> T;
//	while (T--) {
//		std::cin >> N;
//		memset(V, 0, sizeof V);
//		for (int i = 0; i < N; i++) std::cin >> pos[i], pos[i].i = i;// , Q[i] = pos[i];
//		init();
//		std::sort(pos, pos + N, cmpi);
//		for (int i = 0; i < N; i++) std::cout << search(pos[i]) << "\n";
//	}
//	return;
//}

*/