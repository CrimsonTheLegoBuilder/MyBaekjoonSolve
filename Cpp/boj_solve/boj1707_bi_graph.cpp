#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
typedef long long ll;
const int LEN = 20'001;
int T, N, E, u, v, V[LEN];
std::vector<int> G[LEN];
bool F;

//bool DFS(int v) {
//	bool f = 1;
//	for (const int& w : G[v]) {
//		if (!~V[w]) {
//			V[w] = V[v] ^ 1;
//			f = DFS(w);
//		}
//		else if (V[w] == V[v]) return 0;
//	}
//	return f;
//}
bool DFS2(int v, int p) {
	V[v] = !~p ? 1 : V[p] ^ 1;
	bool f = 1;
	for (const int& w : G[v]) {
		if (!~V[w]) {
			f = DFS2(w, v);
		}
		else if (V[w] == V[v]) return 0;
	}
	return f;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N >> E;
		while (E--) {
			std::cin >> u >> v;
			G[u].push_back(v);
			G[v].push_back(u);
		}
		memset(V, -1, sizeof V);
		F = 1;
		for (int i = 1; i <= N; i++) {
			if (!~V[i]) {
				//V[i] = 1;
				F = DFS2(i, -1);
			}
		}
		std::cout << (F ? "YES\n" : "NO\n");
		for (int i = 1; i <= N; i++) { G[i].clear(); }
	}
	return 0;
}