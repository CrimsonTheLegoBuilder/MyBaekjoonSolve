#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
ll N, A, B, C, D, T, R, G, P, U, V, W, K, X, L;
ld F;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(9);
	std::cin >> N >> U >> L;
	if (N >= 1000 && (U >= 8000 || L >= 260)) std::cout << "Very Good\n";
	else if (N >= 1000) std::cout << "Good\n";
	else std::cout << "Bad\n";
	return 0;
}