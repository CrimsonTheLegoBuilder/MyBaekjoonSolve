#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
typedef long long ll;
int N, M, L;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> L;
	int cnt = 0;
	while (N--) {
		std::cin >> M;
		cnt += M >= L;
	}
	std::cout << cnt;
}