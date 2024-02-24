#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long double ld;
const int LEN = 201;
const ld TOL = 1e-8;
int N, M, S, match[LEN];
bool done[LEN];

std::vector<int> G[LEN];
bool bi_matching(int i) {
	if (done[i]) return 0;
	done[i] = 1;
	for (const int& w : G[i]) {
		if (!~match[w] || bi_matching(match[w])) {
			match[w] = i;
			return 1;
		}
	}
	return 0;
}
void solve() {
	std::cin.tie()->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) {
		std::cin >> S;
		int j = 0;
		G[i].resize(S);
		while (S--) std::cin >> G[i][j++];
	}
	memset(match, -1, sizeof match);
	int C = N;
	for (int i = 0; i < N; i++) {
		memset(done, 0, sizeof done);
		C -= bi_matching(i);
	}
	std::cout << N - C << "\n";

}
int main() { solve(); return 0; }//boj2188 bipartite maching