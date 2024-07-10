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
using namespace std;

int verbose = 0;
const int MAX_TRIS = 1000000;
const double EPSILON = 1e-6;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    inline bool operator == (Point const& that) const {
        return x == that.x && y == that.y;
    }
};

inline double sqr(double x) { return x * x; }

double dist_sqr(Point const& a, Point const& b) {
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

void read_input() {
    int num_points;
    scanf("%d", &num_points);
    points.resize(num_points);
    for (int i = 0; i < num_points; ++i) {
        scanf("%lf %lf", &points[i].x, &points[i].y);
    }
}

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

template <typename I, typename V>
class DijkstraHeap {
public:
    struct Pair {
        I i;
        V v;
        Pair() {}
        Pair(I i, V v) : i(i), v(v) {}
        inline bool operator < (Pair const& that) const {
            return v < that.v;
        }
    };
    void add(I i, V v) {
        V& x = seen[i];
        if (v > x) {
            x = v;
            pq.push(Pair(i, v));
        }
    }
    Pair pop() {
        Pair top = pq.top();
        pq.pop();
        return top;
    }
    bool empty() {
        return pq.empty();
    }
private:
    std::priority_queue<Pair> pq;
    std::map<I, V> seen;
};

double dist_to_any(Point const& p) {
    double d = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < points.size(); ++i) {
        d = std::min(d, dist_sqr(p, points[i]));
    }
    return d;
}

double edge_width(TriangleRef tri, SideRef side) {
    Point const& a = tri->p[(side + 1) % 3];
    Point const& b = tri->p[(side + 2) % 3];
    return 0.25 * dist_sqr(a, b);
}

double solve(void) {
    Triangulation tri;
    std::random_shuffle(points.begin(), points.end());
    for (size_t i = 0; i < points.size(); ++i) {
        tri.add_point(points[i]);
    }
    // Where are we now?
    TriangleRef start = tri.find(Point());
    double start_dist = dist_to_any(Point());
    // build a heap
    DijkstraHeap<TriangleRef, double> pq;
    pq.add(start, start_dist);
    /* Dijkstra */
    double escape = 0;
    while (!pq.empty()) {
        DijkstraHeap<TriangleRef, double>::Pair here = pq.pop();
        TriangleRef tri = here.i;
        double width = here.v;
        for (int e = 0; e < 3; ++e) {
            if (!tri->edge[e].tri) {
                escape = std::max(escape, width);
            }
            else {
                double w = std::min(width, edge_width(tri, e));
                pq.add(tri->edge[e].tri, w);
            }
        }
    }
    return escape;
}
#define M_PI        3.14159265358979323846
void run() {
    read_input();
    double r_sqr = solve();
    double r = std::max(0., sqrt(r_sqr) - 2);
    printf("%.f\n", floor(r * r * M_PI));
}

int main(int argc, char** argv) {
    // Parse args
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg.size() > 2 && arg.substr(0, 2) == "--") arg = arg.substr(1);
        if (arg == "-v" || arg == "-verbose") {
            verbose = 1;
        }
        else if (arg == "-v1") {
            verbose = 1;
        }
        else if (arg == "-v2") {
            verbose = 2;
        }
        else {
            fprintf(stderr, "Unknown argument: %s", argv[i]);
            exit(1);
        }
    }
    int runs;
    scanf("%d", &runs);
    for (int i = 0; i < runs; ++i) {
        if (verbose) {
            fprintf(stderr, "\nRun %d:\n", i);
        }
        run();
    }
    return 0;
}