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

int N, M[1001], X;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	memset(M, 0, sizeof M);
	int cnt = 0;
	for (int i = 0; i < 10; i++) {
		std::cin >> X;
		M[X]++;
		N += X;
	}
	std::cout << N / 10 << "\n";
	for (int i = 1000; i > 0; i -= 10) {
		if (cnt < M[i]) { N = i; cnt = M[i]; }
	}
	//std::cout << N << "\n";
	return 0;
}