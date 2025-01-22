#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll N, S[55], C, T;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	C = 1;
	while (N) {
		T++;
		if (N < C) C = 1;
		N -= C;
		C++;
	}
	std::cout << T << "\n";
	return 0;
}