#include <iostream>
#include <algorithm>
typedef long long ll;
const int LEN = 2'250;
int N, u, l;
ll G[LEN][LEN], DP[LEN][LEN];



int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			std::cin >> G[i][j];
		}
	}
	G[0][1] = G[1][0] = 1e9;
	for (int i = 2; i <= N; i++) {
		DP[i][0] = 1e9;
		DP[0][i] = 1e9;
	}
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			u = G[i - 1][j] > G[i][j] ? 0 : G[i][j] - G[i - 1][j] + 1;
			l = G[i][j - 1] > G[i][j] ? 0 : G[i][j] - G[i][j - 1] + 1;
			DP[i][j] = std::min({ DP[i - 1][j] + u, DP[i][j - 1] + l });
		}
	}

	std::cout << DP[N][N] << "\n";
	return 0;
}
	//std::cout << "\n";
	//for (int i = 1; i <= N; i++) {
	//	for (int j = 1; j <= N; j++) {
	//		std::cout << DP[i][j] << " ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n";