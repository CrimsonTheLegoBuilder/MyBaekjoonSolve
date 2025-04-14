#include <iostream>
#include <algorithm>
typedef long long ll;
typedef double ld;
//typedef long double ld;

int P, Q, A, B;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(2);
	std::cin >> P >> Q >> A >> B;
	std::cout << A * std::min(P, Q) + B * std::max(0, Q - P) << "\n";
	return 0;
}