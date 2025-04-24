#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <cmath>
typedef double ld;
const int LEN = 51;
const ld MAX = 1e17;
int T, N, M, S, E;

struct Info {
	int i;
	ld c;
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> Q;
std::vector<Info> G[LEN];
ld C[LEN];
ld dijkstra(int s, int e) {
	for (int i = 1; i <= N; i++) C[i] = MAX;
	Q.push({ s, 0 });
	C[s] = 0;
	while (Q.size()) {
		Info p = Q.top(); Q.pop();
		if (p.c > C[p.i]) continue;
		for (const Info& w : G[p.i]) {
			ld cost = w.c + p.c;
			if (C[w.i] > cost) Q.push({ w.i, C[w.i] = cost });
		}
	}
	return C[e];
}
struct Pos {
	int x, y;
	int i;
	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
	ld mag() { return hypot(x, y); }
};
std::vector<Pos> island[LEN];
int cross(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
}
int dot(const Pos& d1, const Pos& d2, const Pos& d3) {
	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
}
ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
	int f1 = dot(d1, d2, d3), f2 = dot(d2, d1, d3);
	int ret = cross(d1, d2, d3);
	ld D;
	if (f1 <= 0 && f2 <= 0) D = std::abs(ret) / (d1 - d2).mag();
	else D = std::min((d1 - d3).mag(), (d2 - d3).mag());
	return D;
}
ld dist(std::vector<Pos>& HI, std::vector<Pos>& HJ) {
	int szi = HI.size(), szj = HJ.size();
	ld distance = MAX;
	for (int i_ = 0; i_ < szi; i_++) {
		Pos cur = HI[i_], nxt = HI[(i_ + 1) % szi];
		for (int j_ = 0; j_ < szj; j_++) 
			distance = std::min(distance, dist(cur, nxt, HJ[j_]));
	}
	for (int j_ = 0; j_ < szj; j_++) {
		Pos cur = HJ[j_], nxt = HJ[(j_ + 1) % szj];
		for (int i_ = 0; i_ < szi; i_++) 
			distance = std::min(distance, dist(cur, nxt, HI[i_]));
	}
	return distance;
}
int init() {
	for (int i = 1; i < LEN; i++) island[i].clear(), G[i].clear();
	std::cin >> N >> S >> E;
	for (int i = 1; i <= N; i++) {
		std::cin >> M;
		island[i].resize(M);
		for (int j = 0; j < M; j++) std::cin >> island[i][j].x >> island[i][j].y;
	}
	for (int i = 1; i < N; i++) {
		for (int j = i + 1; j <= N; j++) {
			ld cost = dist(island[i], island[j]);
			G[i].push_back({ j, cost });
			G[j].push_back({ i, cost });
		}
	}
	return T--;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	freopen("Island.in", "r", stdin);
	freopen("Island.out", "w", stdout);
	std::cin >> T;
	while (init()) std::cout << dijkstra(S, E) << "\n";
	return;
}
int main() { solve(); return 0; }//boj5379



//#define _CRT_SECURE_NO_WARNINGS
//#include <iostream>
//#include <algorithm>
//#include <queue>
//#include <vector>
//#include <cmath>
//typedef int ll;
////typedef long long ll;
//typedef double ld;
////typedef long double ld;
//const int LEN = 51;
//const ld MAX = 1e17;
//int T, N, M, S, E;
//
//struct Pos {
//	ll x, y;
//	int i;
//	bool operator < (const Pos& p) const { return x == p.x ? y < p.y : x < p.x; }
//	ld mag() { return hypot(x, y); }
//	Pos operator - (const Pos& p) const { return { x - p.x, y - p.y }; }
//};
//std::vector<Pos> island[LEN];
//ll cross(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.y - d2.y) - (d2.y - d1.y) * (d3.x - d2.x);
//}
//ll dot(const Pos& d1, const Pos& d2, const Pos& d3) {
//	return (d2.x - d1.x) * (d3.x - d2.x) + (d2.y - d1.y) * (d3.y - d2.y);
//}
//ld dist(const Pos& d1, const Pos& d2, const Pos& d3) {
//	ll f1 = dot(d1, d2, d3), f2 = dot(d2, d1, d3);
//	ll ret = cross(d1, d2, d3);
//	ld D;
//	if (f1 <= 0 && f2 <= 0) D = std::abs(ret) / (d1 - d2).mag();
//	else D = std::min((d1 - d3).mag(), (d2 - d3).mag());
//	return D;
//}
//struct Info {
//	int i;
//	ld c;
//	bool operator < (const Info& x) const { return c > x.c; }
//};
//std::priority_queue<Info> Q;
//std::vector<Info> G[LEN];
//ld C[LEN];
//ld dijkstra(int s, int e) {
//	for (int i = 1; i <= N; i++) C[i] = MAX;
//	Q.push({ s, 0 });
//	C[s] = 0;
//	while (Q.size()) {
//		Info p = Q.top(); Q.pop();
//		if (p.c > C[p.i]) continue;
//		for (const Info& w : G[p.i]) {
//			ld cost = w.c + p.c;
//			if (C[w.i] > cost) {
//				C[w.i] = cost;
//				Q.push({ w.i, cost });
//			}
//		}
//	}
//	return C[e];
//}
//ld dist(std::vector<Pos>& HI, std::vector<Pos>& HJ) {
//	int szi = HI.size(), szj = HJ.size();
//	ld distance = MAX;
//	for (int i_ = 0; i_ < szi; i_++) {
//		Pos cur = HI[i_], nxt = HI[(i_ + 1) % szi];
//		for (int j_ = 0; j_ < szj; j_++) {
//			Pos des = HJ[j_];
//			distance = std::min(distance, dist(cur, nxt, des));
//		}
//	}
//	for (int j_ = 0; j_ < szj; j_++) {
//		Pos cur = HJ[j_], nxt = HJ[(j_ + 1) % szj];
//		for (int i_ = 0; i_ < szi; i_++) {
//			Pos des = HI[i_];
//			distance = std::min(distance, dist(cur, nxt, des));
//		}
//	}
//	return distance;
//}
//std::vector<Pos> HI, HJ;
//ld dist(int i, int j) {
//	HI = island[i], HJ = island[j];
//	int szi = HI.size(), szj = HJ.size();
//	ld distance = MAX;
//	for (int i_ = 0; i_ < szi; i_++) {
//		Pos cur = HI[i_], nxt = HI[(i_ + 1) % szi];
//		for (int j_ = 0; j_ < szj; j_++) {
//			Pos des = HJ[j_];
//			distance = std::min(distance, dist(cur, nxt, des));
//		}
//	}
//	for (int j_ = 0; j_ < szj; j_++) {
//		Pos cur = HJ[j_], nxt = HJ[(j_ + 1) % szj];
//		for (int i_ = 0; i_ < szi; i_++) {
//			Pos des = HI[i_];
//			distance = std::min(distance, dist(cur, nxt, des));
//		}
//	}
//	return distance;
//}
//void init() {
//	for (int i = 1; i < LEN; i++) island[i].clear(), G[i].clear();
//	std::cin >> N >> S >> E;
//	for (int i = 1; i <= N; i++) {
//		std::cin >> M;
//		island[i].resize(M);
//		for (int j = 0; j < M; j++) std::cin >> island[i][j].x >> island[i][j].y;
//
//	}
//	for (int i = 1; i <= N; i++) {
//		for (int j = i + 1; j <= N; j++) {
//			ld cost = dist(island[i], island[j]);
//			//ld cost = dist(i, j);
//			G[i].push_back({ j, cost });
//			G[j].push_back({ i, cost });
//		}
//	}
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cout << std::fixed;
//	std::cout.precision(3);
//	freopen("Island.in", "r", stdin);
//	freopen("Island.out", "w", stdout);
//	std::cin >> T;
//	while (T--) {
//		init();
//		std::cout << dijkstra(S, E) << "\n";
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj5379
