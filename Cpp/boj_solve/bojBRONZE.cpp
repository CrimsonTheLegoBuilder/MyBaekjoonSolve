#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
int N;

int main() {
	std::cin >> N;
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			std::cout << "G";
		}
		for (int k = 0; k < N * 3; k++) {
			std::cout << ".";
		}
		std::cout << "\n";
	}
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N; k++) {
			std::cout << ".";
		}
		for (int k = 0; k < N; k++) {
			std::cout << "I";
		}
		for (int k = 0; k < N; k++) {
			std::cout << ".";
		}
		for (int k = 0; k < N; k++) {
			std::cout << "T";
		}
		std::cout << "\n";
	}
	for (int j = 0; j < N; j++) {
		for (int k = 0; k < N * 2; k++) {
			std::cout << ".";
		}
		for (int k = 0; k < N; k++) {
			std::cout << "S";
		}
		for (int k = 0; k < N; k++) {
			std::cout << ".";
		}
		//std::cout << "\n";
	}
	return 0;
}