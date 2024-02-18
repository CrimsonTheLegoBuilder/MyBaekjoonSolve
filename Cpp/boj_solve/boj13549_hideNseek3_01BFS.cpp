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