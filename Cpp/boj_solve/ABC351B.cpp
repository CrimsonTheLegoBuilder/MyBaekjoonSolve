#include <iostream>
#include <algorithm>
#include <cmath>
typedef long long ll;
const int LEN = 1e2 + 5;
int N;
char A[LEN][LEN];
char B[LEN][LEN];

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> A[i];
	for (int i = 0; i < N; i++) std::cin >> B[i];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (A[i][j] != B[i][j]) {
				std::cout << i + 1 << " " << j + 1 << "\n";
				return;
			}
		}
	}
	//return;
}
int main() { solve(); return 0; }
