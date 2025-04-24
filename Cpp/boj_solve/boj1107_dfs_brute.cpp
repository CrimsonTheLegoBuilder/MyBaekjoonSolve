#include <iostream>
#include <algorithm>
bool button[10];
int N, MAX;

void DFS(int x, int d, int s) {
	if (d >= x) {
		//if (x==1) std::cout << s << std::abs(N - s) + d;
		MAX = std::min({ MAX, std::abs(N - s) + d });
		return;
	}
	for (int i = 0; i < 10; i++) {
		if (button[i]) {
			DFS(x, d + 1, s * 10 + i);
		}
	}
}

int main() {
	int M, b;
	for (int i = 0; i < 10; i++) {
		button[i] = 1;
	}
	std::cin >> N >> M;
	MAX = std::abs(N - 100);
	//std::cout << MAX << "\n";
	for (int m = 0; m < M; m++) {
		std::cin >> b;
		button[b] = 0;
	}
	for (int x = 1; x < 7; x++) {
		DFS(x, 0, 0);
		//std::cout << MAX << "\n";
	}
	std::cout << MAX;
	return 0;
}