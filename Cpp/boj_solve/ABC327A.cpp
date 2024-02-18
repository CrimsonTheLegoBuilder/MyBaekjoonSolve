#include <iostream>
#include <algorithm>
#include <cstring>
int N;
std::string A;
bool f = 0;

int main() {
	std::cin >> N >> A;
	for (int i = 0; i < N - 1; i++) {
		if (A[i] == 'a' && A[i + 1] == 'b' || A[i] == 'b' && A[i + 1] == 'a') f = 1;
	}
	std::cout << (f ? "Yes" : "No") << "\n";
	return 0;
}