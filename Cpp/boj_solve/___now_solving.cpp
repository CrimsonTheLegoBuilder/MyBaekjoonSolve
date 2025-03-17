#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>

int a, b, x, y, T, A, B;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> a >> x >>b >> y >> T;
	A = a + 21 * x * std::max(0, T - 30);
	B = b + 21 * y * std::max(0, T - 45);
	std::cout << A << " " << B << "\n";
	return 0;
}