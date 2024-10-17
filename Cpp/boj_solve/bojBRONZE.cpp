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

ll N, A, B, C;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	N = 0; C = 0;
	std::cin >> A >> B;
	for (int i = 1; i < 46; i++) {
		for (int j = 0; j < i; j++) {
			//std::cout << i << " " << C << "\n";
			C++;
			if (A <= C && C <= B) {
				N += i;
				//std::cout << i << " fuck " << N << "\n";
			}
		}
	}
	std::cout << N << "\n";
	return 0;
}