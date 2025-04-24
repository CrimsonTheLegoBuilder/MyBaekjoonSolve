#include <iostream>
#include <algorithm>
const int LEN = 100'000;
int N, M, x, A[LEN];

bool bi_search(int x, int s = 0, int e = N) {
	while (s <= e) {
		int m = s + e >> 1;
		if (A[m] == x) return 1;
		if (A[m] < x) s = m + 1;
		if (A[m] > x) e = m - 1;
	}
	return 0;
}

int main() {
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> A[i];
	}
	std::sort(A, A + N);
	std::cin >> M;
	while (M--) {
		std::cin >> x;
		std::cout << bi_search(x) << "\n";
	}
	return 0;
}