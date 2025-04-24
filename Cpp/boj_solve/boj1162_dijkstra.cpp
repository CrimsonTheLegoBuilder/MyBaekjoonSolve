#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
typedef long long ll;
const int LEN = 10'001;
const ll MAX = 1e12;
int N, M, K, v1, v2, c;
ll DP[21][LEN];

struct W { int i, cost; };
struct Cur {
	ll cost; 
	int i, pave;
	bool operator < (const Cur& x) const { return cost > x.cost; }
};

std::vector<W> G[LEN];
std::priority_queue<Cur> H;

ll dijkstra(int K, int v = 1) {
	for (int i = 0; i <= K; i++) {
		for (int j = 2; j <= N; j++) {
			DP[i][j] = MAX;
		}
	}
	H.push({ 0, 1, 0 });
	while (!H.empty()) {
		Cur cur = H.top(); H.pop();
		if (DP[cur.pave][cur.i] < cur.cost) continue;

		for (const W& w : G[cur.i]) {
			ll total = cur.cost + w.cost;
			if (DP[cur.pave][w.i] > total) {  //not pave
				DP[cur.pave][w.i] = total;
				H.push({ total, w.i, cur.pave });
			}

			if (cur.pave < K && DP[cur.pave + 1][w.i] > cur.cost) {  //pave
				DP[cur.pave + 1][w.i] = cur.cost;
				H.push({ cur.cost, w.i, cur.pave + 1 });
			}
		}
	}
	ll MIN = MAX;
	for (int i = 0; i <= K; i++) {
		MIN = std::min(MIN, DP[i][N]);
	}
	return MIN;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M >> K;
	for (int i = 0; i < M; i++) {
		std::cin >> v1 >> v2 >> c;
		G[v1].push_back({ v2, c });
		G[v2].push_back({ v1, c });
	}
	std::cout << dijkstra(K) << "\n";
	return 0;
}
