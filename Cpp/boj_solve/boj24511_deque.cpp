#include <iostream>
#include <algorithm>
#include <queue>
#include <deque>
const int LEN = 1e6;
int N, M, X;
bool SEQ[LEN];
std::deque<int> Q;

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) { std::cin >> SEQ[i]; }
	for (int i = 0; i < N; i++) { 
		std::cin >> X;
		if (!SEQ[i]) Q.push_front(X);
	}
	std::cin >> M;
	for (int i = 0; i < M; i++) {
		std::cin >> X;
		Q.push_back(X);
		std::cout << Q.front() << " ";
		Q.pop_front();
	}
	return 0;
}