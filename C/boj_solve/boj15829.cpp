#include <iostream>
#include <algorithm>
typedef long long ll;
// -2,147,483,648 ~ int ~ 2,147,483,647
const int MOD = 1234567891;
int arr[50];

ll pow(ll a, ll B) {
	if (B == 1) return a;
	int b = B >> 1;
	ll A = pow(a, b);
	if (B % 2) return (A * A * a) % MOD;
	else return (A * A) % MOD;
}



int main() {
	int n;
	char x[51];
	ll ans = 0;
	std::cin >> n >> x;
	for (int i = 0; i < n; i++) {
		arr[i] = x[i] - 96;
	}
	for (int i = 0; i < n; i++) {
		ans += (arr[i] * pow(31, i));
		ans %= MOD;
	}
	std::cout << ans;
}