#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 1e6 + 1;
ll sq(ll x) { return x * x; }

int N;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cin >> N;
	int u = 1e6, r = 1e6, l = -1e6, d = -1e6;
	while (N--) {
		int x1, x2, y1, y2;
		std::cin >> x1 >> x2 >> y1 >> y2;
		u = std::min(u, y2);
		r = std::min(r, x2);
		l = std::max(l, x1);
		d = std::max(d, y1);
	}
	std::cout << std::max(u - d, 0) * std::max(r - l, 0) << "\n";
	return 0;
}