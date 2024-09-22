#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;

ll N;
int main() {
	std::cin >> N;
	if (-(1ll << 15) <= N && N < (1ll << 15)) std::cout << "short";
	else if (-(1ll << 31) <= N && N < (1ll << 31)) std::cout << "int";
	else std::cout << "long long";
	return 0;
}