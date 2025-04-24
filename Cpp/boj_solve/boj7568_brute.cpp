#include <iostream>
#include <vector>
#include <algorithm>
const int LEN = 50;

struct Info {
	int w, h;
	bool operator < (const Info& i) { return w < i.w && h < i.h; }
}I[LEN];


int main() {
	int N;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> I[i].w >> I[i].h;
	}
	int S[LEN]{};
	for (int i = 0; i < N; i++) {
		int cnt = 1;
		for (int j = 0; j < N; j++) {
			if (i != j) {
				cnt += (I[i] < I[j]);
			}
		}
		S[i] = cnt;
	}
	for (int i = 0; i < N; i++) {
		std::cout << S[i] << " ";
	}
	return 0;
}