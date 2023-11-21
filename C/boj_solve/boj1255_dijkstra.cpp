#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
//#include <cstring>
#include <string>
//typedef long double ld;
typedef double ld;
const int LEN = 55;
const ld MAX = 1e17;
int N, M;
ld C[LEN << 1];

struct Pos { int x, y, v; } people[LEN], corner[LEN << 1];
struct Seg { Pos l, r; } wall[LEN];
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
int CCW(const Pos& d1, const Pos& d2, const Pos& d3) {
	int ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return ccw > 0 ? 1 : ccw < 0 ? -1 : 0;
	//return ccw;
}
int between(const Pos& d1, const Pos& d2, const Pos& d3) {
	int dot = (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
	return dot > 0 ? 1 : dot < 0 ? -1 : 0;
	//return dot;
}
bool intersect(const Pos& d1, const Pos& d2, const Seg& s) {
	bool F1 = CCW(d1, d2, s.l) * CCW(d2, d1, s.r) > 0 && CCW(s.l, s.r, d1) * CCW(s.r, s.l, d2) > 0;
	//bool F2 = (
	//	(!CCW(d1, d2, s.l) && between(d1, s.l, d2) >= 0) ||
	//	(!CCW(d1, d2, s.r) && between(d1, s.r, d2) >= 0) ||
	//	(!CCW(s.l, s.r, d1) && between(s.l, d1, s.r) >= 0) ||
	//	(!CCW(s.l, s.r, d2) && between(s.l, d2, s.r) >= 0)
	//	);
	return F1;// || F2;
}
bool pos_init() {
	char a;
	scanf("%d%d", &N, &M);
	//printf("%d %d\n", N ,M);
	for (int i = 0; i < N; i++) {
		scanf(" (%d,%d)", &people[i].x, &people[i].y);
		scanf(" %d", &people[i].v);
		//printf("%d %d %d %d\n", people[i].x, people[i].y, people[i].v, i);
	}
	if (!M) return 0;
	int t = -1;
	for (int i = 0; i < M; i++) {
		scanf(" (%d,%d)", &wall[i].l.x, &wall[i].l.y);
		scanf(" %s", &a);
		scanf(" (%d,%d)", &wall[i].r.x, &wall[i].r.y);
		//printf("%d %d %d %d\n", wall[i].l.x, wall[i].l.y, wall[i].r.x, wall[i].r.y);
		corner[++t] = wall[i].l;
		corner[++t] = wall[i].r;
	}
	return 1;
}

struct Info {
	int i;
	ld c;
	bool operator < (const Info& w) const { return c > w.c; }
};
std::vector<Info> G[LEN << 1];
std::priority_queue<Info> Q;
ld dijkstra(int v, int i) {
	for (int j = 0; j < M + 2; j++) C[j] = MAX;
	C[v] = 0;
	Q.push({ v, 0 });
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Q.push({ w.i, cost });
			}
		}
	}
	//std::cout << C[1] << " cost\n";
	//std::cout << C[1] / people[i].v << " time\n";
	return C[1] / people[i].v;
}
void graph_init() {
	bool f = 1;
	for (int i = 0; i < M * 2; i++) {
		Pos cur = corner[i];
		Pos goal = { cur.x, 0, 0 };
		for (int j = 0; j < M; j++) {
			if (intersect(cur, goal, wall[j])) f = 0;
		}
		if (f) G[i + 2].push_back({ 1, dist(cur, goal) });
		for (int j = i + 1; j < M * 2; j++) {
			Pos nxt = corner[j];
			f = 1;
			for (int k = 0; k < M; k++) {
				if (intersect(cur, nxt, wall[k])) f = 0;
			}
			if (f) {
				ld d = dist(cur, nxt);
				G[i + 2].push_back({ j + 2, d });
				G[j + 2].push_back({ i + 2, d });
			}
		}
	}
}
void compose_graph_with_(const Pos& p) {
	G[0].clear();
	bool f = 1;
	Pos goal = { p.x, 0, 0 };
	for (int i = 0; i < M; i++) {
		if (intersect(p, goal, wall[i])) f = 0;
	}
	if (f) G[0].push_back({ 1, dist(p, goal) });
	for (int i = 0; i < M * 2; i++) {
		Pos w = corner[i];
		f = 1;
		for (int k = 0; k < M; k++) {
			if (intersect(p, w, wall[k])) f = 0;
		}
		if (f) G[0].push_back({ i + 2, dist(p, w) });
		//printf("%d %d %d %d %d %.6f\n", p.x, p.y, w.x, w.y, f, dist(p, w));
	}
}



int main() {
	if (!pos_init()) {
		ld cost = -1;
		for (int i = 0; i < N; i++) cost = std::max(cost, ((ld)people[i].y / people[i].v));
		printf("%.1lf\n", cost);
		return 0;
	}
	if (!N) return 0;

	graph_init();

	ld cost = -1;
	for (int i = 0; i < N; i++) {
		compose_graph_with_(people[i]);
		cost = std::max(cost, dijkstra(0, i));
	}
	printf("%.1lf\n", cost);
	return 0;
}
//bool intersect(const Seg& s1, const Seg& s2) {
//	bool F1 = CCW(s1.l, s1.r, s2.l) * CCW(s1.r, s1.l, s2.r) > 0 && CCW(s2.l, s2.r, s1.l) * CCW(s2.r, s2.l, s1.r) > 0;
//	//bool F2 = (
//	//	(!CCW(s1.l, s1.r, s2.l) && between(s1.l, s2.l, s1.r) >= 0) ||
//	//	(!CCW(s1.l, s1.r, s2.r) && between(s1.l, s2.r, s1.r) >= 0) ||
//	//	(!CCW(s2.l, s2.r, s1.l) && between(s2.l, s1.l, s2.r) >= 0) ||
//	//	(!CCW(s2.l, s2.r, s1.r) && between(s2.l, s1.r, s2.r) >= 0)
//	//	);
//	return F1;// || F2;
//}