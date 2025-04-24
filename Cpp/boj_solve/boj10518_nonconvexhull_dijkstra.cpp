#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
typedef long long ll;
//typedef long double ld;
typedef double ld;
const ld TOL = 1e-8;
const ld INF = 1e17;
const int LEN = 50;
int N, M, t;
ld G[LEN << 1][LEN << 1];

struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> Q;
ld COST[LEN << 1];
ld dijkstra(int v, int g) {
	for (int i = 0; i < LEN << 1; i++) COST[i] = INF;
	Q.push({ v, 0 });
	COST[v] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > COST[p.i]) continue;
		for (int j = 0; j < t; j++) {
			ld w = G[p.i][j];
			if (w > 1e16) continue;
			ld cost = p.c + w;
			if (COST[j] > cost) {
				COST[j] = cost;
				Q.push({ j, cost });
			}
		}
	}
	return COST[g];
}
//void floyd_warshall() {
//	//for (int k = 0; k < t; k++) {
//	//	for (int i = 0; i < t; i++) {
//	//		for (int j = 0; j < t; j++) {
//	//			G[i][j] = std::min(G[i][k] + G[k][j], G[i][j]);
//	//		}
//	//	}
//	//}
//	for (int k = 0; k < t; k++) {
//		for (int i = 0; i < t; i++) {
//			for (int j = i + 1; j < t; j++) {
//				if (k != i && k != j) {
//					G[i][j] = std::min(G[i][k] + G[k][j], G[i][j]);
//					G[j][i] = std::min(G[j][k] + G[k][i], G[j][i]);
//				}
//			}
//		}
//	}
//	return;
//}
bool z(const ld& x) { return std::abs(x) < TOL; }
struct Pos {
	ld x, y;
	int i;
	Pos(ld X = 0, ld Y = 0, int I = 0) : x(X), y(Y), i(I) {}
	bool operator == (const Pos& p) const { return z(x - p.x) && z(y - p.y); }
	bool operator < (const Pos& p) const { return z(x - p.x) ? y < p.y : x < p.x; }
	Pos operator + (const Pos& p) const { return { x + p.x, y + p.y, i }; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y, i }; }
	Pos operator * (const ld& n) const { return { x * n, y * n, i }; }
	Pos operator / (const ld& n) const { return { x / n, y / n, i }; }
	ld operator * (const Pos& p) const { return { x * p.x + y * p.y }; }
	ld operator / (const Pos& p) const { return { x * p.y - y * p.x }; }
	Pos operator ~ () const { return { -y, x, i }; }
	ld operator ! () const { return x * y; }
	Pos& operator += (const Pos& p) { x += p.x; y += p.y; return *this; }
	Pos& operator *= (const ld& scale) { x *= scale; y *= scale; return *this; }
	ld mag() const { return hypot(x, y); }
} inner[LEN], outer[LEN], nodes[LEN << 1];
std::vector<Pos> C, H;
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
	int cnt = 0;
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
void monotone_chain(std::vector<Pos>& C, std::vector<Pos>& H) {
	std::sort(C.begin(), C.end());
	if (C.size() <= 2) {
		for (const Pos& p : C) H.push_back(p);
		return;
	}
	for (int i = 0; i < C.size(); i++) {
		while (H.size() > 1 && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	int s = H.size() + 1;
	for (int i = C.size() - 1; i >= 0; i--) {
		while (H.size() > s && (cross(H[H.size() - 2], H[H.size() - 1], C[i]) < 0 || z(cross(H[H.size() - 2], H[H.size() - 1], C[i])))) {
			H.pop_back();
		}
		H.push_back(C[i]);
	}
	H.pop_back();
	return;
}
bool connectable(const int& i, const int& j) {
	return !blocked(outer, M, nodes[i], nodes[j])
		&& !blocked(inner, N, nodes[i], nodes[j])
		&& inner_check(outer, M, (nodes[i] + nodes[j]) * .5)
		&& !inner_check(inner, N, (nodes[i] + nodes[j]) * .5);
}
void init() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	t = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> inner[i].x >> inner[i].y;
		inner[i].i = t;
		nodes[t] = inner[i];
		t++;
	}
	std::cin >> M;
	for (int j = 0; j < M; j++) {
		std::cin >> outer[j].x >> outer[j].y;
		outer[j].i = t;
		nodes[t] = outer[j];
		t++;
	}
	C.resize(N);
	for (int i = 0; i < N; i++) C[i] = inner[i];
	monotone_chain(C, H);
	return;
}
void solve() {
	for (int i = 0; i < t; i++) for (int j = 0; j < t; j++) G[i][j] = INF;
	for (int i = 0; i < N; i++) {
		G[i][(i + 1) % N] = (inner[i] - inner[(i + 1) % N]).mag();
		for (int k = 0; k < t; k++) {
			if ((ccw(inner[i], inner[(i + 1) % N], nodes[k]) > 0
				|| on_seg_weak(inner[i], nodes[k], inner[(i + 1) % N]))
				&& connectable((i + 1) % N, k)) {
				G[(i + 1) % N][k] = (inner[(i + 1) % N] - nodes[k]).mag();
			}
		}
	}
	for (int j = 0; j < M; j++) {
		G[j + N][((j + 1) % M) + N] = (outer[j] - outer[(j + 1) % M]).mag();
		for (int k = 0; k < t; k++) {
			if ((ccw(outer[j], outer[(j + 1) % M], nodes[k]) < 0
				|| on_seg_weak(outer[j], nodes[k], outer[(j + 1) % M]))
				&& connectable(((j + 1) % M) + N, k)) {
				G[((j + 1) % M) + N][k] = (outer[(j + 1) % M] - nodes[k]).mag();
			}
		}
	}
	ld length = 0;
	int sz = H.size();

	int s = H[0].i, e = H[1 % sz].i;
	ld a = dijkstra(s, e), b = dijkstra(e, s);
	length = a + b;

	//floyd_warshall();
	//for (int i = 0; i < sz; i++) {
	//	Pos cur = H[i], nxt = H[(i + 1) % sz];
	//	if (!blocked(outer, M, cur, nxt)) length += (cur - nxt).mag();
	//	else length += G[cur.i][nxt.i];
	//}

	std::cout << length << "\n";
	return;
}
int main() { init(); solve(); return 0; }//boj10518

/*

8
1 1
15 1
15 7
14 7
14 2
2 2
2 7
1 7
15
0 0
16 0
16 8
13 8
12 4
11 6
10 3
9 6
8 5
7 6
6 3
5 6
4 4
3 8
0 8
43.6832385

7
1 1
5 1
5 19
4 2
3 18
2 2
1 19
9
0 0
6 0
6 20
5 20
4 3
3 19
2 3
1 20
0 20
102.129031841

4
-1000 -1000
0 0
1000 -1000
0 2000

8
1500 -1500
0 2500
-1500 -1500
-460 -730
-20 -510
0 -500
20 -510
460 -730
8560.6232978

*/
