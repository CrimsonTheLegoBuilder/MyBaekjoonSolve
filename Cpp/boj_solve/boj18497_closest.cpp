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

int N, M, order[LEN], idx[LEN], ANS[LEN][LEN];
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
	Pos u, v;
	Pos s() const { return v - u; }
	int ccw(const Pos& p) const { return sign(cross(u, v, p)); }
	bool operator < (const Seg& S) const {
		bool f1 = O < s();
		bool f2 = O < S.s();
		if (f1 != f2) return f1;
		ll CCW = s() / S.s();
		return !CCW ? ccw(S.u) < 0 : CCW > 0;
	}
	bool operator == (const Seg& S) const { return s() / S.s() == 0 && s() * S.s() > 0; }
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
	while (1) {
		memset(order, 0, LEN);
		memset(idx, 0, LEN);
		memset(P, 0, LEN);
		std::cin >> N;
		if (!N) return;

		for (int i = 0; i < N; i++) std::cin >> P[i].x >> P[i].y;
		std::sort(P, P + N);

		for (int i = 0; i < N; i++) order[i] = i, idx[i] = i;
		M = 0;
		ll MIN = INF, MAX = -INF;
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if (P[i].x == P[j].x) {
					if (i > 0) {
						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[i - 1])) });
						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[0])) });
					}
					else if (j < N - 1) {
						MIN = std::min({ MIN, std::abs(cross(P[i], P[j], P[j + 1])) });
						MAX = std::max({ MAX, std::abs(cross(P[i], P[j], P[N - 1])) });
					}
				}
				ll dx = P[j].x - P[i].x;
				ll dy = P[j].y - P[i].y;
				if (!dx) continue;
				if (dx * dy >= 0) dx = std::abs(dx), dy = std::abs(dy);
				else dx = std::abs(dx), dy = -std::abs(dy);
				slopes[M++] = { i, j, dx, dy };
			}
		}
		std::sort(slopes, slopes + M);

		for (int i = 0, j; i < M; i = j) {
			j = i;
			int s = order[slopes[i].u], e = order[slopes[i].u];
			while (j < M && slopes[i] == slopes[j]) {
				int u = slopes[j].u, v = slopes[j].v;
				if (e < order[v]) e = order[v];
				int ou = order[u], ov = order[v];
				if (ou > 0) MIN = std::min(MIN, std::abs(cross(P[u], P[v], P[idx[ou - 1]])));
				if (ov < N - 1) MIN = std::min(MIN, std::abs(cross(P[u], P[v], P[idx[ov + 1]])));
				order[u] = ov; order[v] = ou;
				idx[ou] = v; idx[ov] = u;
				j++;
			}
			if (s == 0 && e == N - 1) MAX = 0;
			if (s > 0) MAX = std::max(MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[0]])));
			if (e < N - 1) MAX = std::max(MAX, std::abs(cross(P[idx[s]], P[idx[e]], P[idx[N - 1]])));
		}
		//std::cout << N << "\n";
		std::cout << (MIN >> 1) << '.' << (MIN & 1 ? "5 " : "0 ");
		std::cout << (MAX >> 1) << '.' << (MAX & 1 ? "5\n" : "0\n");
	}
	return;
}
int main() { solve(); return 0; }//boj9484
