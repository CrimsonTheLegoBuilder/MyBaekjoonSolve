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

ll N, A[LEN], B, C;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	ll ret = 0;
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> A[i];
	std::cin >> B >> C;
	for (int i = 0; i < N; i++) {
		ll D = A[i] - B;
		ret++;
		if (D <= 0) continue;
		else ret += D / C + (D % C != 0);
	}
	std::cout << ret << "\n";
	return 0;
}