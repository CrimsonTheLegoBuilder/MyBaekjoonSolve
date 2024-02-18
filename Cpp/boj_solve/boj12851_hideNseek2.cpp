#include <iostream>
#include <queue>
const int LEN = 100'001;
int v, N, V[LEN], C[LEN];
std::queue<int> Q;

int BFS(int v, int N) {
	for (int i = 0; i < LEN; i++) V[i] = LEN;
	V[v] = 0;
	C[v]++;
	Q.push(v);
	while (!Q.empty()) {
		int p = Q.front(); Q.pop();
		if (0 <= p - 1 && V[p] + 1 <= V[p - 1]) {
			V[p - 1] = V[p] + 1; C[p - 1]++; Q.push(p - 1);
		}
		if (p + 1 < LEN && V[p] + 1 <= V[p + 1]) {
			V[p + 1] = V[p] + 1; C[p + 1]++; Q.push(p + 1);
		}
		if (p << 1 < LEN && V[p] + 1 <= V[p << 1]) {
			V[p << 1] = V[p] + 1; C[p << 1]++; Q.push(p << 1);
		}
		//std::cout << p << " " << V[p] << "\n";
	}
	return V[N];
}



int main() {
	std::cin >> v >> N;
	std::cout << BFS(v, N) << "\n" << C[N] << "\n";
	return 0;
}