#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
ll a, b, c;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> a >> b >> c;
	ll deci;
	if (b == 1000) deci = 10000;
	else if (b >= 100) deci = 1000;
	else if (b >= 10) deci = 100;
	else deci = 10;
	std::cout << a + b - c << "\n" << a * deci + b - c;
	return 0;
}