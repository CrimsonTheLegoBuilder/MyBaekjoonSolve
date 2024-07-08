#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <cstring>
typedef long long ll;
const int LEN = 1e5 + 5;

int N, M;
std::vector<int> G[LEN];
int P[LEN];
bool V[LEN];
void dfs(int v) {
	V[v] = 1;
	for (const int& w : G[v]) {
		if (!V[w]) P[w] = v, dfs(w);
	}
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i < N; i++) {
		int u, v;
		std::cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	memset(V, 0, sizeof V);
	dfs(1);
	//for (int i = 2; i <= N; i++) std::cout << P[i] << "\n";
}
int main() { solve(); return 0; }//boj11725