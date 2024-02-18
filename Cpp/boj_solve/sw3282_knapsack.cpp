#include <iostream>
#include <algorithm>
#include <cstring>
int T, t = 1, N, K, W, V, DP[1'001];


int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		memset(DP, 0, sizeof DP);
		std::cin >> N >> K;
		for (int i = 0; i < N; i++) {
			std::cin >> W >> V;
			for (int l = K; l >= W; l--) {
				DP[l] = std::max(DP[l - W] + V, DP[l]);
			}
		}
		std::cout << "#" << t++ << " " << DP[K] << "\n";
	}
	return 0;
}