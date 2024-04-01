#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 3e5 + 1;
const ll MOD = 1'000'000'007;
int N;
ll sco[LEN];

ll pow_m(ll A, ll B, const ll& mod = MOD) {  //divide & conquer
	if (!B) return 1;
	if (B == 1) return A;
	ll b = B >> 1;
	ll A_ = pow_m(A, b, mod);
	if (B % 2) return (((A_ * A_) % mod) * A) % mod;
	else return (A_ * A_) % mod;
}

ll pow_fuck(ll a, ll b) {
	ll ret = 1;
	while (b) {
		if (b & 1) ret = ret * a % MOD;
		a = a * a % MOD;
		b >>= 1;
	}
	return ret;
}

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> sco[i];
	std::sort(sco, sco + N);
	ll ans = 0;
	for (int i = 0; i < N; i++) {
		ll tmp = sco[i];
		//ans = (ans + tmp * ((pow_m(2, i) - pow_m(2, N - i - 1)) % MOD) + MOD) % MOD;
		ans = (ans + tmp * ((pow_fuck(2, i) - pow_fuck(2, N - i - 1)) % MOD) + MOD) % MOD;
	}
	std::cout << ans << "\n";
	return;
}
int main() { solve(); return 0; }//boj15824