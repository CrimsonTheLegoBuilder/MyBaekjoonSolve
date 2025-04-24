#include <iostream>
#include <algorithm>
typedef long long ll;
// -2,147,483,648 ~ int ~ 2,147,483,647
//const ll MOD = 37;
const ll MOD = 1234567891;
int arr[50];

ll pow_(ll a, int B) {
	if (!B) return 1;
	if (B == 1) return a;
	int b = B >> 1;
	ll A = pow_(a, b);
	A %= MOD;
	//std::cout << A << "\n";
	if (B % 2) return A * A % MOD * a % MOD;
	else return A * A % MOD;
}

int main() {
	int n;
	char x[51];
	ll ans = 0, tmp;
	std::cin >> n >> x;
	for (int i = 0; i < n; i++) {
		arr[i] = x[i] - 'a' + 1;
	}
	for (int i = 0; i < n; i++) {
		tmp = pow_(31, i);
		ans += tmp * arr[i];
		ans %= MOD;
	}
	std::cout << ans;
}