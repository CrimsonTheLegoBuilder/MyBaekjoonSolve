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
		if (p.i == g) return COST[g];
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
	ld x, y;
	int i;
	bool good;
	Pos(ld X = 0, ld Y = 0, int I = 0, bool f = 0) : x(X), y(Y), i(I), good(f) {}
	inline bool operator == (const Pos& p) const { return zero(x - p.x) && zero(y - p.y); }
	bool operator != (const Pos& p) const { return !zero(x - p.x) || !zero(y - p.y); }
	bool operator < (const Pos& p) const { return zero(x - p.x) ? y < p.y : x < p.x; }
	inline Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	inline Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	inline Pos operator * (const ld& scalar) const { return { x * scalar, y * scalar }; }
	Pos operator / (const ld& scalar) const { return { x / scalar, y / scalar }; }
	inline ld operator * (const Pos& p) const { return x * p.x + y * p.y; }
	inline ld operator / (const Pos& p) const { return x * p.y - y * p.x; }
	Pos operator ^ (const Pos& p) const { return { x * p.x, y * p.y }; }
	Pos operator - () const { return { -x, -y }; }
	Pos operator ~ () const { return { -y, x }; }
	Pos operator ! () const { return { y, x }; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator -= (const Pos& p) { x -= p.x; y -= p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	Pos& operator /= (const ld& scale) { x /= scale; y /= scale; return *this; }
	ld xy() const { return x * y; }
	Pos rot(ld the) { return { x * cos(the) - y * sin(the), x * sin(the) + y * cos(the) }; }
	inline ld Euc() const { return x * x + y * y; }
	//inline ld mag() const { return sqrt(Euc()); }
	inline ld mag() const { return hypot(x, y); }
	Pos unit() const { return *this / mag(); }
	ld rad() const { return atan2(y, x); }
	friend ld rad(const Pos& p1, const Pos& p2) { return atan2l(p1 / p2, p1 * p2); }
	int quad() const { return sign(y) == 1 || (sign(y) == 0 && sign(x) >= 0); }
	friend bool cmpq(const Pos& a, const Pos& b) { return (a.quad() != b.quad()) ? a.quad() < b.quad() : a / b > 0; }
	bool close(const Pos& p) const { return zero((*this - p).Euc()); }
	inline friend std::istream& operator >> (std::istream& is, Pos& p) { is >> p.x >> p.y; return is; }
	friend std::ostream& operator << (std::ostream& os, const Pos& p) { os << p.x << " " << p.y; return os; }
}; const Pos O = { 0, 0 };
typedef std::vector<Pos> Polygon;
bool cmpx(const Pos& p, const Pos& q) { return p.x == q.x ? p.y < q.y : p.x < q.x; }
bool cmpy(const Pos& p, const Pos& q) { return p.y == q.y ? p.x < q.x : p.y < q.y; }
bool cmpi(const Pos& p, const Pos& q) { return p.i < q.i; }
Polygon H[105];
struct Vec {
	ld vy, vx;
	Vec(ld Y = 0, ld X = 0) : vy(Y), vx(X) {}
	bool operator == (const Vec& v) const { return (zero(vy - v.vy) && zero(vx - v.vx)); }
	bool operator < (const Vec& v) const { return zero(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	ld operator * (const Vec& v) const { return vy * v.vy + vx * v.vx; }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
	Vec& operator *= (const ld& scalar) { vy *= scalar; vx *= scalar; return *this; }
	Vec& operator /= (const ld& scalar) { vy /= scalar; vx /= scalar; return *this; }
	ld mag() const { return hypot(vy, vx); }
}; const Vec Zero = { 0, 0 };
struct Line {//ax + by = c
	Vec s;
	ld c;
	Line(Vec V = Vec(0, 0), ld C = 0) : s(V), c(C) {}
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return zero(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator * (const Line& l) const { return s * l.s; }
	ld operator / (const Line& l) const { return s / l.s; }
	Line operator + (const ld& scalar) const { return Line(s, c + hypot(s.vy, s.vx) * scalar); }
	Line operator - (const ld& scalar) const { return Line(s, c - hypot(s.vy, s.vx) * scalar); }
	Line operator * (const ld& scalar) const { return Line({ s.vy * scalar, s.vx * scalar }, c * scalar); }
	Line& operator += (const ld& scalar) { c += hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator -= (const ld& scalar) { c -= hypot(s.vy, s.vx) * scalar; return *this; }
	Line& operator *= (const ld& scalar) { s *= scalar, c *= scalar; return *this; }
	ld dist(const Pos& p) const { return s.vy * p.x + s.vx * p.y; }
	ld above(const Pos& p) const { return s.vy * p.x + s.vx * p.y - c; }
	ld mag() const { return s.mag(); }
	friend std::ostream& operator << (std::ostream& os, const Line& l) { os << l.s.vy << " " << l.s.vx << " " << l.c; return os; }
};
inline Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return Line(Vec(dy, dx), c);
}
inline Line L(const Vec& s, const Pos& p) {
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
inline Line rotate(const Line& l, const Pos& p, ld the) {
	Vec s = l.s;
	ld x = -s.vx, y = s.vy;
	ld vx = -(x * cos(the) - y * sin(the));
	ld vy = x * sin(the) + y * cos(the);
	ld c = vy * p.x + vx * p.y;
	return Line(Vec(vy, vx), c);
}
inline Line rot90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return Line(s, c);
}
inline Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
	};
}
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
inline ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
inline ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	ld ret = cross(d1, d2, d3, d4);
	return zero(ret) ? 0 : ret > 0 ? 1 : -1;
}
inline bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || zero(ret));
}
inline bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
}
inline bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
	//bool f3 = on_seg_strong(s1, s2, d1) ||
	//	on_seg_strong(s1, s2, d2) ||
	//	on_seg_strong(d1, d2, s1) ||
	//	on_seg_strong(d1, d2, s2);
	//return (f1 && f2) || f3;
}
inline ld area(std::vector<Pos>& H) {
	ld ret = 0;
	int sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		ret += cross(O, cur, nxt);
	}
	return ret;
}
inline bool norm(std::vector<Pos>& H) { 
	if (area(H) < TOL) {
		std::reverse(H.begin(), H.end());
		return 0;
	}
	return 1;
}
inline int inner_check(const std::vector<Pos>& H, const Pos& p) {//concave
	int cnt = 0, sz = H.size();
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (zero(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return (cnt & 1) * 2;
}
inline int on_seg_check(const Pos& p) {
	for (int i = 0; i < N; i++) {
		int sz = H[i].size();
		for (int j = 0; j < sz; j++) {
			Pos& a = H[i][j], b = H[i][(j + 1) % sz];
			if (on_seg_strong(a, b, p)) return i;
		}
	}
	return -2;
}
inline bool meaningless(const Pos& b, const Pos& p) {
	bool r = 0, l = 0;
	for (int i = 0; i < N; i++) {
		int sz = H[i].size();
		for (int j = 0; j < sz; j++) {
			Pos cur = H[i][j], nxt = H[i][(j + 1) % sz];
			if (intersect(b, p, cur, nxt)) return 1;
			if (on_seg_weak(b, p, cur) && ccw(b, p, cur, nxt) > 0) l = 1;
			if (on_seg_weak(b, p, cur) && ccw(b, p, cur, nxt) < 0) r = 1;
			if (on_seg_weak(b, p, nxt) && ccw(b, p, nxt, cur) > 0) l = 1;
			if (on_seg_weak(b, p, nxt) && ccw(b, p, nxt, cur) < 0) r = 1;
			if (r && l) return 1;
		}
	}
	return 0;
}
inline bool blocked(const Pos& u, const Pos& v, const int& u_idx, const int& v_idx) {
	if (u_idx == v_idx) {
		int sz = H[u_idx].size();
		for (int i = 0; i < sz; i++) {
			Pos cur = H[u_idx][i], nxt = H[u_idx][(i + 1) % sz];
			if (intersect(u, v, cur, nxt)) return 1;
			if (on_seg_weak(u, v, cur)) return 1;
		}
		Pos m = (u + v) * .5;
		//std::cout << "m :  " << m << "\n";
		//std::cout << inner_check(H[u_idx], m) << "\n";
		return inner_check(H[u_idx], m) > 1;
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
inline bool covered(const Pos& b, const int& h_idx, const int& p_idx) {
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
			if (r && l) return 1;
		}
	}
	return 0;
}
inline void init() {
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
		assert(norm(H[i]));
		for (Pos& p : H[i]) p.i = ++T;
		int f = inner_check(H[i], s);
#ifdef ASSERT
		assert(f < 2);
		assert(!inner_check(H[i], bomb));
#endif
		if (f) X = i;
	}
	int f1 = 0;
	for (int i = 0; i < N; i++) {
		int sz = H[i].size();
		for (int j = 0; j < sz; j++) {
			if (covered(bomb, i, j)) H[i][j].good = 1;
			if (H[i][j].good && H[i][j] == s) f1 = 1;
		}
	}

	//std::cout << "DEBUG\n";
	//for (Pos& p : H[0]) std::cout << p << " " << p.good << "\n";
	//std::cout << "DEBUG\n";

	if (f1 || blocked(bomb, s, -1, -2)) return 0;

	Pos inx;
	Line safe, sht, seg;//safe line, short, segment
	for (int i = 0; i < N; i++) {
		int szi = H[i].size();
		for (int j = 0; j < szi; j++) {//O(500)
			Pos& p = H[i][j];
			if (p.good) {
				G[p.i].push_back(Info(goal, 0));
				safe = L(bomb, p);
				sht = rot90(safe, s);
				inx = intersection(safe, sht);
				if (!meaningless(bomb, inx)) {
					if (on_seg_weak(bomb, inx, p)) {
						inx.i = on_seg_check(inx);
						if (inx.i == X) {
							if (!blocked(s, inx, -1, inx.i) && !blocked(s, inx, X, inx.i))
								G[1].push_back(Info(goal, (s - inx).mag()));
						}
						else {
							if (!blocked(s, inx, X, inx.i))
								G[1].push_back(Info(goal, (s - inx).mag()));
						}
					}
				}

				for (int k = 0; k < N; k++) {
					int szk = H[k].size();
					for (int l = 0; l < szk; l++) {//(O(250 * 250)
						Pos& v = H[k][l];
						if (v.good) continue;

						sht = rot90(safe, v);
						inx = intersection(safe, sht);
						if (meaningless(bomb, inx)) continue;

						if (on_seg_weak(bomb, inx, p)) {//O(250 * 250 * 500)
							inx.i = on_seg_check(inx);
							if (inx.i == k) {
								if (!blocked(v, inx, -1, k) && !blocked(v, inx, k, inx.i)) {
									G[v.i].push_back(Info(goal, (v - inx).mag()));
								}
							}
							else {
								if (!blocked(v, inx, k, inx.i)) {
									G[v.i].push_back(Info(goal, (v - inx).mag()));
								}
							}
						}
					}
				}

				for (int k = 0; k < N; k++) {
					int szk = H[k].size();
					for (int l = 0; l < szk; l++) {//O(250 * 250)
						Pos& a = H[k][l], b = H[k][(l + 1) % szk];
						seg = L(a, b);
						if (zero(safe / seg)) continue;

						inx = intersection(safe, seg);
						if (!on_seg_weak(a, b, inx)) continue;
						if (!on_seg_weak(bomb, inx, p)) continue;
						if (meaningless(bomb, inx)) continue;

						inx.i = on_seg_check(inx);
						if (inx.i == X) {//O(250 * 250 * 500)
							if (!blocked(s, inx, -1, inx.i) && !blocked(s, inx, X, inx.i))
								G[1].push_back(Info(goal, (s - inx).mag()));
						}
						else {
							if (!blocked(s, inx, X, inx.i))
								G[1].push_back(Info(goal, (s - inx).mag()));
						}
						for (int m = 0; m < N; m++) {
							int szm = H[m].size();
							for (int n = 0; n < szm; n++) {//O(250 * 250 * 250)
								Pos& v = H[m][n];
								if (v.good) continue;
								if (inx.i == m) {//O(250 * 250 * 250 * 500)
									if (!blocked(v, inx, -1, m) && !blocked(v, inx, m, inx.i)) {
										G[v.i].push_back(Info(goal, (v - inx).mag()));
									}
								}
								else {
									if (!blocked(v, inx, m, inx.i)) {
										G[v.i].push_back(Info(goal, (v - inx).mag()));
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < N; i++) {
		int szi = H[i].size();
		for (int j = 0; j < szi; j++) {//O(500)
			Pos& u = H[i][j];
			if (i == X) {
				if (!blocked(s, u, X, i) && !blocked(s, u, -1, -2)) {
					G[1].push_back(Info(u.i, (u - s).mag()));
					//std::cout << "fucked\n";
					//std::cout << u << "\n";
				}
			}
			else {
				if (!blocked(s, u, X, i)) {
					G[1].push_back(Info(u.i, (u - s).mag()));
				}
			}
			for (int k = i; k < N; k++) {
				int szk = H[k].size();
				for (int l = 0; l < szk; l++) {//O(500 * 500)
					Pos& v = H[k][l];
					if (u.good && v.good) continue;
					if (i == k) {
						if (!blocked(u, v, i, k) && !blocked(u, v, -1, -2)) {
							G[u.i].push_back(Info(v.i, (u - v).mag()));
							G[v.i].push_back(Info(u.i, (u - v).mag()));
						}
					}
					else {
						if (!blocked(u, v, i, k)) {
							G[u.i].push_back(Info(v.i, (u - v).mag()));
							G[v.i].push_back(Info(u.i, (u - v).mag()));
						}
					}
				}
			}
		}
	}
	//std::cout << X << "\n";
	//for (int i = 0; i < T; i++) {
	//	for (Info& i : G[i]) std::cout << i.i << " " << i.c << "\n";
	//	std::cout << "DEBUG\n";
	//}
	ld cost = dijkstra(s.i, goal);
	return cost;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(15);
	while (1) {
		std::cin >> N;
		if (!N) break;
		std::cout << query() << "\n";
	}
	return;
}
int main() { solve(); return 0; }//JAG Practice Contest 2010 E boj13801


/*

1
1 1
4 -1 0 -2 -1 -1 -2 0 -1
1
0 3
4 1 1 1 2 -1 2 -1 1
1
-6 -6
6 1 -2 2 -2 2 3 -2 3 -2 1 1 1
1
6 4
6 5 -498 6 2 5 2 5 1 3 1 -999 -498
1
-10 0
4 0 -5 1 -5 1 5 0 5
1
-10 0
4 0 0 2 -2 1 0 2 2
1
-10 0
4 1 0 3 -2 2 0 3 2
1
-5 -1
4 -10 -1 -9 0 -10 1 -11 0
0

1.0
0.0
3.23606798
4.66227766
5.0000000000
2.8284271247
3.6055512755
10.0498756211

*/
