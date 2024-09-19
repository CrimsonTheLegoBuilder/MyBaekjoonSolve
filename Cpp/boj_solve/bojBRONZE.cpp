#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld METER = 1609.344;
const ld LITER = 3.785411784;
const ld PI = acos(-1);
ld A;

int main() {
	std::cout << std::fixed;
	std::cout.precision(7);
	//std::cin >> A;

	ld t = atan2(12.566, 56.5);
	std::cout << t * 180 / PI << "\n";

	//std::cout << 100000. / (A / LITER * METER) << "\n";
	return 0;
}
