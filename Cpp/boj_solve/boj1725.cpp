#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
typedef long long ll;
const int LEN = 1e5 + 1;
int N;
ll H[LEN];
std::vector<int> stack;

void solve() {
	std::cin.tie(0)->sync_with_stdio(0);
	std::cout.tie(0);
	std::cin >> N;
	for (int i = 0; i < N; i++) std::cin >> H[i];
	ll ans = 0;
	for (int i = 0; i < N; i++) {
		while (stack.size() && H[i] > H[stack.back()]) {
			ans;
			stack.pop_back();
		}
		ans;
	}
	std::cout << ans;
}
int main() { solve(); return 0; }//boj1725 histogram
