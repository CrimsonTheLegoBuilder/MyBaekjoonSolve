#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;
const int LEN = 40'002;
int N, M, R, pile[2001], pole[LEN], MAX, l, r;
bool H[LEN], W[LEN];


int main() {
	std::cin >> N >> M >> R;
	for (int i = 0; i < N; i++) { std::cin >> pile[i]; }
	for (int i = 0; i < M; i++) { std::cin >> pole[i]; }
	std::sort(pile, pile + N);
	for (int i = 0; i < M; i++) {
		H[pole[i]] = 1;
	}
	std::sort(pole, pole + M);
	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			W[pile[j] - pile[i]] = 1;
		}
	}
	for (l = 0, r = LEN - 2; l < LEN; l++) {
		if (W[l]) {
			while (r > 0 && (!H[r] || l * r > R * 2)) r--;
			if (l * r > MAX) MAX = l * r;
		}
	}
		std::cout << std::fixed;
	std::cout.precision(1);
	if (!MAX) std::cout << "-1\n";
	else std::cout << (ld)MAX / 2;
	return 0;
}

//bool z(ld x, int y) { return std::abs(x - ld(y)) < TOL; }
//int bi_search(int R, int w) {
//	int s = 0, e = M - 1, m = 0;
//	while (s < e) {
//		m = s + e >> 1;
//		if (w * pole[m] <= R * 2) s = m + 1;
//		else e = m;
//	}
//	return pole[e];
//}
//
//int main() {
//	std::cin >> N >> M >> R;
//	for (int i = 0; i < N; i++) { std::cin >> pile[i]; }
//	for (int i = 0; i < M; i++) { std::cin >> pole[i]; }
//	std::sort(pile, pile + N);
//	std::sort(pole, pole + M);
//	for (int i = 0; i < N; i++) {
//		for (int j = i; j < N; j++) {
//			if (pile[j] - pile[i]) S.insert(pile[j] - pile[i]);
//		}
//	}
//	for (const int& w : S) {
//		int h = bi_search(R, w);
//		if (h * w <= R * 2) {
//			MAX = std::max(h * w, MAX);
//		}
//	}
//	if (MAX == -1) {
//		std::cout << "-1\n";
//		return 0;
//	}
//	std::cout << std::fixed;
//	std::cout.precision(1);
//	std::cout << (ld)MAX / 2;
//	return 0;
//}