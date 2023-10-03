#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <algorithm>
const int LEN = 2'001;
int S1[LEN][LEN]{ 0 };
char B[LEN][LEN];

int main() {
	int n, m, k, sum;
	std::cin >> n >> m >> k;
	sum = k * k;
	for (int i = 0; i < n; i++) {
		std::cin >> B[i];
	}
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			if (((i + j) % 2 && B[i - 1][j - 1] == 'W') || (!((i + j) % 2) && B[i - 1][j - 1] == 'B')) {
				S1[i][j] = S1[i][j - 1] + S1[i - 1][j] - S1[i - 1][j - 1];
			}
			else {
				S1[i][j] = S1[i][j - 1] + S1[i - 1][j] - S1[i - 1][j - 1] + 1;
			}
		}
	}
	for (int i = 0; i < n + 1 - k; i++) {
		for (int j = 0; j < m + 1 - k; j++) {
			int sum1, sum2;
			sum1 = S1[i + k][j + k] - S1[i][j + k] - S1[i + k][j] + S1[i][j];
			sum2 = k * k - sum1;
			sum = std::min({ sum, sum1, sum2 });
		}
	}
	std::cout << sum << "\n";
	return 0;
}

			//if (sum1 < 0) sum1 = -sum1;
