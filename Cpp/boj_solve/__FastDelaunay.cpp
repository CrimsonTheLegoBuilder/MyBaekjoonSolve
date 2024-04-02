#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <queue>
#include <deque>
#include <tuple>
typedef long long ll;
typedef double ld;
typedef long double lld;
const ld INF = 1e18;
const ld TOL = 1e-7;
const ld PI = acos(-1);
const int LEN = 1e5;
int N, M, T, Q;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int dcmp(const ld& x) { return std::abs(x) < TOL ? 0 : x > 0 ? 1 : -1; }
int dcmp(const ll& x) { return !x ? 0 : x > 0 ? 1 : -1; }
ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

struct Pii {
    ll x, y;
    int i;
    Pii(ll X = 0, ll Y = 0, int I = 0) : x(X), y(Y), i(I) {}
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
    Pii& operator *= (const ll& scale) { x *= scale; y *= scale; return *this; }
    Pii& operator /= (const ll& scale) { x /= scale; y /= scale; return *this; }
    Pii operator ~ () const { return { -y, x }; }
    ll operator ! () const { return x * y; }
    ll Euc() const { return x * x + y * y; }
    ll Man() const { return std::abs(x) + std::abs(y); }
    ld mag() const { return hypot(x, y); }
    friend std::istream& operator >> (std::istream& is, Pii& p) { is >> p.x >> p.y; return is; }
    friend std::ostream& operator << (std::ostream& os, const Pii& p) { os << p.x << " " << p.y; return os; }
};
const Pii Oii = { 0, 0 };
const Pii INF_PT = { (ll)INF, (ll)INF };
ll cross(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pii& d1, const Pii& d2, const Pii& d3, const Pii& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pii& d1, const Pii& d2, const Pii& d3) {
    ll ret = cross(d1, d2, d3);
    return !ret ? 0 : ret > 0 ? 1 : -1;
}
/// source: KACTL
/// https://github.com/ToxicPie/NaCl/blob/master/nacl/geometry/delaunay.hpp

//template <typename T> struct P {
//    T x, y;
//    P(T x = 0, T y = 0) : x(x), y(y) {}
//    bool operator<(const P& p) const {
//        return tie(x, y) < tie(p.x, p.y);
//    }
//    bool operator==(const P& p) const {
//        return tie(x, y) == tie(p.x, p.y);
//    }
//    P operator-() const { return { -x, -y }; }
//    P operator+(P p) const { return { x + p.x, y + p.y }; }
//    P operator-(P p) const { return { x - p.x, y - p.y }; }
//    P operator*(T d) const { return { x * d, y * d }; }
//    P operator/(T d) const { return { x / d, y / d }; }
//    T dist2() const { return x * x + y * y; }
//    double len() const { return sqrt(dist2()); }
//    P unit() const { return *this / len(); }
//    friend T dot(P a, P b) { return a.x * b.x + a.y * b.y; }
//    friend T cross(P a, P b) { return a.x * b.y - a.y * b.x; }
//    friend T cross(P a, P b, P o) {
//        return cross(a - o, b - o);
//    }
//};
//using pt = P<ll>;

typedef struct Quad* Q;
//typedef __int128_t lll; // (can be ll if coords are < 2e4)
Pii arb(LLONG_MAX, LLONG_MAX); // not equal to any other point

struct Quad {
    bool mark;
    Q o, rot;
    Pii p;
    Pii F() { return r()->p; }
    Q r() { return rot->rot; }
    Q prev() { return rot->o->rot; }
    Q next() { return r()->prev(); }
};

bool circ(Pii p, Pii a, Pii b, Pii c) { // is p in the circumcircle?
    lld p2 = p.Euc(), A = a.Euc() - p2,
        B = b.Euc() - p2, C = c.Euc() - p2;
    return cross(p, a, b) * C + cross(p, b, c) * A +
        cross(p, c, a) * B >
        0;
}
Q makeEdge(Pii orig, Pii dest) {
    Q q[] = { new Quad{0, 0, 0, orig}, new Quad{0, 0, 0, arb},
             new Quad{0, 0, 0, dest}, new Quad{0, 0, 0, arb} };
    for (int i = 0; i < 4; i++) q[i]->o = q[-i & 3],
        q[i]->rot = q[(i + 1) & 3];
    return *q;
}
void splice(Q a, Q b) {
    std::swap(a->o->rot->o, b->o->rot->o);
    std::swap(a->o, b->o);
}
Q connect(Q a, Q b) {
    Q q = makeEdge(a->F(), b->p);
    splice(q, a->next());
    splice(q->r(), b);
    return q;
}

std::pair<Q, Q> rec(const std::vector<Pii>& s) {
    if (s.size() <= 3) {
        Q a = makeEdge(s[0], s[1]),
            b = makeEdge(s[1], s.back());
        if (s.size() == 2) return {a, a->r()};
        splice(a->r(), b);
        auto side = cross(s[0], s[1], s[2]);
        Q c = side ? connect(b, a) : 0;
        return { side < 0 ? c->r() : a, side < 0 ? c : b->r() };
    }

#define H(e)     e->F(), e->p
#define valid(e) (cross(e->F(), H(base)) > 0)
    Q A, B, ra, rb;
    int half = s.size() / 2;
    std::tie(ra, A) = rec({ s.begin(), s.end() - half});
    std::tie(B, rb) = rec({ s.size() - half + s.begin(), s.end() });
    while ((cross(B->p, H(A)) < 0 && (A = A->next())) ||
        (cross(A->p, H(B)) > 0 && (B = B->r()->o)))
        ;
    Q base = connect(B->r(), A);
    if (A->p == ra->p) ra = base->r();
    if (B->p == rb->p) rb = base;

#define DEL(e, init, dir)                                  \
  Q e = init->dir;                                         \
  if (valid(e))                                            \
    while (circ(e->dir->F(), H(base), e->F())) {           \
      Q t = e->dir;                                        \
      splice(e, e->prev());                                \
      splice(e->r(), e->r()->prev());                      \
      e = t;                                               \
    }
    for (;;) {
        DEL(LC, base->r(), o);
        DEL(RC, base, prev());
        if (!valid(LC) && !valid(RC)) break;
        if (!valid(LC) || (valid(RC) && circ(H(RC), H(LC))))
            base = connect(RC, base->r());
        else base = connect(base->r(), LC->r());
    }
    return { ra, rb };
}

// returns [A_0, B_0, C_0, A_1, B_1, ...]
// where A_i, B_i, C_i are counter-clockwise triangles
std::vector<Pii> triangulate(std::vector<Pii> pts) {
    std::sort(pts.begin(), pts.end());
    assert(unique(pts.begin(), pts.end()) == pts.end());
    if (pts.size() < 2) return {};
    Q e = rec(pts).first;
    std::vector<Q> q = { e };
    int qi = 0;
    while (cross(e->o->F(), e->F(), e->p) < 0) e = e->o;
#define ADD                                                \
  {                                                        \
    Q c = e;                                               \
    do {                                                   \
      c->mark = 1;                                         \
      pts.push_back(c->p);                                 \
      q.push_back(c->r());                                 \
      c = c->next();                                       \
    } while (c != e);                                      \
  }
    ADD;
    pts.clear();
    while (qi < q.size())
        if (!(e = q[qi++])->mark) ADD;
    return pts;
}