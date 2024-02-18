#include <iostream>

int main() {
	int N, a[100]{};
	bool f = 1;
	std::cin >> N;
	for (int i = 0; i < N; i++) {
		std::cin >> a[i];
	}
	for (int i = 1; i < N; i++) {
		if (a[i - 1] != a[i]) f = 0;
	}
	std::cout << (f ? "Yes" : "No");
}