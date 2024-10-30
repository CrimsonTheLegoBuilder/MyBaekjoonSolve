#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef double ld;
const int LEN = 1e6 + 1;
int sq(int x) { return x * x; }
ld sq(ld x) { return x * x; }

int N, M[4];

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	while (N--) {
		std::cin >> M[0] >> M[1] >> M[2] >> M[3];
		std::sort(M, M + 4);
		if (M[0] == 4) std::cout << "YES\n";
		else if (M[3] == 3) std::cout << "YES\n";
		else if (M[1] / 4) std::cout << "NO\n";
		else if (M[2] / 4 && M[3] / 4) {
			M[2] %= 4;
			M[3] %= 4;
			if (M[0] + M[1] == 3) std::cout << "NO\n";
			else if (M[2] + M[3] == 3) std::cout << "NO\n";
			else if (M[0] == M[2] && M[1] == M[3]) std::cout << "YES\n";
			else std::cout << "NO\n";
		}
		else std::cout << "NO\n";
	}
	return 0;
}