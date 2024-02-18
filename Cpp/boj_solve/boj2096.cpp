#include <iostream>
#include <cmath>
#include <algorithm>
//const int LEN = 100001;

int dph[2][3]{};
int dpl[2][3]{};
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	int N;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		int a, b, c;
		std::cin >> a >> b >> c;
		dph[i & 1][0] = a + std::max(dph[~i & 1][0], dph[~i & 1][1]);
		dph[i & 1][1] = b + std::max({ dph[~i & 1][0], dph[~i & 1][1], dph[~i & 1][2] });
		dph[i & 1][2] = c + std::max(dph[~i & 1][1], dph[~i & 1][2]);
		dpl[i & 1][0] = a + std::min(dpl[~i & 1][0], dpl[~i & 1][1]);
		dpl[i & 1][1] = b + std::min({ dpl[~i & 1][0], dpl[~i & 1][1], dph[~i & 1][2] });
		dpl[i & 1][2] = c + std::min(dpl[~i & 1][1], dpl[~i & 1][2]);
	}
	std::cout << std::max({ dph[~N & 1][0], dph[~N & 1][1], dph[~N & 1][2] }) << " ";
	std::cout << std::min({ dpl[~N & 1][0], dpl[~N & 1][1], dpl[~N & 1][2] }) << "\n";
	return 0;
}