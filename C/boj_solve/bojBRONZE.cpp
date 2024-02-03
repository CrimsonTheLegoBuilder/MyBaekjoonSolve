#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
int a, b, c;

int main() {
	std::cin >> a >> b >> c;
	if (a == b || 
		b == c || 
		c == a || 
		(a + b) == c || 
		(b + c) == a || 
		(c + a) == b) std::cout << "S\n";
	else std::cout << "N\n";
	return 0;
}

