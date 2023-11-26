#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
//typedef long double ld;
typedef double ld;
const int LEN = 100;
const ld MAX = 1e17;
const ld TOL = 1e-8;
int N, M, K;
ld C[LEN];

bool z(ld x) { return std::fabs(x) < TOL; }
struct Pos { ld x, y; }; std::vector<Pos> POS;
struct Seg { Pos l, r; } wall[LEN];
ld dist(const Pos& d1, const Pos& d2) {
	return hypot((d1.x - d2.x), (d1.y - d2.y));
}
int CCW(const Pos& d1, const Pos& d2, const Pos& d3) {
	ld ccw = (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
	return z(ccw) ? 0 : ccw > 0 ? 1 : -1;
}
bool intersect(const Pos& d1, const Pos& d2, const Seg& s) {
	bool F1 = CCW(d1, d2, s.l) * CCW(d2, d1, s.r) > 0 && CCW(s.l, s.r, d1) * CCW(s.r, s.l, d2) > 0;
	return F1;
}
bool OK(const Pos& d1, const Pos& d2) {
	for (int j = 0; j < M; j++) if (intersect(d1, d2, wall[j])) return 0;
	return 1;
}
bool pos_init() {
	std::cin >> N;
	if (!~N) return 0;
	POS.clear();
	POS.push_back({ 0, 5 });
	POS.push_back({ 10, 5 });
	//POS = { { 0, 5 }, { 10, 5 } };
	M = -1; K = 2;
	ld x, a, b, c, d;
	for (int i = 0; i < N; i++) {
		std::cin >> x >> a >> b >> c >> d;
		POS.push_back({ x, a });
		POS.push_back({ x, b });
		POS.push_back({ x, c });
		POS.push_back({ x, d });
		wall[++M] = { { x, 0 }, { x, a } };
		wall[++M] = { { x, b }, { x, c } };
		wall[++M] = { { x, d }, { x, 10 } };
		K += 4;
	}
	return 1;
}
struct Info {
	int i;
	ld c;
	bool operator < (const Info& w) const { return c > w.c; }
};
std::vector<Info> G[LEN];
std::priority_queue<Info> Q;
ld dijkstra(int v) {
	for (int j = 0; j < K; j++) C[j] = MAX;
	C[v] = 0;
	Q.push({ v, 0 });
	while (!Q.empty()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		//for (const Info& w : G[p.i]) {
		for (int i = 0; i < G[p.i].size(); i++) {
			Info w = G[p.i][i];
			ld cost = p.c + w.c;
			if (C[w.i] > cost) {
				C[w.i] = cost;
				Q.push({ w.i, cost });
			}
		}
	}
	return C[1];
}
void graph_init() {
	for (int i = 0; i < K; i++) G[i].clear();
	for (int i = 0; i < K; i++) {
		for (int j = i + 1; j < K; j++) {
			if (OK(POS[i], POS[j])) {
				ld c = dist(POS[i], POS[j]);
				G[i].push_back({ j, c });
				G[j].push_back({ i, c });
			}
		}
	}
	return;
}



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	while (pos_init()) {
		graph_init();
		std::cout << dijkstra(0) << "\n";
	}
	return 0;
}

////bool intersect(const Seg& s1, const Seg& s2) {
////	bool F1 = CCW(s1.l, s1.r, s2.l) * CCW(s1.r, s1.l, s2.r) > 0 && CCW(s2.l, s2.r, s1.l) * CCW(s2.r, s2.l, s1.r) > 0;
////	//bool F2 = (
////	//	(!CCW(s1.l, s1.r, s2.l) && between(s1.l, s2.l, s1.r) >= 0) ||
////	//	(!CCW(s1.l, s1.r, s2.r) && between(s1.l, s2.r, s1.r) >= 0) ||
////	//	(!CCW(s2.l, s2.r, s1.l) && between(s2.l, s1.l, s2.r) >= 0) ||
////	//	(!CCW(s2.l, s2.r, s1.r) && between(s2.l, s1.r, s2.r) >= 0)
////	//	);
////	return F1;// || F2;
////}