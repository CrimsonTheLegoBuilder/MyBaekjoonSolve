#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
const int LEN = 2e5 + 5;
int N, M, Q, X;
int f[LEN], F[21][LEN];

int solve(const int& v, const int& k) {
	int cur = v;
	for (int i = 20; i >= 0; i--) {
		if (k & (1 << i)) cur = F[i][cur];
	}
	return cur;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> M;
	for (int j = 1; j <= M; j++) std::cin >> f[j];
	for (int i = 1; i <= M; i++) F[0][i] = f[i];
	for (int k = 1; k < 21; k++) {
		for (int i = 1; i <= M; i++) {
			int tmp = F[k - 1][i];
			F[k][i] = F[k - 1][tmp];
		}
	}
	std::cin >> Q;
	while (Q--) {
		std::cin >> N >> X;//MAX_N = 500'000 ~= 2^18
		std::cout << solve(X, N) << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj17435