#include <iostream>
const int LEN = 100'001;
int N, K, DP[LEN];


int main() {
	std::cin >> N >> K;
	if (N >= K) std::cout << N - K << "\n";
	else {
		for (int i = N / 2; i <= K; i++) {
			DP[i] = std::abs(i - N);
		}
		for (int i = N; i <= K; i++) {
			DP[i] = std::min({ DP[i], DP[i - 1] + 1, DP[i / 2 + i % 2] + i % 2, DP[i / 2] + i % 2 });
			if (i << 1 < LEN) DP[i << 1] = std::min({ DP[i], DP[i << 1] });
			if (i - 1 >= 0) DP[i - 1] = std::min({ DP[i] + 1, DP[i - 1] });
		}
		std::cout << DP[K] << "\n";
	}
	return 0;
}