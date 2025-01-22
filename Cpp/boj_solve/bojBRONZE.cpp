#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll N, S[55], C, T;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N;
	while (N--) {
		std::string s;
		std::cin >> s;
		int t = s.size();
		int cnt = 0;
		for (int i = 0; i < t; i++) {
			if (s[i] == 'U') cnt++;
			else break;
		}
		std::cout << cnt << "\n";
	}
	return 0;
}