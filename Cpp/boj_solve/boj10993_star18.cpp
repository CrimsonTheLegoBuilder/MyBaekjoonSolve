#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 2 << 10;

#define FUCK

int N, C, X;
bool M[LEN][LEN];
void recur(int x, int d, int b, int t) {
	if (d < 1) return;
	int s = C - x + 1, e = C + x - 1;
	if (d & 1) {
		for (int i = s; i <= e; i++) M[t - 1][i] = 1;
		for (int h = t - 1; h >= b; h--) {
			M[h][s] = M[h][e] = 1;
			s++; e--;
		}
	}
	else {
		for (int i = s; i <= e; i++) M[b][i] = 1;
		for (int h = b; h < t; h++) {
			M[h][s] = M[h][e] = 1;
			s++; e--;
		}
	}
	x >>= 1;
	if (d & 1) b += x, t--;
	else t -= x, b++;
	recur(x, d - 1, b, t);
	return;
}
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	memset(M, 0, sizeof M);
	X = (1 << N) - 1;
	C = X - 1;
	recur(X, N, 0, X);
	for (int i = 0; i < X; i++) {
		for (int j = 0; j <= C + (N & 1 ? i : C - i); j++) {
			std::cout << (M[i][j] ? "*" : " ");
		}
		std::cout << "\n";
	}
	return 0;
}