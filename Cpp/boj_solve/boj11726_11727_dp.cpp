#include <iostream>
#include <algorithm>
typedef long long ll;
const ll MOD = 10'007;
ll arr[1005];

void solve1() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N;
	std::cin >> N;
	arr[1] = 1;
	arr[2] = 2;
	if (N <= 2) {
		std::cout << arr[N] << "\n";
		return;
	}
	for (int i = 3; i < N + 1; i++) arr[i] = (arr[i - 1] + arr[i - 2]) % MOD;
	std::cout << arr[N] << "\n";
	return;
}//boj11726
void solve2() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	int N;
	std::cin >> N;
	arr[1] = 1;
	arr[2] = 3;
	if (N <= 2) {
		std::cout << arr[N] << "\n";
		return;
	}
	for (int i = 3; i < N + 1; i++) arr[i] = (arr[i - 1] + arr[i - 2] * 2) % MOD;
	std::cout << arr[N] << "\n";
	return;
}//boj11727
int main() { solve1(); return 0; }