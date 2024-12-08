#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 1e6 + 1;
int sq(int x) { return x * x; }

int N, X, Y, U, R, L, D;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> N;
	U = -1e9;
	R = -1e9;
	L = 1e9;
	D = 1e9;
	while (N--) {
		std::cin >> X >> Y;
		L = std::min(X, L);
		D = std::min(Y, D);
		std::cin >> X >> Y;
		R = std::max(X, R);
		U = std::max(Y, U);
		std::cout << ((R - L + U - D) << 1) << "\n";
	}
	return 0;
}