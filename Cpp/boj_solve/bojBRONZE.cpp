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

int N, V, E;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(4);
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> V >> E;
		//std::cout << 2 - V + E << "\n";
	}
	return 0;
}