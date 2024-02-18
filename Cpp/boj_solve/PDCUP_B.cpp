#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

int N, M, row, col;
char arr[300][800];

int main() {
	std::cin >> N >> M;
	row = N * 3;
	col = M * 8;
	for (int i = 0; i < row; i++) std::cin >> arr[i];
	for (int i = 1; i < row; i += 3) {
		for (int j = 0; j < col; j += 8) {
			int a, b, c, d, e;
			a = arr[i][j + 1] - '0';
			b = arr[i][j + 3] - '0';
			d = arr[i][j + 5] - '0';
			e = arr[i][j + 6] - '0';
			if (0 <= e && e <= 9) c = 10 * d + e;
			else c = d;
			if (a + b == c) {
				for (int k = 1; k <= 5; k++) {
					arr[i - 1][j + k] = '*';
					arr[i + 1][j + k] = '*';
				}
				arr[i][j] = '*';
				if (c > 9) {
					arr[i - 1][j + 6] = '*';
					arr[i][j + 7] = '*';
					arr[i + 1][j + 6] = '*';
				}
				else
					arr[i][j + 6] = '*';
			}
			else {
				arr[i - 1][j + 3] = '/';
				arr[i][j + 2] = '/';
				arr[i + 1][j + 1] = '/';
			}
		}
	}
	for (int i = 0; i < row; i++) std::cout << arr[i] << "\n";
	return 0;
}

