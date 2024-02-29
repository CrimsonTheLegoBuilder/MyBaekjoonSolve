#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-8;
const ld INF = 1e17;
const int LEN = 102;
int N, t;

ld COST[LEN];
struct Info {
	int i;
	ld c;
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
bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, 0 }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, 0 }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, 0 }; }
	Pos operator / (const ld& n) const { return { x / n, y / n, 0 }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, 0 }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} gallery[LEN], nodes[LEN], sculpture{ 0, 0, 0 }, guard{ 0, 0, 1 };// const Pos O = { 0, 0, -1 };
struct Vec {
	ld vy, vx;
	bool operator < (const Vec& v) const { return z(vy - v.vy) ? vx < v.vx : vy < v.vy; }
	bool operator == (const Vec& v) const { return (z(vy - v.vy) && z(vx - v.vx)); }
	ld operator / (const Vec& v) const { return vy * v.vx - vx * v.vy; }
	Vec operator ~ () const { return { -vx, vy }; }
}; const Vec Zero = { 0, 0 };
struct Line {
	Vec s;
	ld c;
	bool operator < (const Line& l) const {
		bool f1 = Zero < s;
		bool f2 = Zero < l.s;
		if (f1 != f2) return f1;
		ld CCW = s / l.s;
		return z(CCW) ? c * hypot(l.s.vy, l.s.vx) < l.c * hypot(s.vy, s.vx) : CCW > 0;
	}
	ld operator / (const Line& l) const { return s / l.s; }
};
Line L(const Pos& s, const Pos& e) {
	ld dy, dx, c;
	dy = e.y - s.y;
	dx = s.x - e.x;
	c = dy * s.x + dx * s.y;
	return { { dy, dx } , c };
}
Line rotate90(const Line& l, const Pos& p) {
	Vec s = ~l.s;
	ld c = s.vy * p.x + s.vx * p.y;
	return { s, c };
}
Pos intersection(const Line& l1, const Line& l2) {
	Vec v1 = l1.s, v2 = l2.s;
	ld det = v1 / v2;
	return {
		(l1.c * v2.vx - l2.c * v1.vx) / det,
		(l2.c * v1.vy - l1.c * v2.vy) / det,
		0
	};
}
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && (ret > 0 || z(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return z(cross(d1, d2, d3)) && ret > 0;
}
bool inner_check(Pos H[], const int& sz, const Pos& p) {
	int cnt{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (on_seg_strong(cur, nxt, p)) return 1;
		if (z(cur.y - nxt.y)) continue;
		if (nxt.y < cur.y) std::swap(cur, nxt);
		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
		cnt += ccw(cur, nxt, p) > 0;
	}
	return cnt & 1;
}
bool intersect(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	return f1 && f2;
}
bool blocked(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f1 = ccw(s1, s2, d1) * ccw(s2, s1, d2) > 0;
	bool f2 = ccw(d1, d2, s1) * ccw(d2, d1, s2) > 0;
	bool f3 = on_seg_weak(s1, s2, d1) || on_seg_weak(s1, s2, d2);
	return (f1 && f2) || f3;
}
bool blocked(Pos H[], const int& sz, const Pos& s1, const Pos& s2) {
	for (int i = 0; i < sz; i++) if (blocked(s1, s2, H[i], H[(i + 1) % sz])) return 1;
	return 0;
}
bool invisible(Pos H[], const int& sz, const Pos& p, const Pos& target) {
	bool l{ 0 }, r{ 0 };
	for (int i = 0; i < sz; i++) {
		Pos cur = H[i], nxt = H[(i + 1) % sz];
		if (!ccw(p, target, cur) && !ccw(p, target, nxt)) continue;
		if (intersect(p, target, cur, nxt)) return 1;
		if (on_seg_weak(p, target, cur) && ccw(p, target, nxt) > 0) l = 1;
		if (on_seg_weak(p, target, nxt) && ccw(p, target, cur) > 0) l = 1;
		if (on_seg_weak(p, target, cur) && ccw(p, target, nxt) < 0) r = 1;
		if (on_seg_weak(p, target, nxt) && ccw(p, target, cur) < 0) r = 1;
		if (l && r) return 1;
	}
	return 0;
}
bool visible(Pos H[], const int& sz, const Pos& p,  const Pos& inx, const Pos& target) {
	return inner_check(H, sz, inx)
		&& !blocked(H, sz, p, inx) 
		&& inner_check(H, sz, (p + inx) * .5) 
		&& !invisible(H, sz, inx, target);
}
void init() {//O(N^4)
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> gallery[i].x >> gallery[i].y, gallery[i].i = i + 2;
	std::cin >> guard.x >> guard.y >> sculpture.x >> sculpture.y;
	sculpture.i = 0;
	guard.i = 1;

	if (!invisible(gallery, N, guard, sculpture)) { G[1].push_back({ 0, 0 }); return; }

	t = 0;
	nodes[t++] = sculpture;
	nodes[t++] = guard;
	Pos seg;
	for (int i = 0; i < N; i++) nodes[t++] = gallery[i];
	for (int i = 1; i < t; i++) {
		if (!invisible(gallery, N, nodes[i], nodes[0])) G[i].push_back({ 0, 0 });
		for (int j = i + 1; j < t; j++) {
			if (!blocked(gallery, N, nodes[i], nodes[j])
				&& inner_check(gallery, N, (nodes[i] + nodes[j]) * .5)) {
				seg = nodes[i] - nodes[j];
				G[i].push_back({ j, seg.mag() });
				G[j].push_back({ i, seg.mag() });
			}
		}
	}
	Line view_line, last;
	Pos inx;
	for (int i = 0; i < N; i++) {
		view_line = L(sculpture, gallery[i]);
		last = rotate90(view_line, guard);
		inx = intersection(view_line, last);
		if (visible(gallery, N, guard, inx, sculpture))
			G[guard.i].push_back({ 0, (guard - inx).mag() });
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Pos cur = gallery[j], pre = gallery[(j + N - 1) % N], nxt = gallery[(j + 1) % N];
			last = rotate90(view_line, cur);
			inx = intersection(view_line, last);
			if (visible(gallery, N, cur, inx, sculpture))
				G[gallery[j].i].push_back({ 0, (cur - inx).mag() });
			last = L(cur, pre);
			inx = intersection(view_line, last);
			if (on_seg_weak(cur, pre, inx) && !invisible(gallery, N, inx, sculpture)) {
				G[gallery[j].i].push_back({ 0, (cur - inx).mag() });
				for (int k = 1; k < t; k++) 
					if (!blocked(gallery, N, nodes[k], inx) && inner_check(gallery, N, (nodes[k] + inx) * .5))
						G[nodes[k].i].push_back({ 0, (nodes[k] - inx).mag() });
			}
			last = L(cur, nxt);
			inx = intersection(view_line, last);
			if (on_seg_weak(cur, nxt, inx) && !invisible(gallery, N, inx, sculpture)) {
				G[gallery[j].i].push_back({ 0, (cur - inx).mag() });
				for (int k = 1; k < t; k++)
					if (!blocked(gallery, N, nodes[k], inx) && inner_check(gallery, N, (nodes[k] + inx) * .5))
						G[nodes[k].i].push_back({ 0, (nodes[k] - inx).mag() });
			}
		}
	}
	return;
}
void solve() { init(); std::cout << dijkstra(1, 0) << "\n"; return; }
int main() { solve(); return 0; }//boj26133


/*

16
13 33
20 60
23 66
39 97
49 105
73 166
100 205
117 272
98 216
80 180
66 172
68 156
51 122
41 121
22 92
2 44
10 40
104 228

140.8722826


8
10 10
40 25
20 25
20 35
12 23
30 23
10 20
5 40
15 15
19 26

25.0000000

*/

//ld cross(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) { return (d2 - d1) / (d4 - d3); }
//int ccw(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	ld ret = cross(d1, d2, d3, d4);
//	return z(ret) ? 0 : ret > 0 ? 1 : -1;
//}
//bool CW(const Line& l1, const Line& l2, const Line& target) {
//	if (l1.s / l2.s < TOL) return 0;
//	Pos p = intersection(l1, l2);
//	return target.s.vy * p.x + target.s.vx * p.y > target.c - TOL;
//}
//bool half_plane_intersection(std::vector<Line>& HP, std::vector<Pos>& hull) {
//	std::deque<Line> dq;
//	std::sort(HP.begin(), HP.end());
//	for (const Line& l : HP) {
//		if (!dq.empty() && z(dq.back() / l)) continue;
//		while (dq.size() >= 2 && CW(dq[dq.size() - 2], dq.back(), l)) dq.pop_back();
//		while (dq.size() >= 2 && CW(l, dq.front(), dq[1])) dq.pop_front();
//		dq.push_back(l);
//	}
//	while (dq.size() >= 3 && CW(dq[dq.size() - 2], dq.back(), dq.front())) dq.pop_back();
//	while (dq.size() >= 3 && CW(dq.back(), dq.front(), dq[1])) dq.pop_front();
//	for (int i = 0; i < dq.size(); i++) {
//		Line cur = dq[i], nxt = dq[(i + 1) % dq.size()];
//		if (cur / nxt < TOL) {
//			hull.clear();
//			return 0;
//		}
//		hull.push_back(intersection(cur, nxt));
//	}
//	return 1;
//}
//bool inner_check(std::vector<Pos>& H, const Pos& p) {
//	int cnt = 0;
//	int sz = H.size();
//	for (int i = 0; i < sz; i++) {
//		Pos cur = H[i], nxt = H[(i + 1) % sz];
//		if (on_seg_strong(cur, nxt, p)) return 1;
//		if (z(cur.y - nxt.y)) continue;
//		if (nxt.y < cur.y) std::swap(cur, nxt);
//		if (nxt.y - TOL < p.y || cur.y > p.y) continue;
//		cnt += ccw(cur, nxt, p) > 0;
//	}
//	return cnt & 1;
//}