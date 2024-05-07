#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
#include <random>
#include <array>
#include <tuple>
#include <queue>
typedef long long ll;
typedef double ld;
const ll INF = 1e17;
const int LEN = 505;
const ld TOL = 1e-7;
const ll MOD = 1'000'000'007;
int N, M, T;
bool zero(const ld& x) { return std::abs(x) < TOL; }
int sign(const ld& x) { return x < -TOL ? -1 : x > TOL; }

ld COST[LEN];
struct Info {
	int i;
	ld c;
	Info(int I = 0, ld C = 0) : i(I), c(C) {}
	bool operator < (const Info& x) const { return c > x.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;
ld dijkstra(int v, int g) {
	for (int i = 0; i < LEN; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (COST[w.i] > cost) {
				COST[w.i] = cost;
				Q.push({ w.i, cost });
			}
		}
	}
	return COST[g];
}
struct Pos {
	int x, y;
	int i;
	bool good;
	Pos(int X = 0, int Y = 0, int I = 0, bool f = 0) : x(X), y(Y), i(I), good(f) {}
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
} pos[LEN]; const Pos O = Pos(0, 0);
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
typedef std::vector<Pos> Polygon;
std::vector<Pos> H[105];
ll cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ll cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
ll dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ll ret = cross(d1, d2, d3);
	return ret < 0 ? -1 : !!ret;
}
int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ll ret = cross(d1, d2, d3, d4);
	return ret < 0 ? -1 : !!ret;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) >= 0;
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	return !ccw(d1, d2, d3) && dot(d1, d3, d2) > 0;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
	//bool f3 = on_seg_strong(s1, s2, d1) ||
	//	on_seg_strong(s1, s2, d2) ||
	//	on_seg_strong(d1, d2, s1) ||
	//	on_seg_strong(d1, d2, s2);
	//return (f1 && f2) || f3;
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
void norm(std::vector<Pos>& H) { if (area(H) < 0) std::reverse(H.begin(), H.end()); }
bool inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y <= p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}

struct Pdd { ld x, y; Pdd(ld X = 0, ld Y = 0) : x(X), y(Y) {} };
Pdd mid(const Pos& p, const Pos& q) { return Pdd((p.x + q.x) * .5, (p.y + q.y) * .5); }
int ccw(const Pos& d1, const Pos& d2, const Pdd& d3) {
	ld ret = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return !ret ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pdd& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
bool on_seg_strong(const Pos& s1, const Pos& s2, const Pdd p) {
	ld ret = dot(s1, p, s2);
	return !ccw(s1, s2, p) && (ret > 0 || zero(ret));
}
bool inner_check(std::vector<Pos>& H, const Pdd& p) {//concave
	int cnt = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (cur.y == nxt.y) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}

bool blocked(const Pos& u, const Pos& v, const int& u_idx, const int& v_idx) {
	if (u_idx == v_idx) {
		int sz = H[u_idx].size();
		for (int i = 0; i < sz; i++) {
			Pos cur = H[u_idx][i], nxt = H[u_idx][(i + 1) % sz];
			if (intersect(u, v, cur, nxt)) return 1;
			if (on_seg_weak(u, v, cur)) return 1;
		}
		Pdd m = mid(u, v);
		return inner_check(H[u_idx], m);
	}
	else {
		for (int i = 0; i < N; i++) {
			int sz = H[i].size();
			for (int j = 0; j < sz; j++) {
				Pos cur = H[i][j], nxt = H[i][(j + 1) % sz];
				if (intersect(u, v, cur, nxt)) return 1;
				if (on_seg_weak(u, v, cur)) return 1;
			}
		}
		return 0;
	}
}
bool covered(const Pos& b, const int& h_idx, const int& p_idx) {
	bool r = 0, l = 0;
	int sz = H[h_idx].size();
	if (ccw(b, H[h_idx][p_idx], H[h_idx][(p_idx + 1) % sz]) > 0) return 1;
	if (ccw(b, H[h_idx][p_idx], H[h_idx][((p_idx - 1 + sz) % sz)]) < 0) return 1;
	Pos& p = H[h_idx][p_idx];
	for (int i = 0; i < N; i++) {
		sz = H[i].size();
		for (int j = 0; j < sz; j++) {
			Pos cur = H[i][j], nxt = H[i][(j + 1) % sz];
			if (intersect(b, p, cur, nxt)) return 1;
			if (on_seg_weak(b, p, cur) && ccw(b, p, cur, nxt) > 0) l = 1;
			if (on_seg_weak(b, p, cur) && ccw(b, p, cur, nxt) < 0) r = 1;
			if (on_seg_weak(b, p, nxt) && ccw(b, p, nxt, cur) > 0) l = 1;
			if (on_seg_weak(b, p, nxt) && ccw(b, p, nxt, cur) < 0) r = 1;
			if (l && r) return 1;
		}
	}
	return 0;
}

void init() {
	memset(pos, 0, sizeof pos);
	while (Q.size()) Q.pop();
	for (int i = 0; i < 100; i++) std::vector<Pos>().swap(H[i]);
	for (int i = 0; i < LEN; i++) std::vector<Info>().swap(G[i]);
	return;
}
ld query() {
	init();
	int goal = 0;
	Pos s, bomb;
	s = O; s.i = 1;
	std::cin >> bomb; bomb.i = -1;
	T = 1;
	int X = -1;//if Alice is located at the boundary of a polygon X = polygon.i
	for (int i = 0; i < N; i++) {
		std::cin >> M;
		H[i].resize(M);
		for (Pos& p : H[i]) std::cin >> p;
		norm(H[i]);
		for (Pos& p : H[i]) p.i = ++T;
		int f = inner_check(H[i], s);
#ifdef ASSERT
		assert(f < 2);
		assert(!inner_check(H[i], bomb));
#endif
		if (f) X = i;
	}
	if (blocked(bomb, s, -1, -2)) return 0;
	for (int i = 0; i < N; i++) {
		int sz = H[i].size();
		for (int j = 0; j < sz; j++) {
			if (covered(bomb, i, j)) G[H[i][j].i].push_back(Info(goal, 0));
		}
	}
	for (int i = 0; i < N; i++) {//O(500 * 500 * 100) = O(2500'0000)
		int szi = H[i].size();
		for (int j = 0; j < szi; j++) {
			Pos& u = H[i][j];
			if (!blocked(s, u, X, i)) {
				G[s.i].push_back(Info(u.i, (u - s).mag()));
			}
			for (int k = i; k < N; k++) {
				int szk = H[k].size();
				for (int l = 0; l < szk; l++) {
					Pos& v = H[k][l];
					if (!blocked(u, v, i, k)) {
						G[u.i].push_back(Info(v.i, (u - v).mag()));
						G[v.i].push_back(Info(u.i, (u - v).mag()));
					}
				}
			}
		}
	}
	ld cost = dijkstra(s.i, goal);
	return cost;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	while (1) {
		std::cin >> N;
		if (!N) break;
		std::cout << query() << "\n";
	}
	return;
}
int main() { solve(); return 0; }//JAG Practice Contest 2010 E boj13801
