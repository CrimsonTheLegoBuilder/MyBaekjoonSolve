#include <iostream>
#include <queue>
#include <set>
const int LEN = 1001;
std::set<int> graph[LEN];

bool VD[LEN]{};
bool VB[LEN]{};
void DFS(int v) {
	VD[v] = 1;
	std::cout << v << " ";
	for (const int& w : graph[v]) {
		if (!VD[w]) DFS(w);
	}
	return;
}
void BFS(int v) {
	std::queue<int> Q;
	int p;
	Q.push(v);
	VB[v] = 1;
	while (Q.size()) {
		p = Q.front(); Q.pop();
		std::cout << p << " ";
		for (const int& w : graph[p]) {
			if (!VB[w]) {
				VB[w] = 1; Q.push(w);
			}
		}
	}
	return;
}
void make_graph(int M) {
	int s, e;
	for (int i = 0; i < M; i++) {
		std::cin >> s >> e;
		graph[s].insert(e);
		graph[e].insert(s);
	}
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int N, M, K;
	std::cin >> N >> M >> K;
	make_graph(M);
	DFS(K);
	std::cout << "\n";
	BFS(K);
	return 0;
}