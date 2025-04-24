#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const ll MOD = 1e9 + 7;
const int LEN = 4e6 + 5;
ll MAX = 4e6;
ll N, M, K;
ll fac[LEN], inv[LEN];

ll power(ll a, ll b) {
	ll ret = 1;
	while (b) {
		if (b & 1) ret = ret * a % MOD;
		a = a * a % MOD;
		b >>= 1;
	}
	return ret;
}
void init() {
	fac[0] = 0, fac[1] = 1, fac[2] = 2, fac[3] = 6;
	for (int i = 4; i <= MAX; i++)
		fac[i] = (fac[i - 1] * i) % MOD;

	inv[MAX] = power(fac[MAX], MOD - 2);
	for (int i = MAX - 1; i >= 0; i--)
		inv[i] = (inv[i + 1] * (i + 1)) % MOD;
	return;
}
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	init();
	std::cin >> M;
	while (M--) {
		std::cin >> N >> K;
		ll ret = ((fac[N] * inv[N - K]) % MOD) * inv[K] % MOD;
		std::cout << ret << "\n";
	}
	return;
}
int main() { solve(); return 0; }//boj13977