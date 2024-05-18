#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
typedef long long ll;
const int LEN = 1e5;
int N, R, x;
std::string S[100];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> S[i] >> x;
		R += x;
	}
	std::sort(S, S + N);
	std::cout << S[R % N] << "\n";
	return;
}
int main() { solve(); return 0; }
