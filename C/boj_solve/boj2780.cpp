#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const ll MOD = 1'234'567;
int T, N, S;
ll MAP[1001][10]{ 0 };

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);

	for (int i = 0; i < 10; i++) MAP[1][i] = 1;
	for (int j = 2; j <= 1'000; j++) {
		MAP[j][0] += (MAP[j - 1][7]) % MOD;
		MAP[j][1] += (MAP[j - 1][2] + MAP[j - 1][4]) % MOD;
		MAP[j][2] += (MAP[j - 1][1] + MAP[j - 1][3] + MAP[j - 1][5]) % MOD;
		MAP[j][3] += (MAP[j - 1][2] + MAP[j - 1][6]) % MOD;
		MAP[j][4] += (MAP[j - 1][1] + MAP[j - 1][5] + MAP[j - 1][7]) % MOD;
		MAP[j][5] += (MAP[j - 1][2] + MAP[j - 1][4] + MAP[j - 1][6] + MAP[j - 1][8]) % MOD;
		MAP[j][6] += (MAP[j - 1][3] + MAP[j - 1][5] + MAP[j - 1][9]) % MOD;
		MAP[j][7] += (MAP[j - 1][4] + MAP[j - 1][8] + MAP[j - 1][0]) % MOD;
		MAP[j][8] += (MAP[j - 1][5] + MAP[j - 1][7] + MAP[j - 1][9]) % MOD;
		MAP[j][9] += (MAP[j - 1][6] + MAP[j - 1][8]) % MOD;
	}

	std::cin >> T;
	while (T--) {
		std::cin >> N;
		S = 0;
		for (int i = 0; i < 10; i++) S += MAP[N][i], S %= MOD;
		std::cout << S << "\n";
	}

	return;
}
int main() { solve(); return 0; }


//void solve() {
//	std::cin.tie(0)->sync_with_stdio(0);
//	std::cout.tie(0);
//
//	for (int i = 0; i < 10; i++) MAP[1][i] = 1;
//	for (int j = 2; j <= 1'000; j++) {
//		MAP[j][0] += MAP[j - 1][7] % MOD;
//
//		MAP[j][1] += MAP[j - 1][2] % MOD;
//		MAP[j][1] += MAP[j - 1][4] % MOD;
//
//		MAP[j][2] += MAP[j - 1][1] % MOD;
//		MAP[j][2] += MAP[j - 1][3] % MOD;
//		MAP[j][2] += MAP[j - 1][5] % MOD;
//
//		MAP[j][3] += MAP[j - 1][2] % MOD;
//		MAP[j][3] += MAP[j - 1][6] % MOD;
//
//		MAP[j][4] += MAP[j - 1][1] % MOD;
//		MAP[j][4] += MAP[j - 1][5] % MOD;
//		MAP[j][4] += MAP[j - 1][7] % MOD;
//
//		MAP[j][5] += MAP[j - 1][2] % MOD;
//		MAP[j][5] += MAP[j - 1][4] % MOD;
//		MAP[j][5] += MAP[j - 1][6] % MOD;
//		MAP[j][5] += MAP[j - 1][8] % MOD;
//
//		MAP[j][6] += MAP[j - 1][3] % MOD;
//		MAP[j][6] += MAP[j - 1][5] % MOD;
//		MAP[j][6] += MAP[j - 1][9] % MOD;
//
//		MAP[j][7] += MAP[j - 1][4] % MOD;
//		MAP[j][7] += MAP[j - 1][8] % MOD;
//		MAP[j][7] += MAP[j - 1][0] % MOD;
//
//		MAP[j][8] += MAP[j - 1][5] % MOD;
//		MAP[j][8] += MAP[j - 1][7] % MOD;
//		MAP[j][8] += MAP[j - 1][9] % MOD;
//
//		MAP[j][9] += MAP[j - 1][6] % MOD;
//		MAP[j][9] += MAP[j - 1][8] % MOD;
// 
//		for (int i = 0; i < 10; i++) MAP[j][i] %= MOD;
//	}
//
//	std::cin >> T;
//	while (T--) {
//		std::cin >> N;
//		S = 0;
//		for (int i = 0; i < 10; i++) S += MAP[N][i], S %= MOD;
//		std::cout << S << "\n";
//	}
//
//	return;
//}