#include <iostream>
typedef long long ll;
ll N, R, C, S;

int main() {
	std::cin >> N >> R >> C;
	for (int i = 0; i < N; i++) {
		S |= ((R & 1) * 2 | (C & 1)) * (1 << (i * 2));
		R >>= 1, C >>= 1;
	}
	std::cout << S;
}