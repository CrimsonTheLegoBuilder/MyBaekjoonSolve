#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
const int LEN = 105;
int W, H;
char MAP[LEN][LEN];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> H >> W;
	for (int i = 0; i < H; i++) std::cin >> MAP[i];
	int cnt = 0, cur = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (MAP[i][j] == '/' || MAP[i][j] == '\\') cur ^= 1;
			cnt += cur;
		}
	}
	//std::cout << cnt;
	return;
}
int main() { solve(); return 0; }//boj3495