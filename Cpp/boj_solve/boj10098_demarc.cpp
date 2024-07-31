#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cmath>
#include <cstring>

/*
* 
* Splay tree / sweeping by jay0202
* Geometry by crimson231
* 
*/

//#define JAY_MODULE_DEBUG
//#define CRIMSON_MODULE_DEBUG

typedef long long ll;
typedef double ld;
typedef std::pair<int, int> pi;
typedef std::vector<int> Vint;
typedef std::vector<ld> Vld;
const int LEN = 1e5 + 1;

int N;
ll memo[LEN];
std::vector<int> g[LEN]; // above lines
struct Pos {
    int x, y;
    Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
    bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
    bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
    bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
    bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
    Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
    Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
    Pos operator * (const int& n) const { return { x * n, y * n }; }
    Pos operator / (const int& n) const { return { x / n, y / n }; }
    ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
    ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
    Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
    Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
    Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
    Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
    Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
    Pos operator - () const { return { -x, -y }; }
    Pos operator ~ () const { return { -y, x }; }
    Pos operator ! () const { return { y, x }; }
    ll xy() const { return (ll)x * y; }
    ll Euc() const { return (ll)x * x + (ll)y * y; }
    int Man() const { return std::abs(x) + std::abs(y); }
    ld mag() const { return hypot(x, y); }
    ld rad() const { return atan2(y, x); }
    friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
    int quad() const { return y > 0 || y == 0 && x >= 0; }
    friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
    friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
    friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} pos[LEN];
const Pos O = Pos(0, 0);
const Pos INVAL = Pos(-1, -1);
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0; }
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) { return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0; }
bool collinear(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return !ccw(d1, d2, d3) && !ccw(d1, d2, d4); }
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2, const bool f = 1) {
    //f : include end of seg, !f : ignore end of seg
    bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
    bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
    if (!f) return f1 && f2;
    bool f3 = on_seg_strong(s1, s2, d1) ||
        on_seg_strong(s1, s2, d2) ||
        on_seg_strong(d1, d2, s1) ||
        on_seg_strong(d1, d2, s2);
    return (f1 && f2) || f3;
}
bool inner_check(Pos p0, Pos p1, Pos p2, const Pos& t) {
    if (ccw(p0, p1, p2) < 0) std::swap(p1, p2);
    return ccw(p0, p1, t) >= 0 && ccw(p1, p2, t) >= 0 && ccw(p2, p0, t) >= 0;
}
ll area(Pos H[], const int& sz) {
    ll ret = 0;
    for (int i = 0; i < sz; i++) {
        Pos cur = H[i], nxt = H[(i + 1) % sz];
        ret += cross(O, cur, nxt);
    }
    return ret;
}
ll area(std::vector<Pos>& H) {
    ll ret = 0;
    int sz = H.size();
    for (int i = 0; i < sz; i++) {
        Pos cur = H[i], nxt = H[(i + 1) % sz];
        ret += cross(O, cur, nxt);
    }
    return ret;
}
void norm(Pos H[], const int& sz) {
    ll A = area(H, sz);
    assert(A);
    if (A < 0) std::reverse(H, H + sz);
    return;
}
void norm(Polygon& H) {
    ll A = area(H);
    if (A < 0) std::reverse(H.begin(), H.end());
    auto s = std::min_element(H.begin(), H.end());
    std::rotate(H.begin(), s, H.end());
}
void move(Pos H[], const Pos& vec) { for (int i = 0; i < N; i++) H[i] += vec; }
void move(Polygon& H, const Pos& vec) {
    int sz = H.size();
    for (int i = 0; i < sz; i++) H[i] += vec;
}
void rotate() { for (int i = 0; i < N; i++) pos[i] = ~pos[i]; }
void rotate(Polygon& H) {
    int sz = H.size();
    for (int i = 0; i < sz; i++) H[i] = ~H[i];
}
void mirror() { for (int i = 0; i < N; i++) pos[i].x = -pos[i].x; }
void mirror(Polygon& H) {
    int sz = H.size();
    for (int i = 0; i < sz; i++) H[i].x = -H[i].x;
}
bool polygon_cmp(const Polygon& A, const Polygon& B) {
    int a = A.size(), b = B.size();
    if (a != b) return 0;
    for (int i = 0; i < a; i++) {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}
bool operator == (const Polygon& p, const Polygon& q) { return polygon_cmp(p, q); }
Polygon make_polygon(int u, int v, Pos s, Pos e) {
    Polygon H, tmp;
    Vint V;
    //std::cout << "fuck:: make poly " << u << " " << v << "\n";
    //std::cout << "fuck:: " << s << " " << e << "\n";
    H.push_back(e);
    H.push_back(s);
    for (int i = (u + 1) % N; i != (v + 1) % N; i = (i + 1) % N) {
        const Pos& p = pos[i];
        if (p != s && p != e) H.push_back(p);
    }
    int sz = H.size();
    V.resize(sz, 0);
    for (int i = 0; i < sz; i++) {
        const Pos& pre = H[(i - 1 + sz) % sz], cur = H[i], nxt = H[(i + 1) % sz];
        if (!ccw(pre, cur, nxt)) V[i] = 1;
    }
    for (int i = 0; i < sz; i++) if (!V[i]) tmp.push_back(H[i]);
    return tmp;
}
bool all_polygon_cmp(int u, int v, Pos s, Pos e) {
    Polygon A = make_polygon(u, v, s, e);
    Polygon B = make_polygon(v, u, e, s);
#ifdef CRIMSON_MODULE_DEBUG
    std::cout << "FUCK:: s:: " << s << " e:: " << e << "\n";
    std::cout << "FUCK::\n";
    std::cout << "A = [\n";
    for (Pos& p : A) {
        std::cout << "(" << p.x << ", " << p.y << "), \n";
    }
    std::cout << "]\n";
    std::cout << "B = [\n";
    for (Pos& p : B) {
        std::cout << "(" << p.x << ", " << p.y << "), \n";
    }
    std::cout << "]\n";
#endif
    if (A.size() != B.size()) return 0;
    norm(A), norm(B);
    Pos b = *std::min_element(B.begin(), B.end());
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            rotate(A);
            norm(A);
            Pos a = *std::min_element(A.begin(), A.end());
            move(A, b - a);
            if (A == B) return 1;
        }
        mirror(A);
    }
    return 0;
}
struct Line {
    int y;      // y coord
    int l, r;   // x coords (l <= r)

    /// <summary>
    /// sort lines
    /// <para>
    /// 1. compare y.
    /// </para>
    /// <para>
    /// 2. if y coords are equal, compare x.
    /// </para>
    /// </summary>
    /// <param name="o">other</param>
    /// <returns>bool</returns>
    bool operator<(const Line& o) const {
        return y == o.y ? l < o.l : y > o.y;
    }
};
int Q;
/// <summary>
/// event horizon
/// </summary>
struct Event {
    int i; // index of event line
    int d; // flag value. (0: insert range, 1: query range)
    Line l; // line
    bool operator<(const Event& r) const { return l < r.l; }
} events[LEN];
/// <summary>
/// segment struct defined to use in SplayTree node.
/// simplified version of line event (ommited y coord)
/// </summary>
struct Seg {
    int l, r; // x coords
    int i;
    Seg(int l = 0, int r = 0, int i = 0) : l(l), r(r), i(i) {}
    /// <summary>
    /// only compare x coords
    /// </summary>
    /// <param name="o">other</param>
    /// <returns></returns>
    bool operator<(const Seg& o) const {
        return l < o.l;
    }
};
std::vector<Seg> Segs;
void match(int u, int v, int l, int r) {
#ifdef JAY_MODULE_DEBUG
    std::cout << "facing pair: " << u << ' ' << v << '\n';
    std::cout << "    facing range: " << l << ' ' << r << '\n';
#endif
    g[u].push_back(v);
    Segs.push_back(Seg(l, r, v));
}
bool find_split(const int i, const Seg& S, Pos& s, Pos& e) {
    int j = i, k = S.i;
    Pos& J = pos[j], & K = pos[k];
    bool f = 0;
    if (k < j) f = 1, std::swap(j, k);
    ll a1 = memo[k] - memo[j];
    //std::cout << "FUCK:: a1:: " << a1 << "\n";
    //std::cout << "FUCK:: A :: " << memo[N] << "\n";
    if (f) a1 = memo[N] - a1;
    Pos& pl = pos[i], & pr = pos[(i + 1) % N];
    assert(pl.x < pr.x);
    int r = std::min(pr.x, S.r);
    int l = std::max(pl.x, S.l);
    int yh = pos[S.i].y;
    int yl = pl.y;
    int h = yh - yl;
    Pos b1 = Pos(r, yh), b2 = Pos(l, yh), b3 = Pos(l, yl), b4 = Pos(r, yl);
    /*
    sl b2 b1 sr K  -- a1
    pl b3 b4 pr J  --
    */
    ll amax = a1 + K / b2 + b2 / b3 + b3 / J;
    ll amin = a1 + K / b1 + b1 / b4 + b4 / J;
    ll a2 = memo[N] >> 1;
    //std::cout << "DEBUG:: amin:: " << amin << " amax:: " << amax << " a2:: " << a2 << "\n";
    if (amin <= a2 && a2 <= amax) {
        ll box = a2 - amin;
        ll w = box / h;
        if (w & 1) return 0;
        s = Pos(r - (w >> 1), pr.y);
        e = Pos(r - (w >> 1), pos[S.i].y);
        assert(s < e);
        return 1;
    }
    return 0;
}
class SplayTree {
    struct Node {
        Node* l;
        Node* r;
        Node* p;
        Seg val;
        Node(int l, int r, int i) : l(0), r(0), p(0) { val = { l, r, i }; }
        ~Node() { if (l) delete l; if (r) delete r; }
    } *root;
    void rotate(Node* x) {
        Node* p = x->p;
        if (!p) return;
        Node* b = 0;
        if (x == p->l) {
            p->l = b = x->r;
            x->r = p;
        }
        else {
            p->r = b = x->l;
            x->l = p;
        }
        x->p = p->p;
        p->p = x;
        if (b) b->p = p;
        (x->p ? p == x->p->l ? x->p->l : x->p->r : root) = x;
    }
    void splay(Node* x) {
        while (x->p) {
            Node* p = x->p;
            Node* g = p->p;
            if (g) {
                if ((x == p->l) == (p == g->l)) rotate(p);
                else rotate(x);
            }
            rotate(x);
        }
    }

    /// <summary>
    /// find a line that has nearest r coord from l.  
    /// </summary>
    /// <param name="l">: target x coord</param>
    /// <returns></returns>
    Node* find(int l) {
        if (!root) return 0;

        Node* p = root;
        while (1) {
            if (p->val.r == l) break;
            if (p->val.r < l) {
                if (!p->r) {
                    break;
                }
                p = p->r;
            }
            else {
                if (!p->l) {
                    break;
                }
                p = p->l;
            }
        }
        splay(p);
        return p;
    }

public:
    SplayTree() : root(0) {}
    ~SplayTree() { if (root) delete root; }
    void insert(int l, int r, int i) {
        if (!root) {
            root = new Node(l, r, i);
            return;
        }
        Node* p = root;
        Node** pp;
        while (1) {
            if (p->val.l < l) {
                if (!p->r) {
                    pp = &p->r;
                    break;
                }
                p = p->r;
            }
            else {
                if (!p->l) {
                    pp = &p->l;
                    break;
                }
                p = p->l;
            }
        }
        Node* x = new Node(l, r, i);
        *pp = x;
        x->p = p;
        splay(x);
    }
    void pop(Node* ptr) {
        if (!ptr) return;
        splay(ptr);
        Node* p = root;
        if (p->l && p->r) {
            root = p->l; root->p = 0;
            Node* l = root;
            while (l->r) l = l->r;
            l->r = p->r;
            p->r->p = l;
        }
        else if (p->l) root = p->l, root->p = 0;
        else if (p->r) root = p->r, root->p = 0;
        else root = 0;
        p->l = p->r = 0;
        delete p;
    }

    /// <summary>
    /// find leftmost line first.
    /// if line is splitted by query segment, then insert new segment and return.
    /// else, find right segments while r coord of segment is smaller than query r.
    /// </summary>
    /// <param name="l">x coord</param>
    /// <param name="r">x coord</param>
    /// <param name="i">line index</param>
    void query(int l, int r, int i) {
        assert(root);

        Node* x = find(l); // first segment
        assert(x->val.l <= l);

        if (x->val.r < l) { // if segment is out of range: find very right one
            x = x->r;
            while (x->l) x = x->l;
        }

        if (x->val.r > r) { // if splitted
            match(i, x->val.i, l, r);
            int nl = r, nr = x->val.r, ni = x->val.i;
            x->val.r = l; // split
            if (x->val.l == l) pop(x); // and pop if left is covered.
            insert(nl, nr, ni); // insert new right segment
            return;
        }

        match(i, x->val.i, l, x->val.r); // match two of them
        x->val.r = l; // left side of segment is covered by query segment
        if (x->val.r == x->val.l) pop(x); // pop if all range covered

        while (x = find(l)) { // find leftmost
            //if (x->val.r == l) break;
            if (x->val.r <= l) { // if leftmost is out of range
                if (!x->r) return; // no more right segment
                x = x->r;
                while (x->l) x = x->l; // find very right segment
            }
            if (x->val.l > r) return;
            match(i, x->val.i, x->val.l, std::min(r, x->val.r)); // match two of them
            if (x->val.r > r) { // if right segment is not covered totally
                x->val.l = r;
                break; // then, query range is sweeped. return
            }
            else pop(x); // else, a segment is sweeped. pop and find next segment
        }
    }
} sp;
bool vertical_split(Pos& s, Pos& e) {
    memo[0] = 0;
    for (int i = 0; i < N; i++) {
        memo[i + 1] = memo[i] + pos[i] / pos[(i + 1) % N];
    }
    // horizontal check
    Q = 0;
    for (int i = 0; i < N; ++i) {
        int j = (i + 1) % N;
        if (pos[i].y == pos[j].y) {
            if (pos[i].x < pos[j].x) { // right halfline
                events[Q++] = { i, 1, { pos[i].y, pos[i].x, pos[j].x } };
            }
            if (pos[i].x > pos[j].x) { // left halfline
                events[Q++] = { i, 0, { pos[i].y, pos[j].x, pos[i].x } };
            }
        }
    }

    std::sort(events, events + Q);

    for (int i = 0; i < Q; ++i) {
        //std::cout << (events[i].d == 0 ? "insert" : "query") << " segment: (h, l, r) ";
        //std::cout << events[i].l.y << ' ' << events[i].l.l << ' ' << events[i].l.r << '\n';
        if (events[i].d == 0) sp.insert(events[i].l.l, events[i].l.r, events[i].i);
        if (events[i].d == 1) {
            Segs.clear();
            sp.query(events[i].l.l, events[i].l.r, events[i].i);
            for (Seg& S : Segs) {
                //std::cout << "segs::\n";
                if (find_split(events[i].i, S, s, e)) {
                    //std::cout << "polygon cmp before::\n";
                    if (all_polygon_cmp(events[i].i, S.i, s, e)) return 1;
                    //std::cout << "polygon cmp after::\n";
                }
            }
        }
    }
    return 0;
}
void solve() {
    freopen("demarcation.in copy.14", "r", stdin);
    freopen("demarcation.out", "w", stdout);
    std::cin.tie(0)->sync_with_stdio(0);
    //std::cout.tie(0);
    std::cin >> N;
    for (int i = 0; i < N; ++i) std::cin >> pos[i].x >> pos[i].y;
    ll A = area(pos, N);
    assert(A);
    if (A < 0) std::reverse(pos, pos + N);
    A *= -1;
    A >>= 1;
#ifdef CRIMSON_MODULE_DEBUG
    std::cout << "FUCK::\n";
    std::cout << "pos = [\n";
    for (int i = 0; i < N; i++) {
        std::cout << "(" << pos[i].x << ", " << pos[i].y << "), \n";
    }
    std::cout << "]\n";
#endif
    if (A & 1) { std::cout << "NO\n"; return; }

    Pos s, e;
    if (vertical_split(s, e)) {
        //std::cout << "YES\n";
        for (int i = 0; i < N; i++) {
            if (pos[i].x == e.x && s.y < pos[i].y && e.y > pos[i].y)
                e = pos[i];
        }
        std::cout << s << " " << e << "\n";
        return;
    }
    rotate();
    //std::cout << "FUCK::\n";
    if (vertical_split(s, e)) {
        //std::cout << "YES\n";
        for (int i = 0; i < N; i++) {
            if (pos[i].x == e.x && s.y < pos[i].y && e.y > pos[i].y)
                e = pos[i];
        }
        std::cout << -~s << " " << -~e << "\n";
        return;
    }
    //std::cout << "FUCK::\n";
    std::cout << "NO\n";
    return;
}
int main() { solve(); return 0; }//boj10098 Demarcation

/*

8
0 0
6 0
6 2
5 2
5 1
1 1
1 4
0 4



void solve() {
    freopen("demarcation.in.1", "r", stdin);
    freopen("demarcation.out", "w", stdout);
    std::cin.tie(0)->sync_with_stdio(0);
    //std::cout.tie(0);
    std::cin >> N;
    for (int i = 0; i < N; ++i) std::cin >> pos[i].x >> pos[i].y;
    norm(pos, N);

    // horizontal check
    Q = 0;
    for (int i = 0; i < N; ++i) {
        int j = (i + 1) % N;
        if (pos[i].y == pos[j].y) {
            if (pos[i].x < pos[j].x) { // right halfline
                events[Q++] = { i, 1, { pos[i].y, pos[i].x, pos[j].x } };
            }
            if (pos[i].x > pos[j].x) { // left halfline
                events[Q++] = { i, 0, { pos[i].y, pos[j].x, pos[i].x } };
            }
        }
    }

    std::sort(events, events + Q);
    for (int i = 0; i < Q; ++i) {
        std::cout << (events[i].d == 0 ? "insert" : "query") << " segment: (h, l, r) ";
        std::cout << events[i].l.y << ' ' << events[i].l.l << ' ' << events[i].l.r << '\n';
        if (events[i].d == 0) sp.insert(events[i].l.l, events[i].l.r, events[i].i);
        if (events[i].d == 1) sp.query(events[i].l.l, events[i].l.r, events[i].i);
    }
#ifdef JAY_MODULE_DEBUG
    std::cout << "points = [\n";
    for (int i = 0; i < N; i++) {
        std::cout << "  (" << pos[i].x << ", " << pos[i].y << "), \n";
    }
    std::cout << "]\n";
    for (int i = 0; i <= N; ++i) {
        if (g[i].size()) {
            std::cout << "right half-line from " << i << '\n';
            std::cout << "    matching left half-lines from: ";
            for (const int& j : g[i]) std::cout << j << ' ';
            std::cout << '\n';
        }
    }
#endif
}
*/
