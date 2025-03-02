#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;

int N, T, A, B, P, Q;
int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> T;
	while (T--) {
		std::cin >> A >> B;
		P = A * B;
		Q = 0; N = 1;
		int n = 1;
		while (A && B) {
			int a = A % 10;
			int b = B % 10;
			//std::cout << "a:: " << a << " b:: " << b << "\n";
			Q += (a * b) * N;
			A /= 10;
			B /= 10;
			n = 1;
			if (a * b >= 10) n = 10;
			N *= 10 * n;
		}
		Q += A * N;
		Q += B * N;
		//std::cout << "P:: " << P << " Q:: " << Q << "\n";
		std::cout << (P == Q ? "1\n" : "0\n");
	}
	return 0;
}