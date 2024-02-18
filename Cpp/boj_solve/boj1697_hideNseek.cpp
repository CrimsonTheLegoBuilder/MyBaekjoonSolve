#include <iostream>
#include <queue>
const int LEN = 100'001;
int N, K, V[LEN];
std::queue<int> Q;

int BFS(int v, int K) {
	V[v] = 1;
	Q.push(v);
	while (!Q.empty()) {
		int p = Q.front(); Q.pop();
		if (0 <= p - 1 && !V[p - 1]) {
			V[p - 1] = V[p] + 1; Q.push(p - 1);
		}
		if (p + 1 < LEN && !V[p + 1]) {
			V[p + 1] = V[p] + 1; Q.push(p + 1);
		}
		if (p << 1 < LEN && !V[p << 1]) {
			V[p << 1] = V[p] + 1; Q.push(p << 1);
		}
		//std::cout << p << " " << V[p] << "\n";
		if (p == K) return V[p] - 1;
	}
}
int main() {
	std::cin >> N >> K;
	std::cout << BFS(N, K);
	return 0;
}