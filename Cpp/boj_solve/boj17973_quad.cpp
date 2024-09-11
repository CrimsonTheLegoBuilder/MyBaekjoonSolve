#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <cassert>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ll INF = 9e18;
const int LEN = 1e3;
const ld TOL = 1e-9;
const ld PI = acos(-1);
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }
inline int sign(const ld& x) { return x < -TOL ? -1 : x > TOL ? 1 : 0; }
inline bool zero(const ld& x) { return !sign(x); }

int N, E, OD[LEN], Q[LEN];
struct Pos {
	int x, y;
	Pos(int X = 0, int Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return x == p.x && y == p.y; }
	bool operator != (const Pos& p) const { return x != p.x || y != p.y; }
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const int& n) const { return { x * n, y * n }; }
	Pos operator / (const int& n) const { return { x / n, y / n }; }
	ll operator * (const Pos& p) const { return (ll)x * p.x + (ll)y * p.y; }
	ll operator / (const Pos& p) const { return (ll)x * p.y - (ll)y * p.x; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	ll Euc() const { return (ll)x * x + (ll)y * y; }
	friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
} P[LEN];
const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) { ll ret = cross(d1, d2, d3); return ret < 0 ? -1 : !!ret; }
bool same_halfplane(const Pos& p1, const Pos& p2, const Pos& p3, const Pos& p4) { return ccw(p1, p2, p3) == ccw(p1, p2, p4); }
struct Seg {
	short int u, v;//idx
	Seg(short int U = 0, short int V = 0) { u = U; v = V; if (P[v] < P[u]) std::swap(u, v); }
	Pos s() const { return P[v] - P[u]; }
	int ccw(const Pos& p0) const { return sign(cross(P[u], P[v], p0)); }
	bool operator < (const Seg& S) const {
		ll CCW = s() / S.s();
		return !CCW ? S.ccw(P[u]) > 0 : CCW < 0;
	}
	bool operator == (const Seg& S) const { return s() / S.s() == 0 && s() * S.s() > 0; }
} events[LEN * LEN];
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(10);
	memset(P, 0, sizeof P);
	memset(Q, 0, sizeof Q);
	memset(OD, 0, sizeof OD);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> P[i];
	std::sort(P, P + N);
	for (int i = 0; i < N; i++) OD[i] = i, Q[i] = i;
	E = 0;
	for (int i = 0; i < N; i++)
		for (int j = i + 1; j < N; j++)
			events[E++] = Seg(i, j);
	std::sort(events, events + E);
	ll quad = 0, cnt = 0;
	ll MIN = INF;
	for (int i = 0; i < E; i++) {
		int u = events[i].u, v = events[i].v;
		int ou = OD[u], ov = OD[v];
		OD[u] = ov; OD[v] = ou;
		Q[ou] = v; Q[ov] = u;
		assert(ov - ou == 1);
		quad += (ll)ou * (N - 1ll - ov);
		for (int j = std::max(0, ou - 2); j < ou; j++) {
			for (int k = ov + 1; k < std::min(N, ov + 3); k++) {
				ll A = std::abs(cross(P[Q[j]], P[Q[ou]], P[Q[ov]])) + std::abs(cross(P[Q[k]], P[Q[ou]], P[Q[ov]]));
				if (MIN > A) MIN = A, cnt = 0;
				if (MIN == A) cnt += 1ll + same_halfplane(P[Q[j]], P[Q[k]], P[Q[ou]], P[Q[ov]]);
			}
		}
	}
	std::cout << quad + cnt << "\n";
	return;
}
int main() { solve(); return 0; }//boj17973 Quadrilaterals  refer to JusticeHui