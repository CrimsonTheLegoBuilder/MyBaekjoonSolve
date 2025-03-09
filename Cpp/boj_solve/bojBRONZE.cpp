#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll T, N, W;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	T = 1;
	for (ll i = 1; i <= N; i++) T *= i;
	W = 60ll * 60 * 24 * 7;
	//std::cout << T / W << "\n";
	return 0;
}