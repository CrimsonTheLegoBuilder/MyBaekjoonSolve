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

int N, X;
bool M[LEN][LEN];
void recur(int x) {
	if (x < 0) return;
	for (int i = x; i < X - x; i++) {
		M[x][i] = 1;
		M[X + 2 - x - 1][i] = 1;
	}
	for (int i = x; i < X + 1 - x; i++) {
		M[i][x] = 1;
		if (i > x + 1) M[i][X - x - 1] = 1;
	}
	if (x < X - x) M[x][X - x] = 1;
	recur(x - 2);
	return;
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	memset(M, 0, sizeof M);
	X = (N - 1) * 4 + 1;
	recur(N * 2);
	if (N == 1) { std::cout << "*"; return 0; }
	for (int i = 0; i < X + 2; i++) {
		if (i == 1) {
			std::cout << "*\n";
			continue;
		}
		for (int j = 0; j < X; j++) {
			std::cout << (M[i][j] ? "*" : " ");
		}
		//if (i < X + 1) std::cout << "\n";
	}
	return 0;
}