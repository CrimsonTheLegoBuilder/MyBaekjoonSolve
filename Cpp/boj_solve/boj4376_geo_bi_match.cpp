#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cmath>
typedef long double ld;
const ld TOL = 1e-8;
const int LEN = 101;
int N, M, match[LEN];
bool done[LEN];
ld S, V;
std::vector<int> G[LEN];

struct Pos { ld x, y; } g[LEN], H[LEN];

ld dist(const Pos& d1, const Pos& d2) {
	ld x = d1.x - d2.x, y = d1.y - d2.y;
	return hypot(x, y);
}

bool DFS(int i) {
	if (done[i]) return 0;
	done[i] = 1;
	for (const int& w : G[i]) {
		if (!~match[w] || DFS(match[w])) {
			match[w] = i;
			return 1;
		}
	}
	return 0;
}

//int main() {
//	std::cin.tie()->sync_with_stdio(0);
//	std::cout.tie(0);
//	std::cin >> N >> M >> S >> V;
//	for (int i = 0; i < N; i++) { std::cin >> g[i].x >> g[i].y; }
//	for (int i = 0; i < M; i++) { std::cin >> H[i].x >> H[i].y; }
//	for (int i = 0; i < N; i++) {
//		for (int j = 0; j < M; j++) {
//			//if (dist(g[i], H[j]) <= S * V) G[i].push_back(j);
//			if (dist(g[i], H[j]) - S * V < -TOL) G[i].push_back(j);
//		}
//	}
//	memset(match, -1, sizeof match);
//	int v = N;
//	for (int i = 0; i < N; i++) {
//		memset(done, 0, sizeof done);
//		v -= DFS(i);
//	}
//	std::cout << v << "\n";
//	return 0;
//}
int main() {
	std::cin.tie()->sync_with_stdio(0);
	std::cout.tie(0);
 	//freopen("gopherin.txt", "r", stdin);
 	//freopen("gophero.txt", "w", stdout);
	while (std::cin >> N >> M >> S >> V) {
		for (int i = 0; i < N; i++) { std::cin >> g[i].x >> g[i].y; }
		for (int i = 0; i < M; i++) { std::cin >> H[i].x >> H[i].y; }
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				//if (dist(g[i], H[j]) <= S * V) G[i].push_back(j);
				if (dist(g[i], H[j]) - S * V < -TOL) G[i].push_back(j);
			}
		}
		memset(match, -1, sizeof match);
		int v = N;
		for (int i = 0; i < N; i++) {
			memset(done, 0, sizeof done);
			v -= DFS(i);
		}
		std::cout << v << "\n";
		for (int i = 0; i < LEN; i++) G[i].clear();
	}
	return 0;
}