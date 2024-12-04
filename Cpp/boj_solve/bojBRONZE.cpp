#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
#include <cassert>
typedef long long ll;
typedef long double ld;
const int LEN = 1e6 + 1;
int sq(int x) { return x * x; }

int a, b, c;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cout << std::fixed;
	std::cout.precision(3);
	int t = 1;
	while (1) {
		std::cin >> a >> b >> c;
		if (!a && !b && !c) break;
		std::cout << "Triangle #" << t++ << "\n";
		if (c > 0 && (a >= c || b >= c)) { std::cout << "Impossible.\n\n"; continue; }
		if (a < 0) std::cout << "a = ";
		else if (b < 0) std::cout << "b = ";
		else if (c < 0) std::cout << "c = ";
		if (a == -1 || b == -1) {
			if (b == -1) std::swap(a, b);
			else std::cout << sqrt(sq(c) - sq(b)) << "\n";
		}
		else {
			//std::cout << sqrt(sq(a) + sq(b)) << "\n";
		}
		std::cout << "\n";
	}
	return 0;
}