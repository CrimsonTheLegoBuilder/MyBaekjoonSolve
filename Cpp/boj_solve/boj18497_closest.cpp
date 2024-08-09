#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
typedef long double ld;
const ll INF = 1e18;
const int LEN = 300;
const ld TOL = 1e-9;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL ? 1 : 0; }
inline bool zero(const ld& x) { return !sign(x); }
inline void norm(ld x) {
	while (sign(x) < 0) x += 2 * PI;
	while (sign(x - 2 * PI) >= 0) x -= 2 * PI;
	return;
}

int N, M, E, order[LEN], Q[LEN];
ld ANS[LEN][LEN];
struct Pos {
	ll x, y;
	Pos(ll X = 0, ll Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	bool operator <= (const Pos& p) const { return x == p.x ? y <= p.y : x <= p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return x * p.x + y * p.y; }
	ll operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const int& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const int& scale) { x /= scale; y /= scale; return *this; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll xy() const { return x * y; }
	ll Euc() const { return x * x + y * y; }
	int Man() const { return std::abs(x) + std::abs(y); }
	ld mag() const { return hypot(x, y); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return y > 0 || y == 0 && x >= 0; }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN];
const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, INVAL = { -1, -1 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { ll ret = cross(d1, d2, d3, d4); return ret < 0 ? -1 : !!ret; }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d1) / (d2 - d1).mag(); }
ld projection(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) * (d4 - d3) / (d2 - d1).mag(); }
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
struct Seg {
	//Pos u, v;
	int u, v;//idx
	Seg(int U = -1, int V = -1) : u(U), v(V) {}
	Pos s() const { return P[v] - P[u]; }
	Pos p() const { return Pos(u, v); }
	int ccw(const Pos& p) const { return sign(cross(P[u], P[v], p)); }
	bool operator < (const Seg& S) const {
		bool f1 = O < s();
		bool f2 = O < S.s();
		if (f1 != f2) return f1;
		ll CCW = s() / S.s();
		return !CCW ? !S.ccw(P[u]) ? p() < S.p() : S.ccw(P[u]) > 0 : CCW > 0;
	}
	bool operator == (const Seg& S) const { return s() / S.s() == 0 && s() * S.s() > 0; }
	ld ang(const Seg& S) const { return rad(S.s(), s()); }
} events[LEN * LEN];
struct Slope {//segment's two point and slope
	int u, v;//idx
	ll dx, dy;
	Pos p() const { return { u, v }; }
	bool operator < (const Slope& s) const {
		if (dy * s.dx == dx * s.dy) return p() < s.p();
		return dy * s.dx < dx * s.dy;
	}
	bool operator == (const Slope& s) const { return dy * s.dx == dx * s.dy; }
} slopes[LEN * LEN];
ld dist(const Pos& p, const Pos& q) { return (p - q).mag(); }
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(order, 0, sizeof order);
	memset(Q, 0, sizeof Q);
	memset(P, 0, sizeof P);
	std::cin >> N;

	for (int i = 0; i < N; i++) std::cin >> P[i];
	std::sort(P, P + N);
	for (int i = 0; i < N; i++) order[i] = i, Q[i] = i;

	if (N == 2) { std::cout << "1.0000000\n"; return; }

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			ANS[i][j] = -INF;

	E = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			events[E++] = Seg(i, j);
			events[E++] = Seg(j, i);
		}
	}
	std::sort(events, events + E);

	Pos U, V;
	auto cmps = [&](const int& p, const int& q) -> bool {
		const Pos& p_ = P[p], & q_ = P[q];
		ll tp = cross(U, V, p_), tq = cross(U, V, q_);
		if (tp == tq) return dot(U, V, p_) < dot(U, V, q_);
		return tp < tq;
		};
	auto x_dist = [&](const int& p, const int& q) -> ld {
		const Pos& p_ = P[p], & q_ = P[q];
		Pos vec = V - U;
		ll tq = cross(p_, p_ + vec, q_);
		return tq / (U - V).mag();
		};

	U = P[events[0].u];
	V = P[events[0].v];
	std::sort(Q, Q + N, cmps);
	for (int i = 0; i < N; i++) order[Q[i]] = i;

	ld ans = INF;
	for (int i = 0; i < N; i++) {
		for (int j = i - 1; j >= 0; j--) {
			ANS[Q[i]][Q[j]] = ans;
			ans = std::min(ans, dist(P[Q[i]], P[Q[j]]));
		}
	}

	int cnt = 0;
	ld total = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (x_dist(i, j) < ANS[i][j]) cnt++;
		}
	}

	Pos pre_s = events[(E - 1 + E) % E].s();
	for (int i = 0, j; i < E; i = j) {
		j = i;
		int s = order[events[i].u], e = order[events[i].u];
		while (j < E && events[i] == events[j]) {
			int u = events[j].u, v = events[j].v;
			if (e < order[v]) e = order[v];
			int ou = order[u], ov = order[v];



			order[u] = ov; order[v] = ou;
			Q[ou] = v; Q[ov] = u;
			j++;
		}
	}

	std::cout << total / (2 * PI) << "\n";
	
	return;
}
int main() { solve(); return 0; }//boj18497
