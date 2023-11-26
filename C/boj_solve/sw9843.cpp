#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;

ll bi_search(ll N) {
	ll s = 1, e = 1e10, m;
	while (s < e) {
		m = s + e >> 1;
		ll c = (m + 1) * m >> 1;
		if (c == N) return m;
		else if (c > N) e = m;
		else s = m + 1;
	}
	return -1;
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	ll T, N;
	std::cin >> T;
	int t = 1;
	while (T--) {
		std::cin >> N;
		std::cout << "#" << t++ << " " << bi_search(N) << "\n";
	}
	return 0;
}