#include <iostream>
int N, S, X, SUM;

int main() {
	std::cin >> N >> S;
	while (N--) {
		std::cin >> X;
		if (S >= X) SUM += X;
	}
	std::cout << SUM;
}
