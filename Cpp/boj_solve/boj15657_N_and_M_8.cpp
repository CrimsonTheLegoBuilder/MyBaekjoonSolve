#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
typedef long long ll;
int N, M, arr[10];
std::vector<int> stack;

void dfs(int s, int x) {
	if (x == M) {
		for (const int& num : stack) {
			std::cout << num << " ";
		}
		std::cout << "\n";
		return;
	}
	for (int i = s; i < N; i++) {
		stack.push_back(arr[i]);
		dfs(i, x + 1);
		stack.pop_back();
	}
	return;
}

int main() {
	std::cin >> N >> M;
	for (int i = 0; i < N; i++) std::cin >> arr[i];
	std::sort(arr, arr + N);
	dfs(0, 0);
	return 0;
}