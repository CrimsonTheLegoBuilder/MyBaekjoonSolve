#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 100;

int N;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= i; j++) {
			std::cout << "*";
		}
		for (int j = 0; j < N - i - 1; j++) {
			std::cout << " ";
		}
		std::cout << "\n";
	}
	for (int i = N - 2; i >= 0; i--) {
		for (int j = 0; j <= i; j++) {
			std::cout << "*";
		}
		for (int j = 0; j < N - i - 1; j++) {
			std::cout << " ";
		}
		//if (i > 0) std::cout << "\n";
	}
	return 0;
}