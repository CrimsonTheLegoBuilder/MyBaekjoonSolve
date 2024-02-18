#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
int N;
ll DP[100];

int main() {
	DP[0] = 0; DP[1] = 1; DP[2] = 1;
	for (int i = 3; i < 100; i++) {
		DP[i] = DP[i - 1] + DP[i - 2];
	}
	std::cin >> N;
	std::cout << DP[N];
	//N = 100;
	//while (N--) std::cout << DP[N] << "\n";
	return 0;
}