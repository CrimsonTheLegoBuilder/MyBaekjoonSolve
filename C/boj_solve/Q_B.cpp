#include <iostream>
const int LEN = 200'001;
typedef long long ll;
ll N, A[LEN], B[LEN], cnt, diff;

int main() {
	std::cin >> N;
	for (int i = 1; i <= N; i++) {
		std::cin >> A[i];
	}
	for (int i = 1; i <= N; i++) {
		if (A[i] != B[i]) {
			diff = A[i] - B[i];
			cnt += std::abs(diff);
			for (int j = i; j <= N; j += i) {
				B[j] += diff;
			}
		}
	}
	std::cout << cnt;
	return 0;
}