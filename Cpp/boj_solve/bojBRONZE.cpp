#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll A, B, C;
ld N, M;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> A >> B >> C;
	N = (ld)(A * B) / C;
	M = A / (ld)(B * C);
	std::cout << std::max(N, M) << "\n";
	return 0;
}