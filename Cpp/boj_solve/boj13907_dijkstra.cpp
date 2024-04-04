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
int G[LEN][LEN], C[LEN][LEN];

struct Info {
	int i;
	int c;
	int cnt;
	Info(int i, int c, int cnt) : i(i), c(c), cnt(cnt) {}
	bool operator < (const Info& x) const { return c > x.c; }
};
std::priority_queue<Info> PQ;
void dijkstra(int s = S, int e = E, int n = N) {
	while (PQ.size()) PQ.pop();
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			C[i][j] = INF;
	C[0][s] = 0;
	PQ.push(Info(s, 0, 1));
	while (PQ.size()) {
		Info p = PQ.top(); PQ.pop();
		if (C[p.cnt][p.i] < p.c) continue;
		for (int i = 1; i <= n; i++) {
			if (~G[p.i][i]) continue;
			int cost = p.c + G[p.i][i];
			if (C[p.cnt][i] > cost) {
				C[p.cnt][i] = cost;
				PQ.push(Info(i, cost, p.cnt + 1));
			}
		}
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	memset(G, -1, sizeof G);
	std::cin >> N >> M >> K >> S >> E;
	int a, b;
	int w;
	while (M--) {
		std::cin >> a >> b >> w;
		if (~G[a][b]) {
			G[a][b] = std::min(G[a][b], w);
			G[b][a] = std::min(G[b][a], w);
		}
		else {
			G[a][b] = w;
			G[b][a] = w;
		}
	}
	dijkstra();
	while (K--) {
		std::cin >> P;
		int MIN = INF;
		for (int i = 1; i <= N; i++) {
			if (C[i][E] < INF) {
				C[i][E] += P * i;
				MIN = std::min(MIN, C[i][E]);
			}
		}
		std::cout << MIN << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj13907