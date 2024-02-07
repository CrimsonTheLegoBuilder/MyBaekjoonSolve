#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <cmath>
typedef long long ll;
ll a, b, N;

int main() {
	//std::cin >> a >> b >> N;
	if (a + b >= N << 1) std::cout << a + b - (N << 1) << "\n";
	else std::cout << a + b << "\n";
	return 0;
}
