#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

typedef long long ll;
const int LEN = 1e5;
int N, num[LEN], memo[LEN + 1];
ll S;

int main() {
	std::cin >> N >> S;
	for (int i = 0; i < N; i++) std::cin >> num[i];
	for (int i = 1; i <= N; i++) {
		memo[i] = memo[i - 1] + num[i - 1];
		//std::cout << memo[i] << " ";
	}
	//std::cout << "\n";
	int step = 1e7;
	for (int i = 0, j = 1; i < N; i++) {
		while (j <= N && memo[j] - memo[i] < S) {
			//std::cout << memo[j] - memo[i] << "\n";
			j++;
		}
		if (j <= N && memo[j] - memo[i] >= S)
			step = std::min(step, j - i);
	}
	std::cout << (step > 1e6 ? 0 : step) << "\n";
	return 0;
}