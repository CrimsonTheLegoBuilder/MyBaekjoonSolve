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
#define x first
#define y second
using namespace std;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;
const double EPS = 1e-9;
pdd operator + (pdd a, pdd b) { return { a.x + b.x, a.y + b.y }; }
pdd operator - (pdd a, pdd b) { return { a.x - b.x, a.y - b.y }; }
double operator * (pdd a, pdd b) { return a.x * b.x + a.y * b.y; }
double operator / (pdd a, pdd b) { return a.x * b.y - a.y * b.x; }
pdd operator * (double a, pdd p) { return { a * p.x, a * p.y }; }
pdd operator / (pdd a, double b) { return { a.x / b, a.y / b }; }
int dcmp(double x) { return x < -EPS ? -1 : x > EPS ? 1 : 0; }
double sq(double x) { return x * x; }
double size(pdd p) { return hypot(p.x, p.y); }
double sz2(pdd p) { return sq(p.x) + sq(p.y); }
pdd r90(pdd p) { return pdd(-p.y, p.x); }
pdd line_intersect(pdd a, pdd b, pdd u, pdd v) { return u + (((a - u) / b) / (v / b)) * v; }
pdd get_circumcenter(pdd p0, pdd p1, pdd p2) {
    return line_intersect(0.5 * (p0 + p1), r90(p0 - p1), 0.5 * (p1 + p2), r90(p1 - p2));
}
double pb_int(pdd left, pdd right, double sweepline) {
    if (dcmp(left.y - right.y) == 0) return (left.x + right.x) / 2.0;
    int sign = left.y < right.y ? -1 : 1;
    pdd v = line_intersect(left, right - left, pdd(0, sweepline), pdd(1, 0));
    double d1 = sz2(0.5 * (left + right) - v), d2 = sz2(0.5 * (left - right));
    return v.x + sign * sqrt(std::max(0.0, d1 - d2));
}
struct Beachline {
    struct node {
        node() {}
        node(pdd point, int idx) :point(point), idx(idx), end(0), link{ 0, 0 }, par(0), prv(0), nxt(0) {}
        pdd point; int idx; int end;
        node* link[2], * par, * prv, * nxt;
    };
    node* root;
    double sweepline;
    Beachline() : sweepline(-1e20), root(NULL) { }
    inline int dir(node* x) { return x->par->link[0] != x; }
    void rotate(node* n) {
        node* p = n->par; int d = dir(n);
        p->link[d] = n->link[!d];
        if (n->link[!d]) n->link[!d]->par = p;
        n->par = p->par; if (p->par) p->par->link[dir(p)] = n;
        n->link[!d] = p; p->par = n;
    } void splay(node* x, node* f = NULL) {
        while (x->par != f) {
            if (x->par->par == f);
            else if (dir(x) == dir(x->par)) rotate(x->par);
            else rotate(x);
            rotate(x);
        }
        if (f == NULL) root = x;
    } void insert(node* n, node* p, int d) {
        splay(p); node* c = p->link[d];
        n->link[d] = c; if (c) c->par = n;
        p->link[d] = n; n->par = p;
        node* prv = !d ? p->prv : p, * nxt = !d ? p : p->nxt;
        n->prv = prv; if (prv) prv->nxt = n;
        n->nxt = nxt; if (nxt) nxt->prv = n;
    } void erase(node* n) {
        node* prv = n->prv, * nxt = n->nxt;
        if (!prv && !nxt) { if (n == root) root = NULL; return; }
        n->prv = NULL; if (prv) prv->nxt = nxt;
        n->nxt = NULL; if (nxt) nxt->prv = prv;
        splay(n);
        if (!nxt) {
            root->par = NULL; n->link[0] = NULL;
            root = prv;
        }
        else {
            splay(nxt, n); node* c = n->link[0];
            nxt->link[0] = c; c->par = nxt; n->link[0] = NULL;
            n->link[1] = NULL; nxt->par = NULL;
            root = nxt;
        }
    } bool get_event(node* cur, double& next_sweep) {
        if (!cur->prv || !cur->nxt) return false;
        pdd u = r90(cur->point - cur->prv->point);
        pdd v = r90(cur->nxt->point - cur->point);
        if (dcmp(u / v) != 1) return false;
        pdd p = get_circumcenter(cur->point, cur->prv->point, cur->nxt->point);
        next_sweep = p.y + size(p - cur->point); return true;
    } node* find_bl(double x) {
        node* cur = root;
        while (cur) {
            double left = cur->prv ? pb_int(cur->prv->point, cur->point, sweepline) : -1e30;
            double right = cur->nxt ? pb_int(cur->point, cur->nxt->point, sweepline) : 1e30;
            if (left <= x && x <= right) { splay(cur); return cur; }
            cur = cur->link[x > right];
        }
    }
}; using BNode = Beachline::node;
static BNode* arr;
static int sz;
static BNode* new_node(pdd point, int idx) { arr[sz] = BNode(point, idx); return arr + (sz++); }
struct event {
    event(double sweep, int idx) : type(0), sweep(sweep), idx(idx) {}
    event(double sweep, BNode* cur) : type(1), sweep(sweep), prv(cur->prv->idx), cur(cur), nxt(cur->nxt->idx) {}
    int type, idx, prv, nxt; BNode* cur; double sweep;
    bool operator>(const event& l)const { return sweep > l.sweep; }
};
void VoronoiDiagram(vector<pdd>& input, vector<pdd>& vertex, vector<pii>& edge, vector<pii>& area) {
    Beachline bl = Beachline();
    priority_queue<event, vector<event>, greater<event>> events;
    auto add_edge = [&](int u, int v, int a, int b, BNode* c1, BNode* c2) {
        if (c1) c1->end = edge.size() * 2;
        if (c2) c2->end = edge.size() * 2 + 1;
        edge.emplace_back(u, v);
        area.emplace_back(a, b);
        };
    auto write_edge = [&](int idx, int v) { idx % 2 == 0 ? edge[idx / 2].x = v : edge[idx / 2].y = v; };
    auto add_event = [&](BNode* cur) { double nxt; if (bl.get_event(cur, nxt)) events.emplace(nxt, cur); };
    int n = input.size(), cnt = 0;
    arr = new BNode[n * 4]; sz = 0;
    sort(input.begin(), input.end(), [](const pdd& l, const pdd& r) { return l.y != r.y ? l.y < r.y : l.x < r.x; });
    BNode* tmp = bl.root = new_node(input[0], 0), *t2;
    for (int i = 1; i < n; i++) {
        if (dcmp(input[i].y - input[0].y) == 0) {
            add_edge(-1, -1, i - 1, i, 0, tmp);
            bl.insert(t2 = new_node(input[i], i), tmp, 1);
            tmp = t2;
        }
        else events.emplace(input[i].y, i);
    }
    while (events.size()) {
        event q = events.top(); events.pop();
        BNode* prv, * cur, * nxt, * site;
        int v = vertex.size(), idx = q.idx;
        bl.sweepline = q.sweep;
        if (q.type == 0) {
            pdd point = input[idx];
            cur = bl.find_bl(point.x);
            bl.insert(site = new_node(point, idx), cur, 0);
            bl.insert(prv = new_node(cur->point, cur->idx), site, 0);
            add_edge(-1, -1, cur->idx, idx, site, prv);
            add_event(prv); add_event(cur);
        }
        else {
            cur = q.cur, prv = cur->prv, nxt = cur->nxt;
            if (!prv || !nxt || prv->idx != q.prv || nxt->idx != q.nxt) continue;
            vertex.push_back(get_circumcenter(prv->point, nxt->point, cur->point));
            write_edge(prv->end, v); write_edge(cur->end, v);
            add_edge(v, -1, prv->idx, nxt->idx, 0, prv);
            bl.erase(cur);
            add_event(prv); add_event(nxt);
        }
    }
    delete arr;
}

pdd center(pdd a, pdd b) { return (a + b) / 2; }
pdd center(pdd a, pdd b, pdd c) {
    pdd aa = b - a, bb = c - a;
    auto c1 = aa * aa / 2, c2 = bb * bb / 2, d = aa / bb;
    auto x = a.x + (c1 * bb.y - c2 * aa.y) / d;
    auto y = a.y + (c2 * aa.x - c1 * bb.x) / d;
    return { x, y };
}

bool double_eq(double a, double b) { return abs(a - b) < EPS; }
bool point_eq(pdd a, pdd b) { return double_eq(a.x, b.x) && double_eq(a.y, b.y); }
int sign(double v) { return double_eq(v, 0) ? 0 : (v > 0) - (v < 0); }

int quadrant_id(const pdd p) {
    static int qa[9] = { 5, 4, 3, 6, -1, 2, 7, 0, 1 };
    return qa[sign(p.x) * 3 + sign(p.y) + 4];
}

int ccw(const pdd& p1, const pdd& p2, const pdd& p3) {
    double res = (p3 - p1) / (p3 - p2);
    if (double_eq(res, 0)) return 0;
    return (res > 0) - (res < 0);
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<pdd> input(n), vertex;
    vector<pii> edge, area;
    for (auto& a : input) cin >> a.x >> a.y;
    VoronoiDiagram(input, vertex, edge, area);

    double res = 0;
    vector<vector<int>> gph(n);
    for (int i = 0; i < area.size(); i++) {
        gph[area[i].first].push_back(area[i].second);
        gph[area[i].second].push_back(area[i].first);
    }
    for (int i = 0; i < n; i++) {
        sort(gph[i].begin(), gph[i].end());
        gph[i].erase(unique(gph[i].begin(), gph[i].end()), gph[i].end());
    }
    for (int i = 0; i < n; i++) {
        if (gph[i].size() <= 1) continue;
        sort(gph[i].begin(), gph[i].end(), [&](auto a, auto b) {
            auto p1 = input[a] - input[i], p2 = input[b] - input[i];
            return quadrant_id(p1) != quadrant_id(p2) ? quadrant_id(p1) < quadrant_id(p2) : p1 / p2 > 0;
            });
        for (int j = 0; j < gph[i].size(); j++) {
            int k = j + 1 < gph[i].size() ? j + 1 : 0;
            if (!binary_search(gph[gph[i][j]].begin(), gph[gph[i][j]].end(), gph[i][k])) continue;
            auto p = center(input[i], input[gph[i][j]], input[gph[i][k]]);
            res = max(res, size(input[i] - p));
        }
        sort(gph[i].begin(), gph[i].end());
    }
    cout << res;
}