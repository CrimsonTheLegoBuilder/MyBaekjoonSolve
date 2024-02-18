#include <iostream>

int main() {
	int N, a;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> a;
		std::cout << a << " " << a << "\n";
	}
	return 0;
}