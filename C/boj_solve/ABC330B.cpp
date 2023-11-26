#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
typedef long long ll;
int N, M, L, R;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> L >> R;
	while (N--) {
		std::cin >> M;
		if (M <= L) std::cout << L << " ";
		else if (M >= R) std::cout << R << " ";
		else if (L < M && M < R) std::cout << M << " ";
	}
	return 0;
}