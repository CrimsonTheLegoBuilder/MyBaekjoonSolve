#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N, Q, T, x;
bool V[1001];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> Q;
	for (int i = 1; i <= N; i++) V[i] = 1;
	while (Q--) {
		std::cin >> T;
		V[T] ^= 1;
	}
	for (int i = 1; i <= N; i++) x += V[i];
	std::cout << x << "\n";
	return;
}
int main() { solve(); return 0; }