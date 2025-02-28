#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
int T, X, N, Ki, Ai;
int A[1005];

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	memset(A, 0, sizeof A);
	std::cin >> T >> X >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> Ki;
		for (int k = 0; k < Ki; k++) {
			std::cin >> Ai;
			A[Ai]++;
		}
	}
	//std::cout << (A[X] == N ? "YES\n" : "NO\n");
	return 0;
}