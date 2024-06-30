#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld PI = acos(-1);
ll N, a1, p1, r1, p2;

int main() {
	std::cout << std::fixed;
	std::cout.precision(7);
	std::cin >> N;
	while (N--) {
		std::cin >> a1 >> p1 >> r1 >> p2;
		if ((ld)p1 / a1 < (ld)p2 / (PI * r1 * r1)) std::cout << "Slice of pizza\n";
		//else std::cout << "Whole pizza\n";
	}
	return 0;
}
