#include <iostream>
#include <deque>
const int LEN = 100'001;
int N, K, V[LEN];
std::deque<int> Q;

int BFS01(int v, int K) {
	for (int i = 0; i < LEN; i++) V[i] = LEN;
	V[v] = 0;
	Q.push_back(v);
	while (!Q.empty()) {
		int p = Q.front(); Q.pop_front();
		if (0 <= p - 1 && V[p] + 1 < V[p - 1]) {
			V[p - 1] = V[p] + 1; Q.push_back(p - 1);
		}
		if (p + 1 < LEN && V[p] + 1 < V[p + 1]) {
			V[p + 1] = V[p] + 1; Q.push_back(p + 1);
		}
		if (p << 1 < LEN && V[p] < V[p << 1]) {
			V[p << 1] = V[p]; Q.push_front(p << 1);
		}
		//std::cout << p << " " << V[p] << "\n";
	}
	return V[K];
}



int main() {
	std::cin >> N >> K;
	std::cout << BFS01(N, K) << "\n";
	return 0;
}

/*

#include <iostream>
const int LEN = 100'001;
int N, K, DP[LEN];


int main() {
	std::cin >> N >> K;
	if (N >= K) std::cout << N - K << "\n";
	else {
		for (int i = N / 2; i <= K; i++) {
			DP[i] = std::abs(i - N);
		}
		for (int i = N; i <= K; i++) {
			DP[i] = std::min({ DP[i], DP[i - 1] + 1, DP[i / 2 + i % 2] + i % 2, DP[i / 2] + i % 2 });
			if (i << 1 < LEN) DP[i << 1] = std::min({ DP[i], DP[i << 1] });
			if (i - 1 >= 0) DP[i - 1] = std::min({ DP[i] + 1, DP[i - 1] });
		}
		std::cout << DP[K] << "\n";
	}
	return 0;
}

*/