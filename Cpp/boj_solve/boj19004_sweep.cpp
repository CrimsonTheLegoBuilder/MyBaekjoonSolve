#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <deque>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef double ld;
typedef std::vector<int> Vint;
typedef std::vector<ll> Vll;
typedef std::vector<ld> Vld;
const int LEN = 1e5 + 5;
ll gcd(ll a, ll b) { while (b) { ll tmp = a % b; a = b; b = tmp; } return a; }
ll gcd(ll a, ll b, ll c) { a = std::abs(a); b = std::abs(b); c = std::abs(c); a = gcd(a, b); return gcd(a, c); }
void norm(ll& x, ll& y) {
	ll gcd_ = gcd(std::abs(x), std::abs(y));
	x /= gcd_; y /= gcd_;
	return;
}

int T, N, CNT[4];
struct Pos {
	ll x, y;
	Pos(ll x_ = 0, ll y_ = 0) : x(x_), y(y_) {}
	bool operator == (const Pos& q) const { return x == q.x && y == q.y; }
};
struct FracPos {
	ll x, y;
	ll dx, dy;
	int qd, i, f;
	FracPos(ll x_ = 0, ll dx_ = 0, ll y_ = 0, ll dy_ = 0, int qd_ = 0, int f_ = 0) : x(x_), y(y_), dx(dx_), dy(dy_), qd(qd_), f(f_) {}
	ll mod_x() const { return std::abs(x) % std::abs(dx); }
	ll mod_y() const { return std::abs(y) % std::abs(dy); }
	bool x_int() const { return dx != 0 && !mod_x(); }
	ll x_() const { return x / dx; }
	bool y_int() const { return dy != 0 && !mod_y(); }
	ll y_() const { return y / dy; }
	bool is_int() const { return x_int() && y_int(); }
	Pos p() const { return Pos(x / dx, y / dy); }
} P1, P2, P3, P4 ;
typedef std::vector<FracPos> Polygon;
bool x_eq(const FracPos& p, const FracPos& q) { return p.x * q.dx == q.x * p.dx; }
bool y_eq(const FracPos& p, const FracPos& q) { return p.y * q.dy == q.y * p.dy; }
bool eq(const FracPos& p, const FracPos& q) { return x_eq(p, q) && y_eq(p, q); }
bool comp(const FracPos& p, const FracPos& q) {
	if (p.qd != q.qd) return p.qd < q.qd;
	//if (eq(p, q)) return p.f < q.f;
	if (p.qd == 0) return p.x * q.dx > q.x * p.dx;
	if (p.qd == 1) return p.y * q.dy > q.y * p.dy;
	if (p.qd == 2) return p.x * q.dx < q.x * p.dx;
	if (p.qd == 3) return p.y * q.dy < q.y * p.dy;
	assert(0);
	return 0;
}
struct Line {
	ll a, b;
	Line(ll a_ = 0, ll b_ = 0) : a(a_), b(b_) {}
} L[LEN];
struct Seg {
	int l, r;
	bool operator<(const Seg& o) const { return l == o.l ? r < o.r : l < o.l; }
} SEG[LEN]; int sp;
Polygon intersections(const Line& l, const FracPos& p1, const FracPos& p2) {
	FracPos a, b, c, d;
	ll x1 = p2.y - l.b, d1 = l.a; norm(x1, d1);
	a = FracPos(x1, d1, p2.y, 1);
	if (x1 <= p1.x * d1 || p2.x * d1 < x1) a.dx = a.dy = 0;
	ll x3 = p1.y - l.b, d3 = l.a; norm(x3, d3);
	c = FracPos(x3, d3, p1.y, 1);
	if (x3 < p1.x * d3 || p2.x * d3 <= x3) c.dx = c.dy = 0;
	ll y2 = p1.x * l.a + l.b;
	b = FracPos(p1.x, 1, y2, 1);
	if (b.y_() <= p1.y || p2.y < b.y_()) b.dx = b.dy = 0;
	ll y4 = p2.x * l.a + l.b;
	d = FracPos(p2.x, 1, y4, 1);
	if (d.y_() < p1.y || p2.y <= d.y_()) d.dx = d.dy = 0;
	a.qd = 0, b.qd = 1, c.qd = 2, d.qd = 3;
	//if (a.is_int() && (b.is_int() && a.p() == b.p())) a.dx = a.dy = 0;
	//if (b.is_int() && (c.is_int() && b.p() == c.p())) b.dx = b.dy = 0;
	//if (c.is_int() && (d.is_int() && c.p() == d.p())) c.dx = c.dy = 0;
	//if (d.is_int() && (a.is_int() && d.p() == a.p())) d.dx = b.dy = 0;
	Polygon v;
	if (a.dx) v.push_back(a);
	if (b.dx) v.push_back(b);
	if (c.dx) v.push_back(c);
	if (d.dx) v.push_back(d);
	return v;
}

ll fenwick[LEN * 2];
ll sum(int idx) {
	ll s = 0;
	while (idx > 0) {
		s += fenwick[idx];
		idx -= idx & -idx;
	}
	return s;
}
void update(int idx, int diff) {
	while (idx < LEN * 2) {
		fenwick[idx] += diff;
		idx += idx & -idx;
	}
}

void query() {
	std::cin >> N >> P3.x >> P3.y >> P1.x >> P1.y;
	/*
	P2 -- P1
	|      |
	P3 -- P4
	*/
	P2.x = P3.x; P2.y = P1.y; P4.x = P1.x; P4.y = P3.y;
	memset(CNT, 0, sizeof CNT);
	for (int i = 0; i <= N * 2; ++i) fenwick[i] = 0;
	for (int i = 0; i < N; ++i) SEG[i].l = SEG[i].r = 0;
	for (int i = 0; i < N; i++) std::cin >> L[i].a >> L[i].b;
	std::priority_queue<Seg> PQ;
	Polygon P;
	for (int i = 0; i < N; i++) {
		Polygon V = intersections(L[i], P3, P1);
		if (V.empty()) continue;
		if (V.size() == 1) {
			P.push_back(V[0]);
			P.back().i = i;
			P.push_back(V[0]);
			P.back().i = i; P.back().f = 1;
		}
		if (V.size() == 2) {
			P.push_back(V[0]);
			P.back().i = i;
			P.push_back(V[1]);
			P.back().i = i; P.back().f = 1;
		}
	}
	std::sort(P.begin(), P.end(), comp);
	// for (int i = 0; i < P.size(); ++i) std::cout << "index: " << P[i].i << ' ' << P[i].f << '\n';

	//assert(P[0].f == 0);
	SEG[P[0].i].l = 1;
	for (int i = 1, idx = 1; i < P.size(); ++i) {
		if (!eq(P[i], P[i - 1])) idx++;
		// std::cout << i << ' ' << P[i].i << ' ' << P[i].f << ' ' << idx << '\n';
		(!P[i].f ? SEG[P[i].i].l : SEG[P[i].i].r) = idx;
	}
	// for (int i = 0; i < N; ++i) std::cout << SEG[i].l << ' ' << SEG[i].r << '\n';
	std::sort(SEG, SEG + N);
	
	ll ret = 0;
	for (int i = 0; i < N; ++i) {
		if (!SEG[i].l) continue;
		ret += sum(SEG[i].r) - sum(SEG[i].l - 1);
		update(SEG[i].r, 1);
	}
	std::cout << ret << '\n';
}
void solve() {
	P1.dx = 1; P1.dy = 1; P2.dx = 1; P2.dy = 1;
	P3.dx = 1; P3.dy = 1; P4.dx = 1; P4.dy = 1;
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> T;
	while (T--) query();
	return;
}
int main() { solve(); return 0; }