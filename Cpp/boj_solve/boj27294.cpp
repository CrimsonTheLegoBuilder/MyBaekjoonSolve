#include <iostream>

int main() {
	int t, s;
	std::cin >> t >> s;
	int ans = 320;
	if (s || t < 12 || t > 16) ans = 280;
	std::cout << ans;
	return 0;
}