#include <iostream>
typedef long long ll;

int main() {
	ll N;
	std::cin >> N;
	while (!(N % 2)) {
		N /= 2;
	}
	while (!(N % 3)) {
		N /= 3;
	}
	std::cout << (N == 1 ? "Yes" : "No");
}