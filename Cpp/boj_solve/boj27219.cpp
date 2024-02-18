#include <iostream>

int main() {
	int n, v, i;
	std::cin >> n;
	v = n / 5; i = n % 5;
	while (v--) std::cout << "V";
	while (i--) std::cout << "I";
	std::cout << "\n";
	return 0;
}