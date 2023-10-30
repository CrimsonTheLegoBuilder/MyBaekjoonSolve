#include <iostream>
int a, b;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	while (std::cin >> a >> b) {
		std::cout << a + b;
	}
	return 0;
}