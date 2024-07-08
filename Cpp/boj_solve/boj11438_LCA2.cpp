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
int P[LEN][20];
int LV[LEN];
void dfs(int v, int lv = 0) {
	LV[v] = lv;
	if (LV[v] > 0) {
		for (int i = 1; i < 19; i++)
			P[v][i] = P[P[v][i - 1]][i - 1];
	}
	for (const int& w : G[v]) {
		if (LV[w] == -1) {
			P[w][0] = v;
			dfs(w, lv + 1);
		}
	}
	return;
}
int lca(int a, int b) {
	if (LV[a] < LV[b]) std::swap(a, b);
	if (LV[a] != LV[b]) {
		for (int i = 19; i >= 0; i--) {
			if (LV[P[a][i]] >= LV[b]) a = P[a][i];
		}
	}
	if (a == b) return a;
	for (int i = 19; i >= 0; i--) {
		if (P[a][i] != P[b][i]) {
			a = P[a][i];
			b = P[b][i];
		}
	}
	return P[a][0];
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1, j, k; i < N; i++) {
		std::cin >> j >> k;
		G[j].push_back(k);
		G[k].push_back(j);
	}
	memset(LV, -1, sizeof LV);
	//memset(P, 0, sizeof P);
	dfs(1);

	//for (int i = 1; i <= N; i++) {
	//	for (int j = 0; j < 4; j++) std::cout << P[i][j] << " ";
	//	std::cout << "\n";
	//}
	std::cin >> M;
	while (M--) {
		int a, b;
		std::cin >> a >> b;
		std::cout << lca(a, b) << "\n";
	}
	return;
}
int main() { solve(); return 0; }