#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long double ld;
const int LEN = 101;
const ld TOL = 1e-8;
int N, M, match[LEN];
bool done[LEN];
ld S, V;
std::vector<int> G[LEN];

struct Pos {
	ld x, y;
} R[LEN], H[LEN];

ld dist(const Pos& a, const Pos& b) {
	ld x = a.x - b.x, y = a.y - b.y;
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

int main() {
	std::cin.tie()->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M >> S >> V;
	for (int i = 0; i < N; i++) { std::cin >> R[i].x >> R[i].y; }
	for (int i = 0; i < M; i++) { std::cin >> H[i].x >> H[i].y; }
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			//if (dist(g[i], H[j]) <= S * V) G[i].push_back(j);
			if (dist(R[i], H[j]) - S * V < -TOL) G[i].push_back(j);
		}
	}
	memset(match, -1, sizeof match);
	int C = N;
	for (int i = 0; i < N; i++) {
		memset(done, 0, sizeof done);
		C -= DFS(i);
	}
	std::cout << C << "\n";
	return 0;
}