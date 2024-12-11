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

int N, Q, ret[LEN];
ll X[LEN], Y[LEN], Z[LEN];
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> X[i] >> Y[i] >> Z[i];
	std::cin >> Q;
	for (int q = 0; q < Q; q++) {
		ll x, y, z, r;
		std::cin >> x >> y >> z >> r;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			ll d = sq(x - X[i]) + sq(y - Y[i]) + sq(z - Z[i]);
			cnt += d <= sq(r);
		}
		ret[q] = cnt;
	}
	//for (int q = 0; q < Q; q++) std::cout << ret[q] << "\n";
	return 0;
}