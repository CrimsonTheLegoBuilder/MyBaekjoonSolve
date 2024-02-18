#include <iostream>
#include <algorithm>
//#include <unordered_map>
#include <vector>
#include <queue>
#include <cstring>
typedef long long ll;
const int LEN = 101;
const int INF = 1e9;
int cost[LEN][LEN];
//std::unordered_map<int, int> dp[LEN];  //dp[u][p] : from p, now u
ll DP[LEN][LEN];
int N, M, u, v;
std::vector<int> G[LEN];

struct Pos { int x, y; }pos[LEN];
struct Toll {
	int u, p;
	ll c;
	bool operator < (const Toll& t) const { return c > t.c; }
};
std::priority_queue<Toll> H;

int dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
	return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y);
}
bool obtuse(const Pos& prev, const Pos& cur, const Pos& nxt) {
	return (dot(prev, cur, cur, nxt) >= 0);
}
ll dijkstra() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			DP[i][j] = INF;
		}
	}
	//ll ret = 1e9;
	for (const int& v : G[1]) {
		DP[v][1] = cost[1][v];
		H.push({ v, 1, DP[v][1] });
	}
	while (!H.empty()) {
		Toll i = H.top(); H.pop();
		if (i.c > DP[i.u][i.p]) continue;
		if (i.u == 2) return i.c;
		for (const int& v : G[i.u]) {
			if (obtuse(pos[i.p], pos[i.u], pos[v])) {
				if (i.c + cost[i.u][v] < DP[v][i.u]) {
					DP[v][i.u] = i.c + cost[i.u][v];
					H.push({ v, i.u, DP[v][i.u] });
				}
			}
		}
	}
	//if (ret == INF) return -1;
	return -1;
}


int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	ll c;
	std::cin >> N >> M;
	for (int i = 1; i <= N; i++) { std::cin >> pos[i].x >> pos[i].y; }
	for (int i = 0; i < M; i++) {
		std::cin >> u >> v >> c;
		cost[u][v] = cost[v][u] = c;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	std::cout << dijkstra();
	return 0;
}


//#include <iostream>
//#include <algorithm>
//#include <unordered_map>
//#include <vector>
//#include <queue>
//typedef long long ll;
//const int LEN = 101;
//int cost[LEN][LEN];
//std::unordered_map<int, int> dp[LEN];  //dp[u][p] : from p, now u
////ll DP[LEN][LEN];
//std::vector<int> G[LEN];
//
//struct Pos {
//	ll x, y;
//	bool operator < (const Pos& p) const {
//		if (x == p.x) { return y < p.y; }
//		return x < p.x;
//	}
//}pos[LEN];
//
//struct Toll {
//	int u, p;
//	ll c;
//	bool operator < (const Toll& t) const { return u > t.u; }
//};
//std::priority_queue<Toll> H;
//
//int dot(const Pos& d1, const Pos& d2, const Pos& d3, const Pos& d4) {
//	return (d2.x - d1.x) * (d4.x - d3.x) + (d2.y - d1.y) * (d4.y - d3.y);
//}
//bool obtuse(const Pos& prev, const Pos& cur, const Pos& nxt) {
//	return (dot(prev, cur, cur, nxt) >= 0);
//}
//ll dijkstra() {
//	ll ret = 1e9;
//	for (const int& v : G[1]) {
//		dp[v][1] = cost[1][v];
//		H.push({ v, 1, dp[v][1] });
//	}
//	while (!H.empty()) {
//		Toll i = H.top(); H.pop();
//		if (i.c > dp[i.u][i.p]) continue;
//		if (i.u == 2) { ret = std::min(ret, i.c); continue; }
//		for (const int& v : G[i.u]) {
//			if (obtuse(pos[i.p], pos[i.u], pos[v])) {
//				if (dp[v].find(i.u) == dp[v].end() || i.c + cost[i.u][v] < dp[v][i.u]) {
//					dp[v][i.u] = i.c + cost[i.u][v];
//					H.push({ v, i.u, dp[v][i.u] });
//				}
//			}
//		}
//	}
//	if (dp[2].empty()) return -1;
//	return ret;
//}
//
//
//int main() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	int N, M, u, v;
//	ll c;
//	std::cin >> N >> M;
//	for (int i = 1; i <= N; i++) { std::cin >> pos[i].x >> pos[i].y; }
//	for (int i = 0; i < M; i++) {
//		std::cin >> u >> v >> c;
//		cost[u][v] = cost[v][u] = c;
//		G[u].push_back(v);
//		G[v].push_back(u);
//	}
//	std::cout << dijkstra();
//	return 0;
//}