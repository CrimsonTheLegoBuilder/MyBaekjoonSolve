#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>
typedef long long ll;
int N, M, K, Q;



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M;
	K = M - N;
	std::cout << (-3 <= K && K <= 2 ? "Yes" : "No");
	return 0;
}