#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstring>
typedef long long ll;
const int LEN = 2'001;
int N, M, R[LEN], C[LEN];
char board[LEN][LEN];

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> board[i];
	}
	for (int i = 0; i < N; i++) {
		ll row = 0, col = 0;
		for (int j = 0; j < N; j++) {
			row += board[i][j] == 'o';
			col += board[j][i] == 'o';
		}
		R[i] = row;
		C[i] = col;
	}
	ll cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == 'o') {
				cnt += (R[i] - 1) * (C[j] - 1);
			}
		}
	}
	std::cout << cnt << "\n";
	return 0;
}