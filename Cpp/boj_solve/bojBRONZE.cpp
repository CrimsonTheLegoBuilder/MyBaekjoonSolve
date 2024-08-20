#include <iostream>
#include <algorithm>
#include <cmath>

int N, h, w, ans;
int main() {
	std::cin >> N;
	ans = 0;
	while (N--) {
		std::cin >> w >> h;
		ans = std::max(ans, w * h);
	}
	//std::cout << ans << "\n";
}