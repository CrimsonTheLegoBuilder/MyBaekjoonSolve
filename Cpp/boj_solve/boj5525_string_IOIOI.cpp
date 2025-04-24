#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <string>
#include <cmath>

int N, M;
std::string S;
void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N >> M >> S;
	int cnt = 0, ret = 0;
	for (int i = 0, j; i < M - 2 * N + 1; i = j + 1) {
		if (S[i] == 'I') {
			j = i;
			cnt = 0;
			while (j < M - 2 && S[j + 1] == 'O' && S[j + 2] == 'I')
				j += 2, cnt++;
			if (cnt >= N) ret += cnt - N + 1;
		}
		else j = i;
	}
	std::cout << ret << "\n";
	return;
}
int main() { solve(); return 0; }//boj5525 IOIOI