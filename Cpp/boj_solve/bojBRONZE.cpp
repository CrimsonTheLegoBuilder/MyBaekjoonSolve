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
ll sq(ll x) { return x * x; }
int diff(int x, int y) { return std::abs(x - y); }

int A, B, D, N, M;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	N = M = 0;
	std::cin >> A >> B;
	D = 1;
	while (A) {
		int r = A % 10;
		if (r == 5 || r == 6) { N += 5 * D; M += 6 * D; }
		else { N += r * D; M += r * D; }
		D *= 10;
		A /= 10;
	}
	D = 1;
	while (B) {
		int r = B % 10;
		if (r == 5 || r == 6) { N += 5 * D; M += 6 * D; }
		else { N += r * D; M += r * D; }
		D *= 10;
		B /= 10;
	}
	//std::cout << N << " " << M << "\n";
}