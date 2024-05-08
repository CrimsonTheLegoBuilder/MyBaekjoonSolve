#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
typedef long long ll;
int N, A, B, C, D, E;

int main() {
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> A >> B >> C >> D >> E;
	N = 0;
	if (A < 0) N = -A * C + D + B * E;
	else if (A > 0) N = (B - A) * E;
	std::cout << N << "\n";
	return 0;
}
