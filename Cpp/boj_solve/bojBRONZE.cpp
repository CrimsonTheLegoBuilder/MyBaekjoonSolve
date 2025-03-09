#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
int T, X, N, M, Ki, Ai;
int A[1005];

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> X >> N;
	for (int i = 1; i < N; i++) {
		if (i & 1) X = (X / 2) ^ 6;
		else X = (X * 2) ^ 6;
	}
	std::cout << X << "\n";
	return 0;
}