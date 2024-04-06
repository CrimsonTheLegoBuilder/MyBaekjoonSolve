#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
typedef long long ll;
int N;
std::map<ll, ll> MAP;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	ll A, C;
	for (int i = 0; i < N; i++) {
		std::cin >> A >> C;
		auto it = MAP.find(C);
		if (it == MAP.end()) MAP[C] = A;
		else if (it->second > A) it->second = A;
	}
	ll ret = -1;
	for (const auto& pair : MAP) if (pair.second > ret) ret = pair.second;
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }
