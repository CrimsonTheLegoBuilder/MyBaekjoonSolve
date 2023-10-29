#include <iostream>

int t = 1, T, N, M, K, X;
const int V = (1 << 10) - 1;

int main() {
	std::cin >> T;
	while (T--) {
		X = 0;
		std::cin >> N;
		K = N;
		while (X != V) {
			M = K;
			while (M) {
				X |= 1 << (M % 10);
				M /= 10;
			}
			K += N;
			//std::cout << K << " " << X << " " << V << "\n";
		}
		std::cout << "#" << t++ << " " << K - N << "\n";
	}
}