#include <iostream>
#include <algorithm>
int N, DP[100], G[20][2];

int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> G[i][0];
	}
	for (int i = 0; i < N; i++) {
		std::cin >> G[i][1];
	}
	for (int i = 0; i < N; i++) {
		for (int l = 99; l >= G[i][0]; l--) {
			DP[l] = std::max(DP[l - G[i][0]] + G[i][1], DP[l]);
		}
	}
	std::cout << DP[99] << "\n";
	return 0;
}