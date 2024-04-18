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
#include <queue>
#include <numeric>
#include <map>
typedef long long ll;
typedef double db;
typedef long double ld;
const db INF = 1e18;
const db TOL = 1e-7;
const db PI = acos(-1);
const int LEN = 1e5 + 5;
int N, M, T, Q;
ll V[LEN];
int P[LEN << 3];
bool zero(const db& x) { return std::abs(x) < TOL; }
int sign(const db& x) { return x < -TOL ? -1 : x > TOL; }
db sqr(const db& x) { return x * x; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }
db sqr(db x) { return x * x; }

struct Info {
    int u, v;
    db c;
    Info(int u = 0, int v = 0, db c = 0) : u(u), v(v), c(c) {}
    bool operator < (const Info& x) const { return c > x.c; }
};
int find(int v) {
    if (P[v] < 0) return v;
    int p = find(P[v]);
    return P[v] = p;
}
int join(int u, int v) {
    int i, j;
    i = find(u);
    j = find(v);
    if (i == j) return 0;
    if (P[i] < P[j]) {
        P[i] += P[j];
        P[j] = i;
    }
    else {
        P[j] += P[i];
        P[i] = j;
    }
    return 1;
}
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
    Pii operator - () const { return { -x, -y }; }
    Pii operator ~ () const { return { -y, x }; }
    Pii operator ! () const { return { y, x, 0 }; }
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
bool inner_check(const Pos& a, const Pos& b, const Pos& c, const Pos& t) {
    return ccw(a, b, t) > -1 && ccw(b, c, t) > -1 && ccw(c, a, t) > -1;
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

//fast_delaunay / 
inline double sqr(double x) { return x * x; }

double dist_sqr(Pos const& a, Pos const& b) {
    return sqr(a.x - b.x) + sqr(a.y - b.y);
}

bool in_circumcircle(Point const& p1, Point const& p2, Point const& p3, Point const& p4) {
    double u11 = p1.x - p4.x;
    double u21 = p2.x - p4.x;
    double u31 = p3.x - p4.x;
    double u12 = p1.y - p4.y;
    double u22 = p2.y - p4.y;
    double u32 = p3.y - p4.y;
    double u13 = sqr(p1.x) - sqr(p4.x) + sqr(p1.y) - sqr(p4.y);
    double u23 = sqr(p2.x) - sqr(p4.x) + sqr(p2.y) - sqr(p4.y);
    double u33 = sqr(p3.x) - sqr(p4.x) + sqr(p3.y) - sqr(p4.y);
    double det = -u13 * u22 * u31 + u12 * u23 * u31 + u13 * u21 * u32 - u11 * u23 * u32 - u12 * u21 * u33 + u11 * u22 * u33;
    return det > EPSILON;
}
double side(Point const& a, Point const& b, Point const& p) {
    return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
}


std::vector<Point> points;


typedef int SideRef;
struct Triangle;
typedef Triangle* TriangleRef;

struct Edge {
    TriangleRef tri;
    SideRef     side;

    Edge() : tri(0), side(0) {}
    Edge(TriangleRef tri, SideRef side) : tri(tri), side(side) {}
};

struct Triangle {
    Point p[3];
    Edge  edge[3];
    TriangleRef children[3];

    Triangle() {}
    Triangle(Point const& p0, Point const& p1, Point const& p2) {
        p[0] = p0; p[1] = p1; p[2] = p2;
        children[0] = children[1] = children[2] = 0;
    }

    bool has_children() const {
        return children[0] != 0;
    }
    int num_children() const {
        return children[0] == 0 ? 0
            : children[1] == 0 ? 1
            : children[2] == 0 ? 2 : 3;
    }

    bool contains(Point const& q) const {
        double a = side(p[0], p[1], q);
        double b = side(p[1], p[2], q);
        double c = side(p[2], p[0], q);
        return a >= -EPSILON && b >= -EPSILON && c >= -EPSILON;
    }
};

void set_edge(Edge a, Edge b) {
    if (a.tri) a.tri->edge[a.side] = b;
    if (b.tri) b.tri->edge[b.side] = a;
    if (a.tri && b.tri) {
        assert(a.tri->p[(a.side + 1) % 3] == b.tri->p[(b.side + 2) % 3]);
        assert(a.tri->p[(a.side + 2) % 3] == b.tri->p[(b.side + 1) % 3]);
    }
}

class Triangulation {
public:
    Triangulation() {
        num_tris = 0;
        block = new Block(0);
        const double LOTS = 1e6;
        the_root = new_triangle(Point(-LOTS, -LOTS), Point(+LOTS, -LOTS), Point(0, +LOTS));
    }
    ~Triangulation() {
        delete block;
    }
    TriangleRef find(Point p) const {
        return find(the_root, p);
    }
    void add_point(Point const& p) {
        add_point(find(the_root, p), p);
    }
    int size() const {
        return num_tris;
    }

private:
    TriangleRef the_root;

    static const int BLOCK_SIZE = 1000;
    int num_tris;
    struct Block {
        Block* prev;
        Triangle* triangles;

        Block(Block* prev) : prev(prev) {
            triangles = new Triangle[BLOCK_SIZE];
        }
        ~Block() {
            delete prev;
            delete[] triangles;
        }
    } *block;

    TriangleRef new_triangle(Point const& p0, Point const& p1, Point const& p2) {
        if (num_tris == BLOCK_SIZE) {
            block = new Block(block);
            num_tris = 0;
        }
        TriangleRef tri = &block->triangles[num_tris++];
        *tri = Triangle(p0, p1, p2);
        return tri;
    }

    static TriangleRef find(TriangleRef root, Point const& p) {
    again: {
        assert(root->contains(p));
        if (!root->has_children()) {
            return root;
        }
        else {
            for (int i = 0; i < 3 && root->children[i]; ++i) {
                if (root->children[i]->contains(p)) {
                    root = root->children[i];
                    goto again;
                }
            }
            assert(false && "point not found");
        }
        }
    }

    void add_point(TriangleRef root, Point const& p) {
        TriangleRef tab, tbc, tca;

        /* split it into three triangles */
        tab = new_triangle(root->p[0], root->p[1], p);
        tbc = new_triangle(root->p[1], root->p[2], p);
        tca = new_triangle(root->p[2], root->p[0], p);

        set_edge(Edge(tab, 0), Edge(tbc, 1));
        set_edge(Edge(tbc, 0), Edge(tca, 1));
        set_edge(Edge(tca, 0), Edge(tab, 1));
        set_edge(Edge(tab, 2), root->edge[2]);
        set_edge(Edge(tbc, 2), root->edge[0]);
        set_edge(Edge(tca, 2), root->edge[1]);

        root->children[0] = tab;
        root->children[1] = tbc;
        root->children[2] = tca;

        flip(tab, 2);
        flip(tbc, 2);
        flip(tca, 2);
    }

    void flip(TriangleRef tri, SideRef pi) {
        TriangleRef trj = tri->edge[pi].tri;
        int pj = tri->edge[pi].side;

        if (!trj) return;
        if (!in_circumcircle(tri->p[0], tri->p[1], tri->p[2], trj->p[pj])) return;
        assert(tri->p[(pi + 2) % 3] == trj->p[(pj + 1) % 3]);
        assert(tri->p[(pi + 1) % 3] == trj->p[(pj + 2) % 3]);

        TriangleRef trk = new_triangle(tri->p[(pi + 1) % 3], trj->p[pj], tri->p[pi]);
        TriangleRef trl = new_triangle(trj->p[(pj + 1) % 3], tri->p[pi], trj->p[pj]);

        set_edge(Edge(trk, 0), Edge(trl, 0));
        set_edge(Edge(trk, 1), tri->edge[(pi + 2) % 3]);
        set_edge(Edge(trk, 2), trj->edge[(pj + 1) % 3]);
        set_edge(Edge(trl, 1), trj->edge[(pj + 2) % 3]);
        set_edge(Edge(trl, 2), tri->edge[(pi + 1) % 3]);

        tri->children[0] = trk; tri->children[1] = trl; tri->children[2] = 0;
        trj->children[0] = trk; trj->children[1] = trl; trj->children[2] = 0;

        flip(trk, 1);
        flip(trk, 2);
        flip(trl, 1);
        flip(trl, 2);
    }
};


struct Face {
    Pos a, b, c;
    Face(Pos a = Pos(0, 0), Pos b = Pos(0, 0), Pos c = Pos(0, 0)) :
        a(a), b(b), c(c) {}
};
bool inner_check(const Face& F, const Pos& p) { return inner_check(F.a, F.b, F.c, p); }
void query() {
    auto answer = [&](const db& r) -> ll {
        if (r < 2 + TOL) return 0;
        ll ans = sqr(r - 2) * PI + TOL;
        return ans;
        };

    memset(P, -1, sizeof P);
    std::cin >> N;
    //std::cout << N << "\n"; return;
    std::vector<Pos> C(N);
    for (int i = 0; i < N; ++i) std::cin >> C[i], C[i].i = i;

    db ret = INF;
    for (Pos& p : C) ret = std::min(ret, (O - p).mag());

    //soldiers are out of mines' convex
    if (N < 3) { std::cout << answer(ret) << "\n"; return; }

    Delaunator d(C);

    std::vector<Face> face;
    for (int i = 0; i < d.triangles_.size(); i += 3) {
        Pos a = d.points_[d.triangles_[i]];
        Pos b = d.points_[d.triangles_[i + 1]];
        Pos c = d.points_[d.triangles_[i + 2]];
        if (ccw(a, b, c) < 0) std::swap(b, c);//norm ccw
        face.push_back(Face(a, b, c));
    }

    int s = -1, e = face.size();
    std::vector<Pii> seg;
    std::map<Pii, int> MAP;
    int sz = face.size();
    for (int i = 0; i < sz; i++) {
        Face& F = face[i];
        if (inner_check(F, O)) s = i;
        MAP[Pii(F.a.i, F.b.i, 0)] = i;
        MAP[Pii(F.b.i, F.c.i, 0)] = i;
        MAP[Pii(F.c.i, F.a.i, 0)] = i;
        seg.push_back(Pii(F.a.i, F.b.i, i));
        seg.push_back(Pii(F.b.i, F.c.i, i));
        seg.push_back(Pii(F.c.i, F.a.i, i));
    }

    //soldiers are out of mines' convex
    if (s == -1) { std::cout << answer(ret) << "\n"; return; }

    std::vector<Info> info;
    for (Pii& p : seg) {
        if (MAP.find(!p) == MAP.end()) info.push_back(Info(e, p.i, (C[p.x] - C[p.y]).mag() * .5));
        else info.push_back(Info(MAP[!p], p.i, (C[p.x] - C[p.y]).mag() * .5));
    }

    std::sort(info.begin(), info.end());

    //for (Info& i : info) std::cout << i.c << "\n";

    for (const Info& i : info) {
        //std::cout << i.c << "\n";
        if (find(s) == find(e)) break;
        join(i.u, i.v);
        ret = std::min(ret, i.c);
    }

    std::cout << answer(ret) << "\n";
    return;
}
void solve() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cout.tie(0);
    //std::cout << std::fixed;
    //std::cout.precision(10);
    //freopen("../../../input_data/mine_tests/01.in", "r", stdin);
    //freopen("../../../input_data/mine_tests/out.txt", "w", stdout);
    std::cin >> T;
    while (T--) query();
    return;
}
int main() { solve(); return 0; }//BAPC 2009 C Escape from the Minefield   boj5401
//refer to JusticeHui(boj), a.bahr(codeforce)

/*

1
7
-10 -1
-4 3
-4 -4
-1 -6
2 4
3 -4
7 0

*/