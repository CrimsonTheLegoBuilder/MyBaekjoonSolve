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

int N, M[100], T, D;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	int s = 5;
	for (int i = 2; i <= N; i++) {
		s = s + i * 3 + 1;
		s %= 45678;
	}
	std::cout << s << "\n";
	return 0;
}