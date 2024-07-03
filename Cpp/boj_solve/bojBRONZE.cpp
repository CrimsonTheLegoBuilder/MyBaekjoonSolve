#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
ll A, P, C, M;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> A >> P >> C;
	M = (A + C > P ? A + C : P);
	std::cout << M << "\n";
	return 0;
}