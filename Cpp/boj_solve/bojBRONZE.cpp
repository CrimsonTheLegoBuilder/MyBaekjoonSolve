#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 500;

int Q, C, V;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> Q;
	while (Q--) {
		std::cin >> C >> V;
		std::cout << "You get ";
		std::cout << C / V;
		std::cout << " piece(s) and your dad gets ";
		std::cout << C % V;
		std::cout << " piece(s).\n";
	}
	return 0;
}