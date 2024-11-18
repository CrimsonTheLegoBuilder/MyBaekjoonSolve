#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef double ld;
typedef std::vector<int> Vint;
const int LEN = 1e6 + 1;
int sq(int x) { return x * x; }
ld sq(ld x) { return x * x; }

int N, a, b, c;
Vint v;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	v.resize(3);
	for (int i = 1; i <= N; i++) {
		std::cin >> v[0] >> v[1] >> v[2];
		std::sort(v.begin(), v.end());
		a = v[0], b = v[1], c = v[2];
		std::cout << "Case #" << i << ": ";
		if (a + b <= c) std::cout << "invalid!\n";
		else if (a == b == c)std::cout << "equilateral\n";
		else if (a == b || b == c) std::cout << "isosceles\n";
		else std::cout << "scalene\n";
	}
	return 0;
}