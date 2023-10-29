#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
const int LEN = 3e5;
int N, M, K[LEN], Q;



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) {
		std::cin >> K[i];
	}
	std::sort(K, K + N);
	for (int l = 0, r = 0; r < N;  l++) {
		while (r < N && K[r] - K[l] < M) {
			r++;
		}
		Q = std::max(Q, r - l);
	}
	std::cout << Q;
	return 0;
}