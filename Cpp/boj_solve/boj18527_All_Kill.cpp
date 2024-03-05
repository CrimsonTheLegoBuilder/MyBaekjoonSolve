#include <iostream>
#include <algorithm>
typedef long long ll;
const ll MOD = 998244353;
const ll LEN = 1e5;
int N;
ll T, num[LEN];

ll POW(ll a, ll b) {
	ll ret = 1;
	while (b) {
		if (b & 1) ret = ret * a % MOD;
		b >>= 1; a = a * a % MOD;
	}
	return ret;
}
void solve() {
	std::cin >> N >> T;
	T++;
	ll P = 1;
	for (int i = 0; i < N; i++) std::cin >> num[i];
	for (int i = N - 1; i >= 0; i--) {
		T -= num[i] - 1;
		P *= T;
		P %= MOD;
	}
	P *= T - N;
	P %= MOD;
	P *= POW(T, MOD - 2);
	P %= MOD;
	std::cout << P << "\n";
	return;
}
int main() { solve(); return 0; }//refer to seastar105