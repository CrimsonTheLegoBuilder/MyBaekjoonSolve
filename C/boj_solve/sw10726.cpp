#include <iostream>
int t = 1, T, N, M, X;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> N >> M;
		X = 0;
		X = (1 << N) - 1;
		std::cout << "#" << t++ << " ";
		std::cout << ((M & X) == X ? "ON" : "OFF") << "\n";
	}
}