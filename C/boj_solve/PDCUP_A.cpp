#include <iostream>
#include <algorithm>

int main() {
	int T, c ,S = 0, N;
	std::cin >> T >> N;
	while (N--) {
		std::cin >> c;
		S += c;
	}
	std::cout << "Padaeng_i " << (T > S ? "Cry" : "Happy");
}