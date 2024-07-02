#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
ll N, H, V;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> H >> V;
	ll a, b, c, d;
	a = H * V;
	b = (N - H) * V;
	c = H * (N - V);
	d = (N - H) * (N - V);
	std::cout << (std::max({ a, b, c, d }) << 2) << "\n";
	return 0;
}