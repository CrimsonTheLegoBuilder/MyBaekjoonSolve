#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
typedef long long ll;

int main() {
	int N, M;
	ll P;
	std::cin >> N >> M >> P;
	while (N--) {
		int item = 0;
		std::priority_queue<ll> enemy;
		ll e;
		for (int i = 0; i < M; i++) {
			std::cin >> e;
			if (e == -1) item += 1;
			else enemy.push(-e);
		}
		while (enemy.size()) {
			ll x = -enemy.top(); enemy.pop();
			if (x > P) {
				while (x > P && item > 0) {
					item--;
					P <<= 1;
				}
				if (x > P) {
					std::cout << 0;
					return 0;
				}
			}
			P += x;
		}
		while (item--) P <<= 1;
	}
	std::cout << 1 << "\n";
	return 0;
}