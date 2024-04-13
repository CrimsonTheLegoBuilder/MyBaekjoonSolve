#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N, x;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	ll total = 0;
	N--;
	while (N--) std::cin >> x, total -= x;
	std::cout << total << "\n";
	return;
}
int main() { solve(); return 0; }