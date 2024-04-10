#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
typedef long long ll;
const ll INF = 1e9;
const int LEN = 1005;
int N, M, K, S, E, P;
int adj[LEN][LEN], C[LEN][LEN];

struct Info {
	int i, c, cnt;
	Info(int i = 0, int c = 0, int cnt = 0) : i(i), c(c), cnt(cnt) {}
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> PQ;
std::vector<Info> G[LEN];
void dijkstra(int s = S, int e = E, int n = N) {
	while (PQ.size()) PQ.pop();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			C[i][j] = INF;
	C[0][s] = 0;
	PQ.push(Info(s, 0, 0));
	while (PQ.size()) {
		Info p = PQ.top(); PQ.pop();
		//if (C[p.cnt][p.i] < p.c) continue;
		if (C[p.cnt][p.i] != p.c) continue;
		if (p.i == e) continue;
		for (const Info& w : G[p.i]) {
			int cost = p.c + w.c;
			if (p.cnt + 1 < N && C[p.cnt + 1][w.i] > cost) {
				C[p.cnt + 1][w.i] = cost;
				PQ.push(Info(w.i, cost, p.cnt + 1));
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(adj, -1, sizeof adj);
	std::cin >> N >> M >> K >> S >> E;
	int a, b, w;
	while (M--) {
		std::cin >> a >> b >> w;
		if (~adj[a][b]) {
			adj[a][b] = std::min(adj[a][b], w);
			adj[b][a] = std::min(adj[b][a], w);
		}
		else {
			adj[a][b] = w;
			adj[b][a] = w;
		}
	}
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			if (~adj[i][j]) G[i].push_back(Info(j, adj[i][j]));
	dijkstra();
	//std::cout << "dijkstra done\n";
	std::vector<Info> tmp;
	for (int i = 1; i <= N; i++)
		if (C[i][E] < INF)
			tmp.push_back(Info(0, C[i][E], i));
	int MIN = INF;
	K++;
	P = 0;
	while (K--) {
		MIN = INF;
		for (Info& x : tmp)
			x.c += x.cnt * P,
			MIN = std::min(MIN, x.c);
		std::cout << MIN << "\n";
		if (K >= 1) std::cin >> P;
	}
	return;
}
int main() { solve(); return 0; }//boj13907

		//for (int i = 1; i <= N; i++)
		//	if (C[i][E] < INF)
		//		C[i][E] += P * i,
		//		MIN = std::min(MIN, C[i][E]);
//#include <iostream>
//#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <vector>
//#include <queue>
//typedef long long ll;
//const ll INF = 1e9;
//const int LEN = 1005;
//int N, M, K, S, E, P;
//int G[LEN][LEN], C[LEN][LEN];
//
//struct Info {
//	int i;
//	int c;
//	int cnt;
//	Info(int i, int c, int cnt) : i(i), c(c), cnt(cnt) {}
//	bool operator < (const Info& x) const { return c > x.c; }
//};
//std::priority_queue<Info> PQ;
//void dijkstra(int s = S, int e = E, int n = N) {
//	while (PQ.size()) PQ.pop();
//	for (int i = 1; i <= n; i++)
//		for (int j = 1; j <= n; j++)
//			C[i][j] = INF;
//	C[0][s] = 0;
//	PQ.push(Info(s, 0, 0));
//	int x = 1;
//	while (PQ.size()) {
//		Info p = PQ.top(); PQ.pop();
//		if (C[p.cnt][p.i] < p.c) continue;
//		if (p.i == e) continue;
//		for (int i = 1; i <= n; i++) {
//			if (!~G[p.i][i]) continue;
//			int cost = p.c + G[p.i][i];
//			if (C[p.cnt + 1][i] > cost) {
//				C[p.cnt + 1][i] = cost;
//				PQ.push(Info(i, cost, p.cnt + 1));
//			}
//		}
//		//std::cout << "DEBUG:: " << x++ << "\n";
//	}
//	return;
//}
//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//	memset(G, -1, sizeof G);
//	std::cin >> N >> M >> K >> S >> E;
//	int a, b;
//	int w;
//	while (M--) {
//		std::cin >> a >> b >> w;
//		if (~G[a][b]) {
//			G[a][b] = std::min(G[a][b], w);
//			G[b][a] = std::min(G[b][a], w);
//		}
//		else {
//			G[a][b] = w;
//			G[b][a] = w;
//		}
//	}
//	dijkstra();
//	//std::cout << "dijkstra done\n";
//	int MIN = INF;
//	for (int i = 1; i <= N; i++) 
//		if (C[i][E] < INF) 
//			MIN = std::min(MIN, C[i][E]);
//	std::cout << MIN << "\n";
//	while (K--) {
//		std::cin >> P;
//		MIN = INF;
//		for (int i = 1; i <= N; i++) 
//			if (C[i][E] < INF) 
//				C[i][E] += P * i,
//				MIN = std::min(MIN, C[i][E]);
//		std::cout << MIN << "\n";
//	}
//	return;
//}
//int main() { solve(); return 0; }//boj13907

/*

3 3 2
1 3
1 3 5
1 2 1
2 3 2
1
2

3
5
8

*/