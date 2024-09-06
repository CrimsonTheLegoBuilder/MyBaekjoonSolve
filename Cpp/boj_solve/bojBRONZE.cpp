#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <unordered_set>

int N, A, B, ans = 2001;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N;
	while (N--) {
		std::cin >> A >> B;
		if (A <= B) ans = std::min(ans, B);
	}
	//std::cout << (ans != 2001 ? ans : -1) << "\n";
	return 0;
}