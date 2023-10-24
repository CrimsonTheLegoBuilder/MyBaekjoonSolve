#include <iostream>
#include <algorithm>
int N, K, W, V, DP[100'001];


int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> K;
	for (int i = 0; i < N; i++) {
		std::cin >> W >> V;
		for (int l = K; l >= W; l--) {
			DP[l] = std::max(DP[l - W] + V, DP[l]);
		}
	}
	std::cout << DP[K] << "\n";
	return 0;
}