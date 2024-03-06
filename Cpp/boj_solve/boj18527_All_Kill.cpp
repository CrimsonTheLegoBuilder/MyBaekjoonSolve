#include <iostream>
#include <algorithm>
#include <numeric>
typedef long long ll;
const ll MOD = 998244353;
const ll LEN = 1e5;
int N;
ll T, S[LEN];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cin >> N >> T;
	for (int i = 0; i < N; i++) std::cin >> S[i], S[i];
	ll SUM = 0;
	for (int i = 0; i < N; i++) SUM += S[i];
	ll P = T - SUM + 1;
	ll tmp = S[N - 1] - 1;
	for (int i = N - 2; i >= 0; i--) {
		P = P * (T - tmp + 1) % MOD;
		tmp += S[i] - 1;
	}
	//for (int i = 2; i <= N; i++) {
	//	P = P * (T - tmp + 1) % MOD;
	//	tmp += S[N - i] - 1;
	//}
	std::cout << P << "\n";
	return;
}
int main() { solve(); return 0; }//boj18527 All Kill refer to koosaga

//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cin >> N >> T;
//	for (int i = 0; i < N; i++) std::cin >> S[i], S[i]--;
//	ll SUM = 0;
//	for (int i = 0; i < N; i++) SUM += S[i];
//	ll P = T - N - SUM + 1;
//	for (int i = 0; i < N - 1; i++) {
//		SUM -= S[i];
//		P = P * (T - SUM + 1) % MOD;
//	}
//	std::cout << P << "\n";
//	return;
//}
//int main() { solve(); return 0; }//boj18527 All Kill refer to koosaga


//ll POW(ll a, ll b) {
//	ll ret = 1;
//	while (b) {
//		if (b & 1) ret = ret * a % MOD;
//		b >>= 1; a = a * a % MOD;
//	}
//	return ret;
//}
//void solve() {
//	std::cin >> N >> T;
//	T++;
//	ll P = 1;
//	for (int i = 0; i < N; i++) std::cin >> num[i];
//	for (int i = N - 1; i >= 0; i--) {
//		T -= num[i] - 1;
//		P *= T;
//		P %= MOD;
//	}
//	P *= T - N;
//	P %= MOD;
//	P *= POW(T, MOD - 2);
//	P %= MOD;
//	std::cout << P << "\n";
//	return;
//}
//int main() { solve(); return 0; }//refer to seastar105