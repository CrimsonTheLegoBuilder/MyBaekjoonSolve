#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-9;
inline bool zero(ld x) { return -TOL < x && x < TOL; }

int X, Y, C;
int main() {
	std::cin >> X >> Y >> C;
	ld S = hypot(X, Y);
	int step = (int)(S / C);
	if (!X && !Y) std::cout << "0\n";
	else if (S < C) std::cout << "2\n";
	else std::cout << step + (zero(step * C - S) ^ 1) << "\n";
	return 0;
}