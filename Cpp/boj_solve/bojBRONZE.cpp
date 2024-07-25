#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
typedef long long ll;
typedef long double ld;
const ld TOL = 1e-6;
int X, Y;

int main() {
	std::cin >> X >> Y;
	X -= 24;
	Y -= 24;
	for (int i = X; i <= Y; i += 60) {
		//std::cout << "All positions change in year " << i + 24 << "\n";
	}
	return 0;
}