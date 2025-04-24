#include <iostream>
#include <algorithm>
#include <xstring>
#include <vector>
#include <queue>
const int LEN = 5'001;
std::vector<int> G[LEN];
int N, M, incoming[LEN], depth[LEN];

bool Tsort() {
	std::queue<int> Q;
	int ret = 0;
	for (int u = 1; u <= N; u++) {
		if (!incoming[u]) {
			Q.push(u);
			depth[u] = 1;
		}
	}
	while (Q.size()) {
		int u = Q.front(); Q.pop();
		std::cout << u << "\n";
		for (const int& v : G[u]) {
			depth[v] = std::max(depth[v], depth[u] + 1);
			ret = std::max(ret, depth[v]);
			if (!--incoming[v]) {
				Q.push(v);
			}
		}
	}
	return ret < N;
}

int main() {
	std::cin >> N >> M;
	for (int m = 0, u, v; m < M; m++) {
		std::cin >> u >> v;
		G[u].push_back(v);
		incoming[v]++;
	}
	//std::cout << Tsort() << "\n";
}