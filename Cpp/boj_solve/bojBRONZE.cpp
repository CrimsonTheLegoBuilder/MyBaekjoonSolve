#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
typedef long long ll;
ll N, ans;

int main() {
	std::cin >> N;
	ans = N % 8;
	if (!ans) std::cout << "2\n";
	else if (1 <= ans && ans <= 5) std::cout << ans << "\n";
	//else std::cout << 10 - ans << "\n";
	return 0;
}
