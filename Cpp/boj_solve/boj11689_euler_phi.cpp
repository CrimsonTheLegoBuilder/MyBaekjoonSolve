#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
ll N;

ll Euler_phi(const ll& N) {
	ll ans = N, num = N;
	for (ll i = 2; i * i <= N; i++) {
		if (!(num % i)) {
			ans /= i;
			ans *= i - 1;
			while (!(num % i)) num /= i;
		}
	}
	if (num != 1)
		//std::cout << "fuck",
		ans /= num,
		ans *= num - 1;
	return ans;
}
void solve() {
	std::cin >> N;
	std::cout << Euler_phi(N) << "\n";
	return;
}
int main() { solve(); return 0; }//boj11689 GCD(n, k) = 1
