#include <iostream>
#include <algorithm>
#include <Cstring>
#include <cmath>
typedef long long ll;
const int LEN = 1e5;
int N;
std::string S[5000];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	int i = 0;
	ll x = 1;
	ll h = 0;
	while (h <= N) {
		h += x;
		x *= 2;
		i++;
	}
	std::cout << i << "\n";
	return;
}
int main() { solve(); return 0; }
