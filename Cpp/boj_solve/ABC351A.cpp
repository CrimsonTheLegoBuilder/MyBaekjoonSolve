#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 1e5;
int N;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int ret = 0;
	int cnt = 9, x;
	while (cnt--) std::cin >> x, ret += x;
	cnt = 8;
	while (cnt--) std::cin >> x, ret -= x;
	std::cout << ret + 1;
	return;
}
int main() { solve(); return 0; }

