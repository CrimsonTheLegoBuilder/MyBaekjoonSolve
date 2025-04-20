#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cassert>
#include <vector>
typedef long long ll;
//typedef long double ld;
typedef double ld;

int N;
char c;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> c;
		if (c == 'I') std::cout << "i";
		if (c == 'l') std::cout << "L";
	}
	return 0;
}