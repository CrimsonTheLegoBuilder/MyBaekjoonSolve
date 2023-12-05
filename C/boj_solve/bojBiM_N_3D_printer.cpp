#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 33000;
ll N, H[LEN], R = 0;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	R += N << 1;
	for (int i = 0; i < N; i++) {
		std::cin >> H[i];
		R += H[i] << 2;
	}
	for (int i = 0; i < N - 1; i++) {
		R -= std::min(H[i], H[i + 1]);
		R -= std::min(H[i], H[i + 1]);
	}
	std::cout << R << "\n";
	return 0;
}