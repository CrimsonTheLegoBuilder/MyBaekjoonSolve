#include <iostream>
#include <algorithm>
#include <vector>
typedef long long ll;
ll X[16] = {
	0,
	1,
	4,
	27,
	256,
	3125,
	46656,
	823543,
	16777216,
	387420489,
	10000000000,
	285311670611,
	8916100448256,
	302875106592253,
	11112006825558016,
	437893890380859375
};
ll N;

int main() {
	std::cin >> N;
	for (int i = 0; i < 16; i++) {
		if (N == X[i]) {
			std::cout << i;
			return 0;
		}
	}
	std::cout << -1;
	return 0;
}