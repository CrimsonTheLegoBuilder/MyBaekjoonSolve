#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>
#include <queue>
typedef long long ll;
const ll INF = 1e18;
const int LEN = 1001;
int N, M, K, S, E, P;
int G[LEN][LEN], C[LEN][LEN];
int CNT[LEN];

struct Info {
	int u, v; //cur, prev
	int c;    //cost
	int cnt;  //count
	Info(int u, int v, int c, int cnt) : u(u), v(v), c(c), cnt(cnt) {}
	bool operator < (const Info& x) const { return c == x.c ? cnt > x.cnt : c > x.c; }
};
std::priority_queue<Info> PQ;
void dijkstra(int s = S, int e = E, int n = N) {
	while (PQ.size()) PQ.pop();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			C[i][j] = INF;
	for (int i = 1; i <= n; i++) {
		if (~G[s][i]) C[s][i] = G[s][i], PQ.push(Info(i, s, G[s][i], 1));
	}
	while (PQ.size()) {
		Info p = PQ.top(); PQ.pop();
		if (C[p.v][p.u] < p.c) continue;
		//if (p.u == e) CNT[p.v] = p.cnt;
		if (p.u == e) {
			CNT[p.v] = p.cnt;
			continue;
		}
		for (int i = 1; i <= n; i++) {
			if (~G[p.u][i]) continue;
			int cost = p.c + G[p.u][i];
			if (C[p.u][i] > cost) {
				C[p.u][i] = cost;
				PQ.push(Info(i, p.u, cost, p.cnt + 1));
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(G, -1, sizeof G);
	memset(CNT, -1, sizeof CNT);
	std::cin >> N >> M >> K >> S >> E;
	int a, b;
	int w;
	while (M--) {
		std::cin >> a >> b >> w;
		G[a][b] = w;
		G[b][a] = w;
	}
	dijkstra();
	while (K--) {
		std::cin >> P;
		int MIN = INF;
		for (int i = 1; i <= N; i++) {
			if (!~CNT[i]) { 
				C[i][E] += P * CNT[i];
				MIN = std::min(MIN, C[i][E]);
			}
		}
		std::cout << MIN << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj13907