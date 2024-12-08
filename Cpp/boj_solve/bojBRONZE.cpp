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

int N, X, Y, U, R, L, D, A;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> N;
	U = 0;
	R = -1e9;
	L = 1e9;
	D = 0;
	A = 0;
	while (N--) {
		std::cin >> X >> Y;
		if (Y == 0) {
			L = std::min(X, L);
			R = std::max(X, R);
			A++;
		}
		else {
			D = std::min(Y, D);
			U = std::max(Y, U);
		}
	}
	if (A < 2) { std::cout << "0\n"; return 0; }
	A = 0;
	A = std::max(A, (R - L) * std::abs(U));
	A = std::max(A, (R - L) * std::abs(D));
	std::cout << (A >> 1) << "." << (A & 1) * 5 << "\n";
	return 0;
}