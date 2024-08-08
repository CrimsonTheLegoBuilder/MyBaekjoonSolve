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
inline int sign(const ll& x) { return x < 0 ? -1 : !!x; }

int N, M, E, order[LEN], idx[LEN];
ld ANS[LEN][LEN];
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
} P[LEN]; const Pos O = Pos(0, 0);
const Pos O = { 0, 0 }, MAXL = { 0, INF }, MAXR = { INF, 0 }, pivot = { -1, -1 };
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
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
		return !CCW ? !ccw(S.u) ? p() < S.p(): ccw(S.u) < 0 : CCW > 0;
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
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	//freopen("boj9484_triangle_in.txt", "r", stdin);
	//freopen("boj9484_triangle_out.txt", "w", stdout);
	memset(order, 0, LEN);
	memset(idx, 0, LEN);
	memset(P, 0, LEN);
	std::cin >> N;

	for (int i = 0; i < N; i++) std::cin >> P[i];
	std::sort(P, P + N);
	for (int i = 0; i < N; i++) order[i] = i, idx[i] = i;

	M = 0;
	E = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i + 1; j < N; j++) {
			ANS[i][j] = -INF;
			events[E++] = Seg(i, j);
			events[E++] = Seg(j, i);
		}
	}
	std::sort(events, events + E);

	ld ans = INF;
	for (int i = 0; i < N; i++) {
		for (int j = i - 1; j >= 0; j--) {
			ANS[idx[i]][idx[j]] = ans;
			ans = std::min(ans, (P[idx[i]] - P[idx[j]]).mag());
		}
	}

	int cnt = 0;
	ld total = 0;

	for (int i = 0, j; i < E; i = j) {
		j = i;
		int s = order[events[i].u], e = order[events[i].u];
		while (j < E && events[i] == events[j]) {
			int u = events[j].u, v = events[j].v;
			if (e < order[v]) e = order[v];
			int ou = order[u], ov = order[v];



			order[u] = ov; order[v] = ou;
			idx[ou] = v; idx[ov] = u;
			j++;
		}
	}
	
	return;
}
int main() { solve(); return 0; }//boj18497
