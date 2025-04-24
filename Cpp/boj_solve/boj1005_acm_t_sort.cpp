#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <queue>
typedef long long ll;
const int LEN = 5'001;
std::vector<int> G[LEN];
int T, N, M, W, incoming[LEN];
ll depth[LEN], D[LEN];

ll Tsort(int W) {
	std::queue<int> Q;
	for (int u = 1; u <= N; u++) {
		if (!incoming[u]) {
			Q.push(u);
			depth[u] = D[u];
		}
	}
	while (Q.size()) {
		int u = Q.front(); Q.pop();
		//std::cout << u << "\n";
		for (const int& v : G[u]) {
			depth[v] = std::max(depth[v], depth[u] + D[v]);
			if (!--incoming[v]) {
				Q.push(v);
			}
		}
	}
	return depth[W];
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		memset(D, 0, sizeof D);
		memset(depth, 0, sizeof depth);
		memset(incoming, 0, sizeof incoming);
		for (int i = 1; i <= N; i++) { G[i].clear(); }

		std::cin >> N >> M;
		for (int n = 1; n <= N; n++) { std::cin >> D[n]; }
		for (int m = 0, u, v; m < M; m++) {
			std::cin >> u >> v;
			G[u].push_back(v);
			incoming[v]++;
		}
		std::cin >> W;
		std::cout << Tsort(W) << "\n";
	}
	return 0;
}