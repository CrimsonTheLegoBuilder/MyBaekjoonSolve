#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
const int LEN = 200'001;
int N, M, cnt;
int V[LEN], A[LEN], B[LEN];
std::vector<int> G[LEN];
std::queue<int> Q;

bool bfs(int v) {
	Q.push(v);
	V[v] = 1;
	while (Q.size()) {
		int p = Q.front(); Q.pop();
		//cnt++;
		//if (cnt == N) return 1;
		for (const int& w : G[p]) {
			if (!~V[w]) {
				Q.push(w);
				V[w] = V[p] ^ 1;
			}
			if (V[p] == V[w]) return 0;
		}
	}
	return 1;
}


int main() {
	memset(V, -1, sizeof V);
	std::cin >> N >> M;
	for (int i = 0; i < M; i++) { std::cin >> A[i]; }
	for (int i = 0; i < M; i++) { std::cin >> B[i]; }
	for (int i = 0; i < M; i++) { 
		G[A[i]].push_back(B[i]);
		G[B[i]].push_back(A[i]);
	}
	bool f = bfs(A[0]);
	for (int i = 1; i <= N; i++) if (!~V[i]) f = 0;
	std::cout << (f ? "Yes" : "No") << "\n";
	return 0;
}