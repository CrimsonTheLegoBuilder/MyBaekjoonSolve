#include <iostream>

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int t;
	std::cin >> t;
	while (t--) {
		int h, w, n;
		std::cin >> h >> w >> n;
		std::cout << 100 * (n % h ? n % h : h) + n / h + (n % h ? 1 : 0) << "\n";
	}
	return 0;
}