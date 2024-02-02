#include <iostream>
#include <algorithm>
#include <cstring>

typedef long long ll;
const int LEN = 1e4;
const int V_LEN = 201;
const ll INF = 1e9;

int N, M;

struct Pos { ll x, y; ll f() const { return x * y; } };
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
    ll c = cross(d1, d2, d3);
    return c > 0 ? 1 : c < 0 ? -1 : 0;
}

int p[V_LEN];
int find(int i) { return p[i] < 0 ? i : p[i] = find(p[i]); }
int join(int a, int b) {
    a = find(a), b = find(b);
    if (a == b) return 0;
    if (p[a] < p[b]) p[a] += p[b], p[b] = a;
    else p[b] += p[a], p[a] = b;
    return 1;
}

ll A, B;

struct Edge {
    ll t, c;
    int u, v;
    ll f() const { return A * t + B * c; }
    bool operator<(const Edge& o) const { return f() < o.f(); }
} edges[LEN];

Pos mst(ll a, ll b) {
    A = a, B = b;
    Pos ret = { 0, 0 };
    std::sort(edges, edges + M);
    memset(p, -1, sizeof p);
    for (int i = 0, j = 0; i < M && j < N - 1; ++i) {
        if (join(edges[i].u, edges[i].v)) {
            ++j;
            ret.x += edges[i].t;
            ret.y += edges[i].c;
        }
    }
    return ret;
}

Pos min_cost;
ll min_a, min_b;

void f(const Pos& s, const Pos& e) {
    ll dx = e.x - s.x;
    ll dy = s.y - e.y;
    Pos m = mst(dy, dx);
    if (m.f() < min_cost.f()) {
        min_cost = m;
        min_a = dy, min_b = dx;
    }
    if (ccw(s, e, m) >= 0) return;
    f(s, m);
    f(m, e);
}

int main() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cin >> N >> M;
    for (int i = 0; i < M; ++i) std::cin >> edges[i].u >> edges[i].v >> edges[i].t >> edges[i].c;

    std::cout << "check\n";

    Pos s = mst(256, 1);
    Pos e = mst(1, 256);

    std::cout << "s: " << s.x << ' ' << s.y << '\n';
    std::cout << "e: " << e.x << ' ' << e.y << '\n';

    if (s.f() < e.f()) min_cost = s, min_a = 256, min_b = 1;
    else min_cost = e, min_a = 1, min_b = 256;
    f(s, e);
    std::cout << min_cost.x << ' ' << min_cost.y << '\n';
    A = min_a, B = min_b;
    std::sort(edges, edges + M);
    memset(p, -1, sizeof p);
    for (int i = 0, j = 0; i < M && j < N - 1; ++i) {
        if (join(edges[i].u, edges[i].v)) {
            ++j;
            std::cout << edges[i].u << ' ' << edges[i].v << '\n';
        }
    }
}