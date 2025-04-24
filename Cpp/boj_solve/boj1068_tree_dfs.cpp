#include <iostream>
#include <algorithm>
#include <vector>
const int LEN = 50;
int N, v, s, root, cnt;
std::vector<int> G[LEN];
bool V[LEN];

void DFS(int v) {
	if (v == s) return;
	V[v] = 1;
	bool f = 1;
	for (const int& w : G[v]) {
		if (w == s) continue;
		if (!V[w]) {
			DFS(w);
			f = 0;
		}
	}
	if (f) cnt++;
	return;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> v;
		if (~v) G[v].push_back(i);
		if (v == -1) root = i;
	}
	std::cin >> s;
	DFS(root);
	std::cout << cnt << "\n";
	return 0;
}

/*
6
-1 0 0 1 2 3
3

*/
