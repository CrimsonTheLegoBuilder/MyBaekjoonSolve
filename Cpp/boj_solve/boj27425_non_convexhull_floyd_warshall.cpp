#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>;
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-8;
const ld INF = 1e17;
const int LEN = 100;
int N, M, t, order[LEN];
ld G[LEN << 1][LEN << 1];
bool V[LEN];

struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const { return c > x.c; }
};
bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	Pos(ld X = 0, ld Y = 0) : x(X), y(Y) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	Pos operator * (const ld& n) const { return { x * n, y * n }; }
	Pos operator / (const ld& n) const { return { x / n, y / n }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} island[LEN], port[LEN], nodes[LEN << 1];
ld cross(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) / (d3 - d2); }
ld dot(const Pos& d1, const Pos& d2, const Pos& d3) { return (d2 - d1) * (d3 - d2); }
int ccw(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = cross(d1, d2, d3);
	return z(ret) ? 0 : ret > 0 ? 1 : -1;
}
bool on_seg_strong(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && (ret > 0 || z(ret));
}
bool on_seg_weak(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ret = dot(d1, d3, d2);
	return !ccw(d1, d2, d3) && ret > 0;
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
	//bool f3 = on_seg_strong(s1, s2, p1) ||
	//	on_seg_strong(s1, s2, p2) ||
	//	on_seg_strong(p1, p2, s1) ||
	//	on_seg_strong(p1, p2, s2);
	return (f1 && f2);// || f3;
}
bool blocked(const Pos& s1, const Pos& s2, const Pos& d1, const Pos& d2) {
	bool f0 = intersect(s1, s2, d1, d2);
	bool f4 = on_seg_weak(s1, s2, d1) || on_seg_weak(s1, s2, d2);
	return f0 || f4;
}
bool blocked(Pos H[], const int& sz, const Pos& s1, const Pos& s2) {
	for (int i = 0; i < sz; i++) if (blocked(s1, s2, H[i], H[(i + 1) % sz])) return 1;
	return 0;
}
void port_sort(Pos H[], const int& sz_h, Pos P[], const int& sz_p, bool V[], int order[]) {
	memset(V, 0, sizeof V);
	memset(order, -1, sizeof order);
	std::vector<Info> tmp;
	int o = 0;
	for (int i = 0; i < sz_h; i++) {
		tmp.clear();
		Pos cur = H[i], nxt = H[(i + 1) % sz_h];
		for (int j = 0; j < sz_p; j++) {
			if (!V[j] && (cur == P[j] || on_seg_weak(cur, nxt, P[j]))) {
				tmp.push_back({ j, (nxt - P[j]).mag() });
				V[j] = 1;
			}
		}
		std::sort(tmp.begin(), tmp.end());
		for (const Info& I : tmp) order[o++] = I.i;
	}
	return;
}
void floyd_warshall() {
	for (int k = 0; k < t; k++) {
		for (int i = 0; i < t; i++) {
			for (int j = 0; j < t; j++) {
				G[i][j] = std::min(G[i][k] + G[k][j], G[i][j]);
			}
		}
	}
	//for (int k = 0; k < t; k++) {
	//	for (int i = 0; i < t; i++) {
	//		for (int j = i + 1; j < t; j++) {
	//			if (k != i && k != j) {
	//				G[i][j] = std::min(G[i][k] + G[k][j], G[i][j]);
	//				G[j][i] = std::min(G[j][k] + G[k][i], G[j][i]);
	//			}
	//		}
	//	}
	//}
	return;
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N >> M;
	t = 0;
	for (int i = 0; i < N; i++) std::cin >> island[i].x >> island[i].y, nodes[t++] = island[i];
	for (int j = 0; j < M; j++) std::cin >> port[j].x >> port[j].y, nodes[t++] = port[j];
	port_sort(island, N, port, M, V, order);
	return;
}
void solve() {
	Pos seg;
	for (int i = 0; i < t; i++) for (int j = 0; j < t; j++) G[i][j] = INF;
	for (int i = 0; i < t; i++) {
		G[i][i] = 0l;
		for (int j = i + 1; j < t; j++) {
			if (!blocked(island, N, nodes[i], nodes[j])
				&& inner_check(island, N, (nodes[i] + nodes[j]) * .5)) {
				seg = nodes[i] - nodes[j];
				G[i][j] = seg.mag();
				G[j][i] = seg.mag();
			}
		}
	}
	floyd_warshall();
	ld length = 0;
	for (int j = 0; j < M; j++) length += G[order[j] + N][order[(j + 1) % M] + N];
	std::cout << length << "\n";
	return;
}
int main() { init(); solve(); return 0; }//boj27425
