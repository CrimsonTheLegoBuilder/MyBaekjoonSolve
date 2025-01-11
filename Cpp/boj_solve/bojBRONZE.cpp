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

int N, P[11];
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	for (int i = 0; i < 10; i++) {
		std::cin >> N;
		for (int j = i; j < 10; j++) P[j] += N;
	}
	int df = 1e9, ret = 0;
	for (int i = 0; i < 10; i++) {
		int tmp = diff(P[i], 100);
		if (tmp == df && ret < P[i]) df = tmp, ret = P[i];
		else if (tmp < df) df = tmp, ret = P[i];

	}
	//std::cout << ret << "\n";
}