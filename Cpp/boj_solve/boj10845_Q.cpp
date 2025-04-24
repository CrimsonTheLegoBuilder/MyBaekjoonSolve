#include <iostream>
#include <queue>
std::queue<int> Q;

void solve(int N) {
	std::string S;
	int x;
	while (N--) {
		std::cin >> S;
		if (S == "push") {
			std::cin >> x;
			Q.push(x);
		}
		if (S == "pop") {
			if (Q.empty()) std::cout << "-1\n";
			else {
				std::cout << Q.front() << "\n";
				Q.pop();
			}
		}
		if (S == "size") std::cout << Q.size() << "\n";
		if (S == "empty") std::cout << Q.empty()<< "\n";
		if (S == "front") {
			if (Q.empty()) std::cout << "-1\n";
			else std::cout << Q.front() << "\n";
		}
		if (S == "back") {
			if (Q.empty()) std::cout << "-1\n";
			else std::cout << Q.back() << "\n";
		}
	}
}

int main() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N;
	std::cin >> N;
	solve(N);
	return 0;
}